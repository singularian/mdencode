package mdFormatJson

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE


import (
	"fmt"
	"time"
	"strings"
	"encoding/json"
	"os"
)


type MdFormat struct {
	commandArgs int
	fileName string
        filePath string
        fileSize uint64
        blockSize uint64
        blockCount uint64
	fileCount uint64
        blockRemainder uint64
        modSize uint64
        modExponent uint64
        mdFormat int
        mdVersion string
	mdfileHashListString string
	mdblockHashListString string
	outputFile string
	file *os.File 
}

type jsonHeader struct {
	Mdtype string
	FileName string
	FilePath string
	Filesize int64 
	Blocksize int64 
	Modsize int64 
	Format int
	// Hashlist string
	FileHashlist string
	BlockHashlist string
	Version string
	Time string
}

type jsonFile struct {
	Mdtype string
	Hashname string
	Filehashlist string
}

type jsonBlock struct {
	Mdtype string
	Blocksize string
	Hashlist string
	Modexp string
	Mod string

}

var version string = "1.0"

// Init returns a new mdFormat object 
func Init(encodingFormat int, fileName string, filePath string, fileSize uint64, blockSize uint64, modulusSize uint64, fileHashListString string, blockHashListString string, outputfileName string) (mdfmt *MdFormat) {

	md := new(MdFormat)
	md.mdFormat = encodingFormat
	md.fileName = fileName
	md.filePath = filePath
	md.fileSize = fileSize
	md.blockSize = blockSize
	md.modSize = modulusSize
	md.blockCount = 0
	md.fileCount = 0
	// set the input hashlist string
	md.mdfileHashListString  = fileHashListString
	md.mdblockHashListString = blockHashListString
	// set the output file name
	md.outputFile = outputfileName
	md.mdVersion = version 


	return md
}


// append to a file or replace the file
// this is set based on the appendfile boolean
// if appendfile is true it appends to the file or creates a new file
// if appendfile is false it replaces the file
func (md *MdFormat) OpenFile(appendfile bool) {

        var outputfileName = md.outputFile

        if md.outputFile != "" {
                // if appendfile is set to true
                // append to the file or if it does not exists create a new file
                if appendfile {
                        file, err := os.OpenFile(outputfileName, os.O_APPEND|os.O_CREATE|os.O_WRONLY, 0644)
                        md.checkError(err)
                        defer md.file.Close()
                        // should be after the defer
                        md.file = file
                } else {
                        file, err := os.Create(outputfileName)
                        md.checkError(err)
                        defer md.file.Close()
                        md.file = file
                }

        }
}

// format init file methods
func (md *MdFormat) InitFile() {

}


// generate the file signature header
// it contains the file md format type, filename, filepath, filesize, block signature size, file hash signature list, block hash signature list and modulus bit size 
// it can also add time attribute
//
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) {

	// reset the block count
	md.blockCount = 0

	// set the file and block hash list
	var filehashListString = strings.Join(filehashList, ":")
        var blockhashListString = strings.Join(blockhashList, ":")

	// var fb = filehashListString + "-" + blockhashListString
	var time = fmt.Sprintf("%v", time.Now())

	mdj := jsonHeader{
			Mdtype: "MDheader",
			FileName: fileName, 
			FilePath: filePath, 
			Filesize: fileSize, 
			Blocksize: blockSize, 
			Modsize: modulusSize, 
			Format: encodingFormat, 
			FileHashlist: filehashListString, 
			BlockHashlist: blockhashListString, 
			Version: md.mdVersion, 
			Time: time}

	head, err := json.Marshal(mdj)
	md.checkError(err)

	md.print("{\"mdfile\": [")
	md.print(string(head))
	md.fileCount++

}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {


	mdj := jsonFile{
			Mdtype: "File",
                        Hashname: hashName,
                        Filehashlist: hashBytes}

	file, err := json.Marshal(mdj)
        md.checkError(err)
	md.println(",")
        md.print(string(file))
}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	var hashListString = strings.Join(hashList, ":")

	var blocksize = fmt.Sprintf("%v", blockSize)
	var modexp = fmt.Sprintf("%v", modExp)
	var modulus = fmt.Sprintf("%v", mod)

        mdj := jsonBlock{
			Mdtype: "Block",
                        Blocksize: blocksize,
                        Hashlist: hashListString,
                        Modexp:  modexp,
			Mod: modulus}

	block, err := json.Marshal(mdj)
        md.checkError(err)
	md.println(",")
	md.print(string(block))
	md.blockCount++

}


// this can encode the block as minimal simple, inform, xml
func (md *MdFormat) EncodeEndFile(encodingFormat int) {

	md.println("]}")
}

// md print
// this can either write to a file or print to standard out
// or write to a binary file or insert to sql db 
func (md *MdFormat) print(line  ...interface{}) {

        if md.outputFile != "" {
                fmt.Fprint(md.file, line...)
        } else {
                fmt.Print(line...); 
        }

}

// md println
// this can either write to a file or print to standard out
// or write to a binary file or insert to sql db 
func (md *MdFormat) println(line  ...interface{}) {

	if md.outputFile != "" {
		fmt.Fprint(md.file, line...)
		fmt.Fprint(md.file, "\n")
	} else {
		fmt.Print(line...);
		fmt.Print("\n")
	}

}


// test interface method
func (md *MdFormat) Println2(line  ...interface{}) {
        fmt.Print(line...);
        fmt.Print("\n")
}

// check error function
func (md *MdFormat) checkError(err error) {
        if err != nil {
                // panic("psql err: " + err.Error())
                fmt.Println("formatter err: ", err)
                os.Exit(3)
        }
}

// test inteface function
func (md *MdFormat) PrintFormatType() {
        fmt.Println("Format mdFormatJson")
}
