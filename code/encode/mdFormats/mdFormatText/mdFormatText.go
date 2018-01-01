package mdFormatText

// mdencode
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
// it contains the file size in bytes, format type, block signature size, modulus bit size and signatures as well as a potential time attribute
// 522:2:64:16:md5:sha1:2017-09-14 20:51:00.671424081 -0400 EDT
//
// also could have a binary encoded signature format for efficiency in size
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) {

        var buffer bytes.Buffer
	buffer.WriteString(fileName)

	buffer.WriteString("\n")
	buffer.WriteString(filePath)

	var filehashListString = strings.Join(filehashList, ":")
	var blockhashListString = strings.Join(blockhashList, ":")

	var hashListString = strings.Join(blockhashList, ":")

	if filehashListString == "" { 
		filehashListString = "empty"
	}
	if blockhashListString == "" {
		blockhashListString = "empty"
	}

	// should have a version
	// ad mdversion

	switch encodingFormat {
	case 0:
		buffer.WriteString("\n")
		buffer.WriteString(fmt.Sprintf("%v", fileSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", encodingFormat))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", blockSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", modulusSize))
                buffer.WriteString(":")
                buffer.WriteString(hashListString)
	case 4:
                buffer.WriteString("\n")
                buffer.WriteString(fmt.Sprintf("%v", fileSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", encodingFormat))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", blockSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", modulusSize))
                buffer.WriteString(":")
                buffer.WriteString(hashListString)
                buffer.WriteString(":")
                // add the time
                buffer.WriteString(fmt.Sprintf("%v", time.Now()))
	case 5:
                buffer.WriteString("\n")
                buffer.WriteString(fmt.Sprintf("%v", fileSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", encodingFormat))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", blockSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", modulusSize))
                buffer.WriteString(":")
                buffer.WriteString(hashListString)
                buffer.WriteString(":")
                // add the time
                buffer.WriteString(fmt.Sprintf("%v", time.Now()))
	case 7:
                buffer.WriteString("\n")
                buffer.WriteString(fmt.Sprintf("%v", fileSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", encodingFormat))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", blockSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", modulusSize))
                buffer.WriteString(":")
	        buffer.WriteString(fmt.Sprintf("%v", md.mdfileHashListString))
		buffer.WriteString("-")
		buffer.WriteString(fmt.Sprintf("%v", md.mdblockHashListString))
                buffer.WriteString(":")
                // add the time
                buffer.WriteString(fmt.Sprintf("%v", time.Now()))
	default:
		encodingFormat = 8 

                buffer.WriteString("\n")
                buffer.WriteString(fmt.Sprintf("%v", fileSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", encodingFormat))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", blockSize))
                buffer.WriteString(":")
                buffer.WriteString(fmt.Sprintf("%v", modulusSize))
                buffer.WriteString(":")
		buffer.WriteString(filehashListString)
		buffer.WriteString("-")
		buffer.WriteString(blockhashListString)
		buffer.WriteString(":")
		buffer.WriteString(md.mdVersion)
		buffer.WriteString(":")
		buffer.WriteString(fmt.Sprintf("%v", time.Now()))
	}

        md.println(buffer.String())


}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

        switch encodingFormat {
	case 0:
		md.println("z: ", hashName, " ", hashBytes)
	case 1:
		md.println("z: ", hashName, " ", hashBytes)
	case 2:
		md.println("z: ", hashName, " ", hashBytes)
	case 3:
		md.println("z: ", hashName, " ", hashBytes)
	case 4: 
		md.println("z: ", hashName, " ", hashBytes)
        case 5:
		md.println("z: ", hashName, " ", hashBytes)
	default:
		md.println("filehash: ", hashName, " ", hashBytes)
	}

}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	var hashListString = strings.Join(hashList, ":")

	// the higher encoding formats are more verbose
        switch encodingFormat {
	// minimal format
        case 0:
                hashListString = strings.Join(hashList, "")
                md.print(hashListString);
                md.print(modExp);
                md.print(mod);
                // fmt.Print(":", logN(z, i))
	case 1:
                hashListString = strings.Join(hashList, "")
                md.print(hashListString);
                md.print(modExp);
                md.print(":", mod);
                // fmt.Print(":", logN(z, i))
	case 2:
                md.print(hashListString);
                md.print(modExp);
                md.print(":", mod);
                // fmt.Print(":", logN(z, i))
	case 3:
                md.print(blockSize, ":")
                md.print(hashListString);
                md.print(":", modExp);
                md.print(":", mod);
		// fmt.Print(":", logN(z, i))
	case 4:
	        md.print(blockSize, ":")
                md.print(hashListString);
                md.print(":", modExp);
                md.print(":", mod);
                // need to pass this in or something
                // output the block bigint
                // fmt.Print(":", mod);
        case 5:
                md.print(blockSize, ":")
                md.print(hashListString);
                md.print(":", modExp);
                md.print(":", mod);
                // need to pass this in or something
                // output the block bigint
                // fmt.Print(":", mod);
	case 6:
                md.print(blockSize, ":")
                md.print(hashListString);
                md.print(":", modExp);
                md.print(":", mod);
                md.print(":COLLISION");
                // need to pass this in or something
                // output the block bigint
                // fmt.Print(":", mod);
	default:
                md.print(blockSize, ":")
                md.print(hashListString);
                md.print(":", modExp);
                md.print(":", mod);
	}

	md.print("\n")
}


// this can encode the block as minimal simple, inform, xml
func (md *MdFormat) EncodeEndFile(encodingFormat int) {

/*        switch encodingFormat {
	// used to be case 6
	case 9:
		md.println("[md]")
	case 100:
		md.println("</md>")
	}
*/
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
        fmt.Println("Format mdFormatText")

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
