package mdFormatImport

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE


import (
	"hash"
	"fmt"
	"log"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatCSV"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatText"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatInform"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatHtml"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatXML"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatXMLgo"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatJson"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatBinary"
	// "github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatSQL"

)

// mdformat interface struct
type mdformat interface {
	PrintFormatType()
	OpenFile(appendfile bool)
	InitFile()
	EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) 
	EncodeFileHash(encodingFormat int, hashName string, hashBytes string)
	EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string)
	EncodeEndFile(encodingFormat int)
}

// mdencode struct
type FileData struct {
	commandArgs int
	fileName string
	filePath string
	outputFileName string
	fileSize uint64
	blockSize uint64
	blockCount uint64
	blockRemainder uint64
	modSize uint64
	modExponent uint64
	mdFormat int
	mdVersion string
	// append file
        appendfile bool
	// byte block
	byteblock bool
	// filehashline
	filehashline bool
	// key string
	key string
	// logfile
	logfile string
	// argument hash list bits
	fileHashListString string
	blockHashListString string
	// argument hash list bit arrays 
        fileHashListArray     []string
        blockHashListArray    []string
	// argument signature hash list names
	fileHashListNames  []string
	blockHashListNames []string
	// ===========================================
	// formatter args
	// md hash list bits
        mdfileHashListBitString string
        mdblockHashListBitString string
        // md hash list string
        mdfileHashListString string
        mdblockHashListString string
        mdHashlist string
        // output file variables
        ////outputFileName string // not needed
	// ===========================================
	// dictionary
	dictionary map[string]string
	// hash list for files
	hashList  map[string]hash.Hash
	// hash list for blocks
	hashListBlocks  map[string]hash.Hash
	// formatter
	// md *mdFormatText.MdFormat
	// mdformat interface for modular type assignment
	mdfmt mdformat
	// log writer
	log *log.Logger
}


var version string = "1.0"

// Init returns a new mdFormat loader object
// this sets the correct mdFormat object
func Init(encodingFormat int, fileName string, filePath string, fileSize uint64, blockSize uint64, modulusSize uint64, fileHashListString string, blockHashListString string, outputfileName string) (mdimport *FileData) {

        md := new(FileData)
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
        md.outputFileName = outputfileName
	// other initializers
	md.appendfile = false
	md.byteblock  = false
	md.filehashline = false
	md.logfile = ""

	// fmt.Println("init import md file ", md.fileName)
        // fmt.Println("init import output file ", md.outputFileName)

        return md
}

// setmdFormat 
// sets the correct md format object 
func (md *FileData) SetmdFormatText (filehashline bool) (mdfmt mdformat) {

        // md.mdfmt = mdFormatText.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
        // md.mdfmt.OpenFile(md.appendfile)
	// md.mdfmt.InitFile()
	mdf := mdFormatText.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
	mdf.OpenFile(md.appendfile)
	mdf.InitFile()

	// return md.mdfmt
	return mdf
}

// setmdFormatNoSQL 
// sets the correct md format object 
func (md *FileData) SetmdFormatNoSQL (filehashline bool) (mdfmt mdformat) {

        var format = md.mdFormat

	// fmt.Println("md file ", md.fileName)
	// fmt.Println("output file ", md.outputFileName)

        /* if format == 100 {
                mdf := mdFormatXML.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
		mdf.InitFile()
		return mdf
        } else if format == 99 {
                mdf := mdFormatInform.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
                mdf.InitFile()
                return mdf
	*/
	// CSV
        if format == 101 || format == 102 {
                mdf := mdFormatCSV.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
		mdf.InitFile()
		return mdf
        /* } else if format == 600 {
                mdf := mdFormatJson.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
		mdf.InitFile()
		return mdf
	*/
	// Binary
        } else if format == 1000 {
                // l.mdfmt = mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
                if md.outputFileName == "" {
                        md.outputFileName = "default.mdbin"
                }
                mdf := mdFormatBinary.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(false)
                mdf.InitFile()
                return mdf
/*      } else if format == 2000 {
                // set a default filename if none is provided
                if md.outputFileName == "" {
                        md.outputFileName = "default"
                }
                md.outputFileName = md.outputFileName + ".db"
                mdf := mdFormatSQL.Init(101, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.InitFile()
                return mdf
*/
        // Inform
        } else if format == 3000 {
                mdf := mdFormatInform.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
                mdf.InitFile()
                return mdf
        // JSON
        } else if format == 4000 {
                mdf := mdFormatJson.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
                mdf.InitFile()
                return mdf
        // HTML Formatter
        } else if format == 4500 {
                mdf := mdFormatHtml.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
		return mdf
        // XML
        } else if format == 5000 {
                mdf := mdFormatXMLgo.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
                mdf.InitFile()
                return mdf
	// XML Non go
        } else if format == 5001 {
                mdf := mdFormatXML.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
                mdf.InitFile()
                return mdf
        } else {
                mdf := mdFormatText.Init(format, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
		mdf.InitFile()
		return mdf
        }

        // md.mdfmt.InitFile()
        // mdf.InitFile()

        // return mdf

}

// setmdFormatAll 
// sets the correct md format object 
/*
func (md *FileData) SetmdFormatAll (filehashline bool) (mdfmt mdformat) {

	var format = md.mdFormat

	if format == 100 {
                mdf := mdFormatXML.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
	} else if format == 99 {
		mdf := mdFormatInform.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
		mdf.OpenFile(md.appendfile)
	} else if format == 101 || format == 102 {
		mdf := mdFormatCSV.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
		mdf.OpenFile(md.appendfile)
	} else if format == 600 {
		mdf := mdFormatJson.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
                mdf.OpenFile(md.appendfile)
	} else if format == 1000 {
		// l.mdfmt = mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		if md.outputFileName == "" {
			md.outputFileName = "default.mdbin"
		}
		mdf := mdFormatBinary.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
		mdf.OpenFile(false)
		mdf.InitFile()
		return mdf
        } else {
		mdf := mdFormatText.Init(md.mdFormat, md.fileName, md.filePath, md.fileSize, md.blockSize, md.modSize, md.fileHashListString, md.blockHashListString, md.outputFileName)
		mdf.OpenFile(md.appendfile)
        }

	// md.mdfmt.InitFile()
	var mdf
	return mdf
}
*/

// SetByteBlock
// set the byte block mode
func (md *FileData) SetByteBlock (byteblock bool ) {
	md.byteblock = byteblock
}

// SetAppendFile
// set the append file mode
func (md *FileData) SetAppendFile (appendfile bool) {
        md.appendfile = appendfile 
}

// SetFileHashLine
// set the file hash format 
// if true it will write or display the file hash list as one line  
func (md *FileData) SetFileHashLine (filehashline bool) {
        md.filehashline = filehashline 
}

// SetKeyFile
// set the md key
// this defaults to a default key if the key is less than 16 bytes
// one of the hash libs faults if the key is to small 
// some of the signatures use a key
func (md *FileData) SetKeyFile (key string) {
        md.key = key
}

// SetLogFile
// set the optional logfile
func (md *FileData) SetLogFile (logfile string) {
        md.logfile = logfile 
}

// display the object type
func (md *FileData) PrintType() {
	fmt.Println("mdFormatImport")

}
