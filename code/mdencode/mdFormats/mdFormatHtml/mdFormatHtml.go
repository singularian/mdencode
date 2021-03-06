package mdFormatHtml

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
// format: inform
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) {

        var buffer bytes.Buffer
	// var hashListString = strings.Join(blockhashList, ":")
	var filehashListString = strings.Join(filehashList, ":")
        var blockhashListString = strings.Join(blockhashList, ":")

	md.blockCount = 0

//	switch encodingFormat {
//	case 100:
                buffer.WriteString("\n")
                buffer.WriteString("<!DOCTYPE html>\n")
                buffer.WriteString("<html>\n")
                buffer.WriteString("<body>\n")
                buffer.WriteString("<head>\n")
                buffer.WriteString("<title>\n")
                buffer.WriteString(fileName)
                buffer.WriteString(" Signature</title>\n")
                buffer.WriteString("</head>\n")
                buffer.WriteString("<body>\n")
                //buffer.WriteString("<ul>")
                buffer.WriteString("<p>")
                //buffer.WriteString("<li>")

                // buffer.WriteString("\n")
                buffer.WriteString(filePath)
		buffer.WriteString("</p>\n<p>")
		buffer.WriteString(fileName)
                // buffer.WriteString("[filepath]")
                // buffer.WriteString("[filesize]")
                buffer.WriteString(fmt.Sprintf(" %v ", fileSize))
                // buffer.WriteString("[filesize]")
                // buffer.WriteString("[format]")
                buffer.WriteString(fmt.Sprintf("%v ", encodingFormat))
                // buffer.WriteString("[format]")
                // buffer.WriteString("[blocksize]")
                buffer.WriteString(fmt.Sprintf("%v ", blockSize))
               // buffer.WriteString("[blocksize]")
                //buffer.WriteString("[modulussize]")
                buffer.WriteString(fmt.Sprintf("%v ", modulusSize))
                //buffer.WriteString("[modulussize]")
                //buffer.WriteString("[filehashlist]")
                buffer.WriteString(filehashListString)
		//buffer.WriteString("[filehashlist]")
		//buffer.WriteString("[blockhashlist]")
                buffer.WriteString(blockhashListString)
		//buffer.WriteString("[blockhashlist]")
                //buffer.WriteString("[hashlist]")
                //buffer.WriteString("[version]")
		buffer.WriteString(md.mdVersion) 
		//buffer.WriteString("[version]")
                // add the time
                //buffer.WriteString("[time]")
                buffer.WriteString(fmt.Sprintf("%v", time.Now()))
                buffer.WriteString("</p>")
                //buffer.WriteString("[time][hd]")
//	}

        md.println(buffer.String())


}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

	md.println("<p>", hashName, " ", hashBytes, "</p>")

}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	var hashListString = strings.Join(hashList, ":")

	md.print("<p>", blockSize)
	md.print(" ", md.blockCount, " ")
        md.print(" ", hashListString);
        md.print(" ", modExp);
        md.print(" ", mod, "</p>");

	md.print("\n")
	md.blockCount++
}


// this can encode the block as minimal simple, inform, xml
func (md *MdFormat) EncodeEndFile(encodingFormat int) {

        //switch encodingFormat {
	//case 100:
		md.println("</body>")
		md.println("</html>")
	//}
}

// md print
// this can either write to a file or print to standard out
// or write to a binary file or insert to sql db 
func (md *MdFormat) print(line  ...interface{}) {

        if md.outputFile != "" {
                fmt.Fprint(md.file, line...)
        } else {
                fmt.Print(line...) 
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
		fmt.Print(line...)
		fmt.Print("\n")
	}

}


// test interface method
func (md *MdFormat) Println2(line  ...interface{}) {
        fmt.Print(line...)
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
        fmt.Println("Format mdFormatXML")
}
