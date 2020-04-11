package mdFormatBinary

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"encoding/binary"
	"encoding/hex"
	"strings"
	"math/big"
	"log"
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
	// hashlist bit strings
        mdfileHashListString string
        mdblockHashListString string
	// output file
	outputFile string
	file *os.File 
}

var version string = "1.0"

// Init returns a new mdFormat object
//  mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
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

	// fmt.Println("file Hashlist size init ", encodingFormat, fileName, filePath, fileSize, blockSize, modulusSize, fileHashListString, " ", blockHashListString )

        // set the output file name
	md.outputFile = outputfileName

	return md
}

func (md *MdFormat) OpenFile(appendfile bool) {

	var outputfileName = md.outputFile

	if md.outputFile != "" { 
                file, err := os.Create(outputfileName)
                if err != nil {
                        log.Fatal("Cannot create file", err)
                        // log.Fatal("Cannot create file", err)
                        os.Exit(3)
                }
                defer md.file.Close()
                md.file = file
        }

}

// format init file methods
func (md *MdFormat) InitFile() {

}


// generate the file signature header
// it contains the file md format type, filename, filepath, filesize, block signature size, file hash signature list, block hash signature list and modulus bit size 
// it can also add time attribute
//
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, keylist string, modulusSize int64) {

	var fileAttribute [6]int64
	fileAttribute[0] = fileSize
        fileAttribute[1] = blockSize
        fileAttribute[2] = modulusSize
	md.modSize = uint64(modulusSize)

	// set filename length
	var length int = len(fileName)
	fileAttribute[3] = int64(length)
	// fmt.Println("filename length ", length, " ", fileAttribute[3])

	// set the filepath uint64 size
	// filepath length
	length  = len(filePath)
	fileAttribute[4]  = int64(length)

	// hashlist size
	// filehashlist:blockhashlist:signaturekeylist
	var hashListString = strings.Join(filehashList, ":") + "-" + strings.Join(blockhashList, ":")

	// if the keylist is specified add it
	if keylist != "" {
		hashListString += "-"
		hashListString += keylist
	}


	// calculate the hashlist string size
        length  = len(hashListString)
        fileAttribute[5]  = int64(length)
        // fmt.Println("file Hashlist size ", length, " ", hashListString, " attribute 5 ", fileAttribute)

	// write the fileattributes to the binary file
	err := binary.Write(md.file, binary.BigEndian, &fileAttribute)
        if err != nil {
                fmt.Println("bad argument ", err)
        }

        // write the filePathBytes
        var filePathBytes = []byte(filePath)
        err = binary.Write(md.file, binary.BigEndian, filePathBytes)
        if err != nil {
                fmt.Println("bad argument path ", err)
        }

        // write the filename
        var fn = []byte(fileName)
        // fmt.Println("test ", fn)
        err = binary.Write(md.file, binary.BigEndian, fn)
        if err != nil {
                fmt.Println("bad argument filename ", err)
        }


        // write the hashListString
        var fh = []byte(hashListString)
        err = binary.Write(md.file, binary.BigEndian, fh)

}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

	b, _ := hex.DecodeString(hashBytes)
	_ = binary.Write(md.file, binary.BigEndian, b)

}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
// func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, modremainder *big.Int) {


	var hashListString = strings.Join(hashList, "")
	b, _ := hex.DecodeString(hashListString)
	_ = binary.Write(md.file, binary.BigEndian, b)

	// write the modulus exponent
	// if the block size is less than or equal to 32 bytes use a single byte to encode the modulus exponent
	// 2 ^ 256 = 32 bytes = 32 * 8
	if md.blockSize <= 32 {
		var number  = byte(modExp)
		// err = binary.Write(md.file, binary.BigEndian, &number)
		_ = binary.Write(md.file, binary.BigEndian, &number)
	// if the block size is greater than 32 bytes use a uint32 to encode the modulus exponent
	} else {
		var number  = uint32(modExp)
		// err = binary.Write(md.file, binary.BigEndian, &number)
		_ = binary.Write(md.file, binary.BigEndian, &number)
	}

        // this makes sure the modulus bytes are the same size as the modulus bitsize in bytes                                                                                                          
        // so it doesn't get an overflow error on different byte slice sizes 	
	var modByteSize uint64 = 0
	if md.modSize % 8 == 0 {
		modByteSize = md.modSize / 8
	} else if md.modSize <= 8 {
		modByteSize = 1;
	} else {
		modByteSize = (md.modSize / 8) + 1
	}

	padBlock := make([]byte, modByteSize) 
	var start = modByteSize - uint64(len(modremainder.Bytes())) 
        var end = modByteSize 
	copy(padBlock[start:end], modremainder.Bytes())


	// fmt.Println("testing line ", modremainder.Bytes(), bs, modByteSize, " start end ", start, end, " modsize ", md.modSize)
	// fmt.Println("modremainder ", modremainder.Bytes(), bs)
	 _ = binary.Write(md.file, binary.BigEndian, padBlock)
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
		fmt.Print(line...)
		fmt.Print("\n")
	}

}


// test interface method
func (md *MdFormat) Println2(line  ...interface{}) {
        fmt.Print(line...)
        fmt.Print("\n")
}

// test inteface function
func (md *MdFormat) PrintFormatType() {
        fmt.Println("Format mdFormatBinary")
}
