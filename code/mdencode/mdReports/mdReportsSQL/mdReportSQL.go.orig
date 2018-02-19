package mdReportSQL

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE


import (
	"database/sql"
	"fmt"
	"strings"
	"log"
	"os"
	"strconv"
	"github.com/singularian/mdencode/code/encode/mdFormatsImport/mdFormatImportSQL"
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

// PrintFileList
// Display the files list of md encode file hash signatures in the mdfilelist table
func (md *MdFormat) PrintFileList(format int, fileid uint64) {

        rows, err := md.db.Query("select fileid, filename, filepath, version from mdfilelist")
        md.err = err
        md.checkError()

        var id uint64
        var filename string
        var filepath string
        var version string
        for rows.Next() {
                if err := rows.Scan(&id, &filename, &filepath, &version); err != nil {
                        log.Fatal("Unable to scan results:", err)
                        continue
                }
                fmt.Printf("file %d: %s %s %s\n", id, filepath, filename, version)
        }

        rows.Close()

}

// PrintEntireFileList 
// display all the files in the mdfilelist
func (md *MdFormat) PrintEntireFileList(format int) {

        rows, err := md.db.Query("select fileid, filename, filepath, version from mdfilelist")
        md.err = err
        md.checkError()

        var id uint64
        var filename string
        var filepath string
        var version string
        for rows.Next() {
                if err := rows.Scan(&id, &filename, &filepath, &version); err != nil {
                        log.Fatal("Unable to scan results:", err)
                        continue
                }
                // fmt.Printf("file %d: %s %s %s\n", id, filepath, filename, version)
		// fmt.Print("test ", id)
		md.PrintReport(format, filename, id)
        }

        rows.Close()

}

// PrintReport
// generates a formatted report from the sql file with a fileid identifier number
func (md *MdFormat) PrintReport(format int, fileName string, fileid uint64) {

        args := []interface{}{fileid}
	rows, err := md.db.Query("select fileid, filename, filepath, blocksize, filesize, modulusbitsize, filehashlist, blockhashlist, filehashlistbits, blockhashlistbits, date from mdfile where fileid=?", args...)
        md.err = err
        md.checkError()

        var rowid uint64 = 0
        var id uint64
	var filename string
	var filepath string
	var filesize string
	var blocksize string
	var modulusbitsize string
	var filehashlist string
	var blockhashlist string
	var filehashlistbits string
	var blockhashlistbits string
	var date string
	// var md *mdFormatText.MdFormat
	//md := mdFormatText.Init(format, fileName, fdata.filePath, fdata.fileSize, fdata.blockSize, fdata.modSize, outputfileName)
	// mdfmt := mdFormatText.Init(format, fileName, "", 0, 0, 0, "0", "0", "")
	// var mdfmt
	// mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, filehashlistbits, blockhashlistbits, "")
	mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", "")
	mdfmt := mdload.SetmdFormatNoSQL(true)
        for rows.Next() {
                if err := rows.Scan(&id, &filename, &filepath, &blocksize, &filesize, &modulusbitsize, &filehashlist, &blockhashlist, &filehashlistbits, &blockhashlistbits, &date); err != nil {
                        log.Fatal("Unable to scan results:", err)
                        continue
                }

		//mdfmt := mdFormatText.Init(format, fileName, "", 0, 0, 0, filehashlistbits, blockhashlistbits, "")
		// if rowid == 0 {
			// mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, filehashlistbits, blockhashlistbits, "")
			// mdload.SetByteBlock(false)
			// mdload.SetFileHashLine(false)
			// mdload.SetLogFile("")
			// mdload.SetKeyFile(l.key)
        		// mdfmt := mdload.SetmdFormatNoSQL(true)
		//}


		// currently empty
		var filelist = strings.Split(filehashlist, ":")
		var blocklist = strings.Split(blockhashlist, ":")
		// var hlist = strings.Split("thing:jaic:", ":") 
		// filesize64, err := strconv.ParseInt(filesize, 10, 64)
		// blocksize64, err := strconv.ParseInt(blocksize, 10, 64)
		// modulusbitsize64, err := strconv.ParseInt(modulusbitsize, 10, 64)
		filesize64, _ := strconv.ParseInt(filesize, 10, 64)
		blocksize64, _ := strconv.ParseInt(blocksize, 10, 64)
		modulusbitsize64, _ := strconv.ParseInt(modulusbitsize, 10, 64)

		// blocksize64, err := strconv.ParseUint(blocksize, 10, 64)
		// md.EncodeFileHeader(format, fileName, filepath, filesize, blocksize, hashlist, modulusbitsize)
		mdfmt.EncodeFileHeader(format, filename, filepath, filesize64, blocksize64, filelist, blocklist, modulusbitsize64)

		rowid++
        }

	rows.Close()

	rows, err = md.db.Query("select fileid, hashname, hashhex, modexp, modremainder from mdfilehash where fileid=?", args...)
	md.err = err
	md.checkError()

	var hashname string
	var hashhex string
	var modexp string
	var modremainder string
	for rows.Next() {
                if err := rows.Scan(&id, &hashname, &hashhex, &modexp, &modremainder); err != nil {
                        log.Fatal("Unable to scan results:", err)
                        continue
                }
		// fmt.Printf("z: %s:%s\n", hashname, hashhex)
		mdfmt.EncodeFileHash(format, hashname, hashhex)
        }

        rows.Close()

	rows, err = md.db.Query("select fileid, blocksize, blocknumber, hashname, hashhex, modexp, modremainder from mdfileblocks where fileid=?", args...)
	md.err = err
	md.checkError()

	var blocknumber string
	for rows.Next() {
                if err := rows.Scan(&id, &blocksize, &hashname, &blocknumber, &hashhex, &modexp, &modremainder); err != nil {
                        log.Fatal("Unable to scan results:", err)
                        continue
                }
		// fmt.Printf("%s%s%s%s%s%s%s%s%s\n", blocksize, ":", hashname, ":", hashhex, ":", modexp, ":", modremainder)
		// l.md.EncodeBlock(format, bytesRead, hlistarray, modExp, mod.String());
		blocksize64, _ := strconv.ParseUint(blocksize, 10, 64)
		modexpInt, _ := strconv.Atoi(modexp)
		// var hlistarray = strings.Split(hashlist, ":")
		var hlistarray = strings.Split(hashhex, ":")
		mdfmt.EncodeBlock(format, blocksize64, hlistarray, modexpInt, modremainder);
        }

	if filename != "" {
		mdfmt.EncodeEndFile(format)
	}

        rows.Close() 



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
        fmt.Println("Format mdReportsSQL")
}

// check the error
func (md *MdFormat) checkError() {
        if md.err != nil {
            log.Fatal("mdfile error ", md.err)
        }

}

