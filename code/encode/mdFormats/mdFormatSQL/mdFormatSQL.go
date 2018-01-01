package mdFormatSQL

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"database/sql"
	"fmt"
	"time"
	"strings"
	"log"
	"os"
	_ "github.com/mattn/go-sqlite3"
)


type MdFormat struct {
	commandArgs int
	fileID uint64 
	fileName string
        filePath string
        fileSize uint64
        blockSize uint64
        blockCount uint64
        blockRemainder uint64
	// the current block number
        blockNumber uint64
        modSize uint64
        modExponent uint64
        mdFormat int
        mdVersion string
	// md hash list bits
	mdfileHashListBitString string
        mdblockHashListBitString string
	// md hash list string
	mdfileHashListString string
        mdblockHashListString string
	mdHashlist string
	// output file variables
	outputFile string
	file *os.File
	// err
	err     error
	// go sqlite3 variables
	db      *sql.DB
}

var version string = "1.0"

// Init returns a new sqlite3 db md file type
// this writes to the file and block signatures to the sqlite3 db file.
func Init(encodingFormat int, fileName string, filePath string, fileSize uint64, blockSize uint64, modulusSize uint64, fileHashListString string, blockHashListString string, outputfileName string) (mdfmt *MdFormat) {

	md := new(MdFormat)
	md.mdFormat = encodingFormat
	md.fileName = fileName
	md.filePath = filePath
	md.fileSize = fileSize
	md.blockSize = blockSize
	md.modSize = modulusSize
	md.mdVersion = version 
        // set the input hashlist bit string
        md.mdfileHashListBitString  = fileHashListString
        md.mdblockHashListBitString = blockHashListString
	// set the input hashlist string
        md.mdfileHashListString  = fileHashListString
        md.mdblockHashListString = blockHashListString
        // set the output file name
	md.outputFile = outputfileName
	md.fileID = 0
	md.blockNumber = 0

        // detect if the db file exists
        // var dbfile string = outputfileName // + ".db"
        var dbfile string = outputfileName 
	if info, err := os.Stat(fileName); err == nil && info.IsDir() {
		log.Fatal("The dbfile path is a directory")
		os.Exit(3)
	}
	// need to check if this not a directory
        // err = os.Remove(dbfile)
        // if err == nil || os.IsNotExist(err) {
        // }

        db, err := sql.Open("sqlite3", dbfile)
	md.db = db
        // initialize the error handle
        md.err = err 
	md.checkError()

	// mdata.sqlCreateTable()

	// defer db.Close()
	return md
}

// OpenFile
// create the db file handle
func (md *MdFormat) OpenFile(append bool) {

        // detect if the db file exists
        // var dbfile string = outputfileName // + ".db"
	var outputfileName = md.outputFile
        var dbfile string = outputfileName
        // if info, err := os.Stat(fileName); err == nil && info.IsDir() {
        if info, err := os.Stat(dbfile); err == nil && info.IsDir() {
                log.Fatal("The dbfile path is a directory")
                os.Exit(3)
        }
        // need to check if this not a directory
        // err = os.Remove(dbfile)
        // if err == nil || os.IsNotExist(err) {
        // }

        db, err := sql.Open("sqlite3", dbfile)
        md.db = db
        // initialize the error handle
        md.err = err 
        md.checkError()

	defer md.db.Close()

}

// InitFile 
// init file methods
func (md *MdFormat) InitFile() {
	md.SqlCreateTable()

}

// SqlCreateTable 
// generates the sql to create the md tables
//
// mdfilelist
// mdfile
// mdfilehash
// mdfileblocks 
// mdbyteblocks
func (md *MdFormat) SqlCreateTable() {

	// clean the table prior to inserting
	// create the author table
	// sqlStmt := `CREATE TABLE IF NOT EXISTS mdmetalist (fileid integer primary key autoincrement, date varchar(30), author text, corp text, description text, version text);`

        // create the mdfilelist table
	// added the current directory path
        sqlStmt := `CREATE TABLE IF NOT EXISTS mdfilelist (fileid integer primary key autoincrement, date varchar(30), filename text, currentpath text, filepath text, version text);`
        _, md.err = md.db.Exec(sqlStmt)
	md.checkError()

	// mdfile prefix
	// create the mdfile table
	_, md.err = md.db.Exec(`
        create table if not exists mdfile (id integer primary key autoincrement, fileid integer, filename text, filepath text, filesize text, blocksize integer, modulusbitsize integer, filehashlist text, blockhashlist text, filehashlistbits text, blockhashlistbits text, date text);
        `)
	md.checkError()

	// mdfilehash
	// create the mdfilehash table
        _, md.err = md.db.Exec(`
        create table if not exists mdfilehash (id integer primary key autoincrement, fileid integer, hashname text, hashhex text, modexp integer, modremainder text);
        `)
	md.checkError()

	// mdfileblocks
	// create the mdfileblocks table
	_, md.err = md.db.Exec(`
	create table if not exists mdfileblocks (id integer primary key autoincrement, fileid integer, blocksize integer, blocknumber integer, hashname text, hashhex text, collision text, window text,  modexp integer, modremainder text);
	`)
	md.checkError()

	// mdfile byte blocks table
	// add an option mdfile byte blocks table
	// it has the file byte blocks

	_, md.err = md.db.Exec("BEGIN TRANSACTION;")
        md.checkError()

	md.insertFile()
}

