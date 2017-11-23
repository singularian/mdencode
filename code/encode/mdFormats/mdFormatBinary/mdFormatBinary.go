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
// it contains the file size in bytes, format type, block signature size, modulus bit size and signatures as well as a potential time attribute
// 522:2:64:16:md5:sha1:2017-09-14 20:51:00.671424081 -0400 EDT
//
// also could have a binary encoded signature format for efficiency in size
func (md *MdFormat) EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) {

//	var number uint64 = 10102938
/*      err = binary.Write(f, binary.LittleEndian, number)
        if err != nil {
                log.Fatalln(err)
        }
*/
	var length int = len(fileName)
	fmt.Println("filename length ", length)
	var number uint32 = uint32(length)
	fmt.Println("filename int ", number)
	// number = 9
	// var err = binary.Write(md.file, binary.LittleEndian, number)
	var err = binary.Write(md.file, binary.BigEndian, &number)
        if err != nil {
                fmt.Println("bad argument filname length ", err)
        }

	// write the filename
	var fn = []byte(fileName)
	// fmt.Println("test ", fn)
	err = binary.Write(md.file, binary.LittleEndian, fn)
	if err != nil {
                fmt.Println("bad argument filename ", err)
        }

	// write the filepath uint32 size
	length  = len(filePath)
	number  = uint32(length)
	err = binary.Write(md.file, binary.BigEndian, &number)

	// write the filePathBytes
	var filePathBytes = []byte(filePath)
	err = binary.Write(md.file, binary.LittleEndian, filePathBytes)
        if err != nil {
                fmt.Println("bad argument path ", err)
        }

	// write the hashlist size as a uint32 
	/*var hashListString = strings.Join(hashList, ":")
	length  = len(hashListString)
	number  = uint32(length)
	err = binary.Write(md.file, binary.BigEndian, &number)
        if err != nil {
                fmt.Println("bad argument ", err)
        }*/
	var hashListString = md.mdfileHashListString + "-" + md.mdblockHashListString
	// mdata.mdfileHashListString
        // mdata.mdblockHashListString
	length  = len(hashListString)
        number  = uint32(length)
        err = binary.Write(md.file, binary.BigEndian, &number)
        if err != nil {
                fmt.Println("bad argument hashlist ", err)
        }

	// write the hashListString
	var fh = []byte(hashListString)
	err = binary.Write(md.file, binary.BigEndian, fh)

	err = binary.Write(md.file, binary.BigEndian, &fileSize)
	err = binary.Write(md.file, binary.BigEndian, &blockSize)

	err = binary.Write(md.file, binary.BigEndian, &modulusSize)


}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

	b, _ := hex.DecodeString(hashBytes)
	_ = binary.Write(md.file, binary.BigEndian, b)

}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	var hashListString = strings.Join(hashList, "")
	b, _ := hex.DecodeString(hashListString)
	_ = binary.Write(md.file, binary.BigEndian, b)

	// write the modulus exponent
        var number  = uint32(modExp)
        // err = binary.Write(md.file, binary.BigEndian, &number)
	_ = binary.Write(md.file, binary.BigEndian, &number)

	// convert modulus to bigint bytes
	n := new(big.Int)
	// n, ok := n.SetString(mod, 10)
	n, _ = n.SetString(mod, 10)

	modbytes := n.Bytes()
	_ = binary.Write(md.file, binary.BigEndian, &modbytes)
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
        fmt.Print(line...);
        fmt.Print("\n")
}

// test inteface function
func (md *MdFormat) PrintFormatType() {
        fmt.Println("Format mdFormatBinary")
}
