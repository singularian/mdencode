package mdFormatMD

// mdencode
// markdown formatter
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"bytes"
	"time"
	"strings"
	"os"
)


type MdFormat struct {
	commandArgs int
	fileName string
        filePath string
        fileSize uint64
        blockSize uint64
        blockCount uint64
        blockRemainder uint64
        modSize uint64
        modExponent uint64
        mdFormat int
        mdVersion string
	mdfileHashListString string
	mdblockHashListString string
	outputFile string
        err     error
	file *os.File 
	// byte buffer
	// optionally add an option -buffer boolean
	// when println is called and it is specified it writes to byte buffer
	// then add a method to output it as a string
	buffer bytes.Buffer
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
	md.blockCount = 0
	md.modSize = modulusSize
	md.mdVersion = version 
	// set the input hashlist string
	md.mdfileHashListString  = fileHashListString
	md.mdblockHashListString = blockHashListString
	// set the output file name
	md.outputFile = outputfileName

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
// Example:
// 522:2:64:16:md5:sha1:2017-09-14 20:51:00.671424081 -0400 EDT
//
// also could have a binary encoded signature format for efficiency in size
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) {

        var buffer bytes.Buffer

	// reset the block count
	md.blockCount = 0

	var filehashListString = strings.Join(filehashList, ":")
	var blockhashListString = strings.Join(blockhashList, ":")

	// var hashListString = strings.Join(blockhashList, ":")

	if filehashListString == "" { 
		filehashListString = "empty"
	}
	if blockhashListString == "" {
		blockhashListString = "empty"
	}

	// should have a version
	// add mdversion

	var mdhead     = fmt.Sprintf("# Filename\n%s\n\n# Filepath\n%s\n\n# File Header\n%v:%v:%v:%v:%s-%s:%v:%v\n\n", fileName, filePath, fileSize, encodingFormat, blockSize, modulusSize, filehashListString, blockhashListString, md.mdVersion, time.Now())
	var mdheadlist = fmt.Sprintf("- File Size %v\n- Ecoding Format %v\n- Block Size %v\n- Modulus Size %v\n- File Hash List %s\n- Block Hash List %s\n- Version %v\n- Time %v\n", fileSize, encodingFormat, blockSize, modulusSize, filehashListString, blockhashListString, md.mdVersion, time.Now())
	buffer.WriteString(mdhead)
	buffer.WriteString(mdheadlist)

        md.println(buffer.String())


}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

	var last  = fmt.Sprintf("# Filehash\n %v %v", hashName, hashBytes)

	md.println(last)

}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	var hashListString = strings.Join(hashList, ":")

	// the higher encoding formats are more verbose
	var mdb = fmt.Sprintf("# blockhash %v\n- %v bytes\n- Hash List %s\n- Modulus Exponent %d\n- Modulus %s\n", md.blockCount, blockSize, hashListString, modExp, mod)
	md.println(mdb)

	md.blockCount++
}


// this can encode the block as minimal simple, inform, xml
func (md *MdFormat) EncodeEndFile(encodingFormat int) {

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
        // fmt.Print(line...);
        // fmt.Print("\n")

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
        fmt.Println("Format mdFormatMD")

}

// check error function
func (md *MdFormat) checkError(err error) {
	if err != nil {
		// panic("psql err: " + err.Error())
		fmt.Println("formatter err: ", err)
		os.Exit(3)
	}
}

// check the error
func (md *MdFormat) checkError2() {
        if md.err != nil {
            panic("mdfile error " + md.err.Error())
        }

}