// insertFile
// insert the file into the mdfilelist table
// calculate the current file id from the db that was created
// this will associate the block signatures with the current file id
func (md *MdFormat) insertFile () {
	query := `INSERT INTO mdfilelist
                        (date, filename, filepath, version)
                        VALUES
                (?, ?, ?, ?);`
        args := []interface{}{
			time.Now(),
                        md.fileName,
                        md.filePath,
                        "1.0"}
        _, md.err = md.db.Exec(query, args...)
	md.checkError()

	// select the current file count from the mdfilelist to create the current file id
	// rows, err := md.db.Query("select count (*) from mdfilelist")
	rows, err := md.db.Query("select max(fileid)  from mdfilelist")

	md.err = err
	md.checkError()

	var id uint64 = 1 
	for rows.Next() {
		if err := rows.Scan(&id); err != nil {
			log.Fatal("Unable to scan results:", err)
			continue
		}

        }

	// initialize the fileID number
	md.fileID = id

        rows.Close() 

}

// EncodeFileHeader 
// add the file signature header to the sqlite3 db
// it contains the file md format type, filename, filepath, filesize, block signature size, file hash signature list, block hash signature list and modulus bit size 
// it can also add time attribute
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) {

	var fileListString = strings.Join(filehashList, ":")
	md.mdfileHashListString = fileListString

	var hashListString = strings.Join(blockhashList, ":")
	md.mdHashlist = hashListString
	md.mdblockHashListString = hashListString

	// fsize, err := strconv.ParseInt(fileSize, 10, 64)

        query := `INSERT INTO mdfile
                        (fileid, filename, filepath, filesize, blocksize, modulusbitsize, filehashlist, blockhashlist, filehashlistbits, blockhashlistbits, date)
                        VALUES
                (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);`
        args := []interface{}{
			md.fileID,
			fileName,
			filePath,
			fileSize,
			// fsize,
			blockSize,
			modulusSize,
			fileListString,
                        hashListString,
		        md.mdfileHashListBitString,
			md.mdblockHashListBitString,
			time.Now()}
         _, md.err = md.db.Exec(query, args...)
	md.checkError()


}

// EncodeFileHash 
// add the file signature to the mdfilehash sqlite3 table
// this is the entire file signature file id, hash type, and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

	query := `INSERT INTO mdfilehash
			(fileid, hashname, hashhex, modexp, modremainder)
			VALUES
		(?, ?, ?, ?, ?);`
	args := []interface{}{
			md.fileID,
			hashName,
			hashBytes,
			"0",
			"0"}
	_, md.err = md.db.Exec(query, args...)
	md.checkError()
}


// EncodeBlock
// add the block signature to the mdfileblocks table in the sqlite3 db 
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	var hashListString = strings.Join(hashList, ":")

// id integer, fileid integer, blocksize integer, blocknumber integer, hashname text, hashhex text, modexp integer, modremainder text
// have to think about this whether to have each block sig in one row or separate rows

        query := `INSERT INTO mdfileblocks
                        (fileid, blocksize, blocknumber, hashname, hashhex, modexp, modremainder)
                        VALUES
                (?, ?, ?, ?, ?, ?, ?);`
        args := []interface{}{
                        md.fileID,
			blockSize,
			md.blockNumber,
                        // hashName,
			// "hashname",
			md.mdHashlist,
                        hashListString,
                        modExp,
			mod}
	_, md.err = md.db.Exec(query, args...)
	md.checkError()

	md.blockNumber += 1

}


// EncodeEndFile
// this is the format end file method executed at the end 
func (md *MdFormat) EncodeEndFile(encodingFormat int) {

	// commit to db
	_, md.err = md.db.Exec("END TRANSACTION;")
        md.checkError()

	md.db.Close()
}



// md print
// this can either write to a file or print to standard out
// included to satisfy the interface
func (md *MdFormat) print(line  ...interface{}) {

        if md.outputFile != "" {
                fmt.Fprint(md.file, line...)
        } else {
                fmt.Print(line...); 
        }

}

// md println
// this can either write to a file or print to standard out
// included to satisfy the interface
func (md *MdFormat) println(line  ...interface{}) {

	if md.outputFile != "" {
		fmt.Fprint(md.file, line...)
		fmt.Fprint(md.file, "\n")
	} else {
		fmt.Print(line...);
		fmt.Print("\n")
	}

}

// test inteface function
func (md *MdFormat) PrintFormatType() {
        fmt.Println("Format mdFormatSQL")
}

// check the error
func (md *MdFormat) checkError() {
        if md.err != nil {
            log.Fatal("mdfile error ", md.err)
        }

}

