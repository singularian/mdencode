// Use of this source code is governed by a license
// that can be found in the LICENSE file

package main

import (
	"hash"
	"fmt"
	"strconv"
	"log"
	"os"
	"github.com/singularian/mdencode/code/encode/mdReports/mdReportsSQL"
)

// mdformat interface struct
type mdformat interface {
        Println()
        OpenFile()
        EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) 
        EncodeFileHash(encodingFormat int, hashName string, hashBytes string)
        EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string)
        EncodeEndFile(encodingFormat int)
}

// mdencode struct
type fileData struct {
	commandArgs    int
	fileName       string
	filePath       string
	fileSize       uint64
	blockSize      uint64
	blockCount     uint64
	blockRemainder uint64
	modSize        uint64
	modExponent    uint64
	mdFormat       int
	mdVersion      string
        // argument hash list bits
        fileHashListString string
        blockHashListString string
	// argument hash list bits
	fileHashListArray  []string
	blockHashListArray []string
	// argument signature hash list
	fileHashListNames  []string
	blockHashListNames []string
	// dictionary
	dictionary map[string]string
	// hash list for files
	hashList map[string]hash.Hash
	// hash list for blocks
	hashListBlocks map[string]hash.Hash
	// md formatter
	// mdx *mdFormatText.MdFormat
	// md *mdFormatSQL.MdFormat
	mdfmt mdformat
	// log writer
	log *log.Logger
}

var byteblock = 0

func main() {

	// var hashlist string = "01100000000"

	var argsNumber int = len(os.Args)
	argsReport(argsNumber)

	os.Exit(0)

}


func argsReport(argsNumber int) {

	// var outfilename = ""
	var defaultFormat = 9 

	// if just the file is given display the file mdencode list
	// make 0 display everything
	if argsNumber == 2 {
		filename := os.Args[1]
		if _, err := os.Stat(filename); os.IsNotExist(err) {
			log.Fatal("The mdfile db does not exist")
			os.Exit(3)
		}
		// fdata := fileData{}
		//fdata.md = mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		md := mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		md.PrintFileList(1, 0)

	// mdencodereport filename fileid
	} else if argsNumber == 3 {
		filename := os.Args[1]
		fileid := os.Args[2]
		fileInt, _ := strconv.ParseUint(fileid, 10, 64)

		if _, err := os.Stat(filename); os.IsNotExist(err) {
			log.Fatal("The mdfile db does not exist")
			os.Exit(3)
		}

		// fdata := fileData{}
		// fdata.md = mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		md := mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		if fileid != "0" {
			// fdata.md.PrintReport(defaultFormat, "filename", fileInt)
			md.PrintReport(defaultFormat, "filename", fileInt)
			// display the entire file list
		} else {
			// fdata.md.PrintEntireFileList(defaultFormat)
			md.PrintEntireFileList(defaultFormat)
	}	
	// mdencode filename fileid format
	} else if argsNumber == 4 {
		filename := os.Args[1]
		fileid := os.Args[2]
		format := os.Args[3]
		fileInt, _ := strconv.ParseUint(fileid, 10, 64)

		// fdata := fileData{}
		formatInt, _ := strconv.Atoi(format)
		/// fdata.md = mdReportSQL.Init(formatInt, filename, "", 0, 0, 0, "0", "0", filename)
		////fdata.md.PrintReport(formatInt, "filename", fileInt)
		md := mdReportSQL.Init(formatInt, filename, "", 0, 0, 0, "0", "0", filename)
		md.PrintReport(formatInt, "filename", fileInt)


		// fdata.md.EncodeEndFile(formatInt)

	} else {
		// display the file list
		fmt.Println("\nUsage: mdencodereport [filename]")
		// dump everything from the db in the default format
		fmt.Println("Usage: mdencodereport [filename] 0")
		// display the mdfile with a db mdfileid number
		fmt.Println("Usage: mdencodereport [filename] [fileid]")
		fmt.Println("Usage: mdencodereport [filename] [fileid] [format]")
	}

}

