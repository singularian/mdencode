package mdFormatImg

// mdencode
// signature file image png formatter
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"bytes"
	// "time"
	"strings"
	"os"
	"github.com/fogleman/gg"
	"encoding/hex"
	"math/big"
	"encoding/binary"
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
	// image contest
	gg *gg.Context
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
	const W = 1024
        const H = 1024

	md.gg = gg.NewContext(W, H)
	md.gg.SetRGB(0, 0, 0)
	md.gg.Clear()

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

	md.fileName = fileName
        // var buffer bytes.Buffer

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

	//var mdhead     = fmt.Sprintf("# Filename\n%s\n\n# Filepath\n%s\n\n# File Header\n%v:%v:%v:%v:%s-%s:%v:%v\n\n", fileName, filePath, fileSize, encodingFormat, blockSize, modulusSize, filehashListString, blockhashListString, md.mdVersion, time.Now())
	//var mdheadlist = fmt.Sprintf("- File Size %v bytes\n- Ecoding Format %v\n- Block Size %v bytes\n- Modulus Size %v bits\n- File Hash List %s\n- Block Hash List %s\n- Version %v\n- Time %v\n", fileSize, encodingFormat, blockSize, modulusSize, filehashListString, blockhashListString, md.mdVersion, time.Now())
	//buffer.WriteString(mdhead)
	//buffer.WriteString(mdheadlist)

        // md.println(buffer.String())


}

// print and generate the file signature
// this is the entire file signature hash type and signature hex bytes
func (md *MdFormat) EncodeFileHash(encodingFormat int, hashName string, hashBytes string) {

	src := []byte(hashBytes)
	dst := make([]byte, hex.DecodedLen(len(src)))
	_, err := hex.Decode(dst, src)
	if err != nil {
                fmt.Println(err)
        }

        var i uint64 = 0
        for i = 0; i < uint64(len(dst) - 1); i++ {
                var color = int(dst[i])
		 md.gg.SetRGB255(color, int(i), int(dst[i+1]))

                var x = float64(dst[i]) * 4
                var y = float64(dst[i+1]) * 4
                var width = float64((dst[i] % 64) + 1)
                md.gg.DrawCircle(x, y, width)
                md.gg.FillPreserve()
                md.gg.Stroke()

	}


}


// this can encode the block as minimal simple, inform, xml
// this could also add a collision number byte or 4 byte 32-bit integer to specify which collsion is the correct hash block
func (md *MdFormat) EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string) {

	// convert modulus bigint to hex
	modulusBigInt := new(big.Int)
	modulusBigInt, ok := modulusBigInt.SetString(mod, 16)
	if !ok {
            fmt.Println("SetString: error")
            return
	}

	// convert modulus exponent int to hex
	modexpbytes := make([]byte, 4)
	binary.LittleEndian.PutUint32(modexpbytes, uint32(modExp))

	// concatenate the hex hashlist and the hex modulus bigint bytes and the hex of the mod exponent bytes
	var hashListString = strings.Join(hashList, "00")
	hashListString     = hashListString + fmt.Sprintf("%x", modulusBigInt.Bytes()) + fmt.Sprintf("%x", modexpbytes)

        src := []byte(hashListString)

	// var hashListStringSize = len(src)

        dst := make([]byte, hex.DecodedLen(len(src)))
        // n, err := hex.Decode(dst, src)
        _, err := hex.Decode(dst, src)
        if err != nil {
        //      log.Fatal(err)
                fmt.Println(err)
        }

	const W = 1024
        const H = 1024

	var i uint64 = 0
	for i = 0; i < uint64(len(dst) - 1); i++ {
		var color = int(dst[i])
		md.gg.SetRGB255(color, int(i), int(dst[i+1]))
		var x = float64(dst[i]) * 4
		var y = float64(dst[i+1]) * 4

		var width = float64(3)
		if encodingFormat == 8001 {
			width = float64((dst[i] % 64) + 1)
		}

		md.gg.DrawCircle(x, y, width)
		md.gg.FillPreserve()
		md.gg.Stroke()

	}


	md.blockCount++
}


// this can encode the block as minimal simple, inform, xml
func (md *MdFormat) EncodeEndFile(encodingFormat int) {
	var outputfile = md.outputFile

	if outputfile == "" {
		outputfile = md.fileName + ".png"
	}
	md.gg.SavePNG(outputfile)
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
        fmt.Println("Format mdFormatIMG")

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
