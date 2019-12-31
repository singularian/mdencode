package main

import (
	"fmt"
	"encoding/binary"
	"os"
	"bufio"
	"bytes"
	"strings"
	"math/big"
	"strconv"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
	"github.com/singularian/mdencode/code/mdencode/mdFormatsImport/mdFormatImport"
)


func main() {

	var filename = "default.mdbin"

	var argsNum int = len(os.Args)
	var format int = 1
	var fileName   = ""
	var outputFile = ""

	if argsNum == 2 {
		format, _ = strconv.Atoi(os.Args[1])
	} else if (argsNum == 3) {
		format, _ = strconv.Atoi(os.Args[1])
		filename  = os.Args[2]
	} else if (argsNum == 4) {
		format, _ = strconv.Atoi(os.Args[1])
		filename  = os.Args[2]
		outputFile = os.Args[3]
	} else {
		var exe = os.Args[0]
		fmt.Printf("USAGE of %s:\n", os.Args[0])
		fmt.Println("Examples:")
		fmt.Println(exe, " [FORMAT] ")
		// doesn't have an output file
		fmt.Println(exe, " [FORMAT] [FILENAME]")
		fmt.Println(exe, " [FORMAT] [FILENAME] [OUTPUTFILE]")
		fmt.Println(exe, " 4000 (opens default.mdbin)")
		fmt.Println(exe, " 4000 ")
		fmt.Println(exe, " 4000 mdbintest.md")
		os.Exit(0)
	}

	file, err := os.Open(filename)
        if err != nil {
                fmt.Println("counldn't open file ", err)
                os.Exit(1)
        }
        defer file.Close()

        // mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", md.outputFile)
        mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", outputFile)
        // mdload.SetAppendFile(md.appendfile)
        mdload.SetAppendFile(false)
        mdload.SetOutputFile(outputFile)
        mdfmt := mdload.SetmdFormatNoSQL(true)

	stats, _ := file.Stat()
	var size int64 = stats.Size()
	bytes := make([]byte, size)

	bufr := bufio.NewReader(file)
	_,_ = bufr.Read(bytes)

	/////////// fmt.Println("buf ", bytes)
	fileSize      := binary.BigEndian.Uint64(bytes[0:8])
	blockSize     := binary.BigEndian.Uint64(bytes[8:16])
	modSize       := binary.BigEndian.Uint64(bytes[16:24])
	fileNameLen   := binary.BigEndian.Uint64(bytes[24:32])
	filePathLen   := binary.BigEndian.Uint64(bytes[32:40])
	fileHashLen   := binary.BigEndian.Uint64(bytes[40:48])

	// need to add the version

	/* fmt.Println("fileSize ", fileSize)
	fmt.Println("blockSize ", blockSize)
	fmt.Println("modSize ", modSize)
	fmt.Println("fileNameLength ", fileNameLen)
	fmt.Println("filePathLength ", filePathLen)
	fmt.Println("fileHashLength ", fileHashLen)
	*/

	// get the filename
	var start uint64
	var end   uint64
	// start = filePathLen 
	// end = start + fileNameLen
	// fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	start = 48 
	end = start + filePathLen
	var filepath = string(bytes[start:end])
	// fmt.Println("filePath ", string(bytes[start:end]), start, end)
	// fileName
        start = end 
        end = start + fileNameLen
        // fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	var filename2 = string(bytes[start:end])
	// get hashlist string
	start = end
	end = end + fileHashLen
	// fmt.Println("hashlistname ", string(bytes[start:end]))
	// split hash list
	hlist := string(bytes[start:end])
	hashlist := strings.Split(hlist, "-")
	filelist := hashlist[0]
	blocklist := hashlist[1]
	// fmt.Println("hashlist ", filelist, blocklist)

	fl :=  strings.Split(hashlist[0], "-")
	bl :=  strings.Split(hashlist[1], "-")

	// initialize the mdBlockSize object
	mdBlock := mdBinaryList.Init()

	// mdfmt.EncodeFileHeader(format2, filename, filepath, int64(fileSize), int64(blockSize), filelist, blocklist, modSize)
	mdfmt.EncodeFileHeader(format, filename2, filepath, int64(fileSize), int64(blockSize), fl, bl, int64(modSize))

	// calculate and return the file signature list byte block size
	// var filesize uint64
	var filelistarr []int
	_, filelistarr = mdBlock.CalcHashBlockSize(filelist)

	// fmt.Println("test array ", filelistarr)
	st := strings.Split(filelist, ":")
	// get the file hash list
	for i:= 0; i < len(filelistarr); i++ {
		start = end
		end = end + uint64(filelistarr[i])
		var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		// filePathLen   := binary.BigEndian.(bytes[:40])
		// fmt.Println("hashlistname ", st[i], " hex ", hexstring)
		// fmt.Println("hashlistname ", st[i], " hex ", hexstring)
		mdfmt.EncodeFileHash(format, st[i], hexstring)
	}

	// check if the blocklist is specified
	// so it doesn't try to check the blocks if they don't exist
	if blocklist == "" {
		os.Exit(0)
	}

	// calculate and return the file signature block byte size list
	// mdblocksize, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)
	_, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)

	// calculate the file blocks count and the last file block size
	// ie if the file size is 100 and block size is 12 calculate the number of blocks and
	// remainder
	// 100 byte file contains 9 blocks with the last block 4 bytes
	blocks, remainder := calculateFileBlocks(fileSize, blockSize)

	var i uint64
	var modByteSize uint64 
	modByteSize = modSize / 8
	if modByteSize == 0 {
		modByteSize = 1 
	}
	// var hlistarray = strings.Split(hashhex, ":")
	var hlistarray []string
	// fmt.Println("mod byte size ", modSize, " ", modByteSize)
	for i = 0; i < blocks; i++ {
		start = end
		// end = end + fileblocksize + modByteSize + 4;
		// end = end + fileblocksize;
		// var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		for i:= 0; i < len(blocklistarr); i++ {
			start = end
			end = end + uint64(blocklistarr[i])
			hlistarray = append(hlistarray, fmt.Sprintf("%x", string(bytes[start:end])))
		}
		// should make modexp an int16
		start = end
		end = end + 4 
		modSize       := binary.BigEndian.Uint32(bytes[start:end])
		start = end
		end = end + modByteSize
		n := new(big.Int)
		n = n.SetBytes(bytes[start:end])

		//  fmt.Println("blockhashlist ", fileblocksize, " arr ", blocklistarr, " ", " hex ", hexstring, " modexp ", modSize, " modulus byte size ", modByteSize, " mod size ", modSize, " modulus ", n.String())
		// mdfmt.EncodeBlock(format, fileblocksize, hlistarray, modSize, n.String());	
		if i + 1 != blocks {
			mdfmt.EncodeBlock(format, blockSize, hlistarray, int(modSize), n.String());
		} else {
			mdfmt.EncodeBlock(format, remainder, hlistarray, int(modSize), n.String());
		}
		hlistarray = hlistarray[:0]
	}

	// fmt.Println("lll ", filesize, fileblocksize)
	// return bytes, err

	// fmt.Println("test count ", CalcHashSizeFile("sha1:md5"))
	// fmt.Println("test count ", CalcHashSizeFile("sha1:md5:ripe160:sha224"))

        // endcode the end of file formatter
        mdfmt.EncodeEndFile(format)

}

func Convert32(data []byte) (uint32, error) {
	var v uint32
	err := binary.Read(bytes.NewReader(data), binary.BigEndian, &v)
	if err != nil {
		return 0, err
	}
	return v, nil
}

func Convert64(data []byte) (uint64, error) {
	var v uint64
	err := binary.Read(bytes.NewReader(data), binary.BigEndian, &v)
	if err != nil  {
		return 0, err
	}
	return v, nil
}

// calculate the number of file blocks
func calculateFileBlocks(fileSize uint64, blockSize uint64) (uint64, uint64) {

        remainder := fileSize % blockSize
        var blocksCount uint64 = 0
        if remainder == 0 {
                blocksCount = fileSize / blockSize
                remainder = blockSize
        } else {

                blocksCount = (fileSize / blockSize) + 1
        }

        return blocksCount, remainder
}
