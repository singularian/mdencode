package main

// mdencode project
// 
// mdlist.go
// 
// mdencode mdzip mdlist block dumper
// this displays the mdzip binary data in a simplified text format
//
// this is a cleaned up simplified version of testBinReadMdFmt.go
// https://github.com/singularian/mdencode
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"encoding/binary"
	"os"
	"bufio"
	"bytes"
	"strings"
	"math/big"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
)


func main() {

	var filename = "default.mdbin"

	var argsNum int = len(os.Args)

	if argsNum == 2 {
		filename  = os.Args[1]
	} else {
		var exe = os.Args[0]
		fmt.Printf("USAGE of %s:\n", os.Args[0])
		fmt.Println("Examples:")
		fmt.Println(exe, " [FILENAME] ")
		os.Exit(0)
	}

	file, err := os.Open(filename)
        if err != nil {
                fmt.Println("counldn't open file ", err)
                os.Exit(1)
        }
        defer file.Close()

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

	// display the current file attributes
	// need to add the version
        fmt.Printf("FileSize %11d\n",  fileSize)
        fmt.Printf("BlockSize %10d\n", blockSize)
        fmt.Printf("ModSize %12d\n",   modSize)
        fmt.Printf("FileNameLength %5d\n", fileNameLen)
        fmt.Printf("FilePathLength %5d\n", fileNameLen)
        fmt.Printf("FileHashLength %5d\n", fileNameLen)

	// get the filename
	var start uint64
	var end   uint64
	// start = filePathLen 
	// end = start + fileNameLen
	// fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	start = 48 
	end = start + filePathLen
	var filepath = string(bytes[start:end])
	// fmt.Println("FilePath     ", string(bytes[start:end]), start, end)
	fmt.Println("FilePath       ", filepath);

	// fileName
        start = end 
        end = start + fileNameLen
        // fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)

	var filename2 = string(bytes[start:end])

	fmt.Println("Filename:      ", filename2);

	// get hashlist string
	start = end
	end = end + fileHashLen
	fmt.Println("Hashlistname   ", string(bytes[start:end]))
	// split hash list
	hlist := string(bytes[start:end])
	hashlist := strings.Split(hlist, "-")
	filelist := hashlist[0]
	blocklist := hashlist[1]

	var keylist string
	if len(hashlist) > 2 {
                keylist = hashlist[2]
        }

	// fmt.Println("hashlist       ", filelist, blocklist)
	fmt.Println("File hashlist  ", filelist)
	fmt.Println("Block hashlist ", blocklist)
	fmt.Println("Keylist        ", keylist, "\n")

        // initialize the mdBlockSize object
	mdBlock := mdBinaryList.Init()

        // calculate and return the file signature list byte block size
        var filelistarr []int
        _, filelistarr = mdBlock.CalcHashBlockSize(filelist)

	// fmt.Println("test array ", filelistarr)
	fileHashListNames := strings.Split(filelist, ":")
	// get the file hash list
	for i:= 0; i < len(filelistarr); i++ {
		start = end
		end = end + uint64(filelistarr[i])
		var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		// fmt.Println("File hashlistname ", st[i], " hex ", hexstring)
		fmt.Printf("File hashlistname %-20s %s\n", fileHashListNames[i], hexstring)
	}

	// check if the block list is specified
	// exit if it is not specified
	if blocklist == "" {
		fmt.Println("No file block specified")
		os.Exit(0)
	}

	fmt.Println("")

	// calculate and return the file signature block byte size list
        // mdblocksize, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)
        _, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)

        // calculate the file blocks count and the last file block size
        // ie if the file size is 100 and block size is 12 calculate the number of blocks and
        // remainder
        // 100 byte file contains 9 blocks with the last block 4 bytes
        blocks, remainder := calculateFileBlocks(fileSize, blockSize)

	// fmt.Println("remainder ", remainder)
	var blockNumber uint64
	var modByteSize uint64 

	// calculate the modulus remainder byte block size
	// also consider odd number modulus sizes ie 33 or 65 bits	
        if modSize % 8 == 0 {
                modByteSize = modSize / 8
        } else if modSize <= 8 {
                modByteSize = 1;
        } else {
                modByteSize = (modSize / 8) + 1
        }

	// var hlistarray = strings.Split(hashhex, ":")
	var hlistarray []string
	var modExp uint32 = 0
	// fmt.Println("mod byte size ", modSize, " ", modByteSize)
	for blockNumber = 0; blockNumber < blocks; blockNumber++ {
		start = end
		// end = end + fileblocksize + modByteSize + 4;
		// end = end + fileblocksize;
		// var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		for i:= 0; i < len(blocklistarr); i++ {
			start = end
			end = end + uint64(blocklistarr[i])
			hlistarray = append(hlistarray, fmt.Sprintf("%x", string(bytes[start:end])))
		}

		// calculate the binary modulus exponent
                // if the block size is less than or equal to 32 bytes use a single byte to decode the modulus exponent
                // 32 bytes * 8 bits = 256
                if blockSize <= 32 {
                        start = end
                        end = end + 1
                        //modExp    = binary.BigEndian.Uint32(bytes[start:end])
                        modExp    = uint32(bytes[start])
                // if the block size is greater than 32 bytes use a uint32 to encode the modulus exponent
                } else {
                        start = end
                        end = end + 4
                        modExp = binary.BigEndian.Uint32(bytes[start:end])
                }

		// caldulate the modulus
		start = end
		end = end + modByteSize
		n := new(big.Int)
		n = n.SetBytes(bytes[start:end])

		// display the block hash list
		if blockNumber + 1 != blocks {
			// mdfmt.EncodeBlock(format, blockSize, hlistarray, int(modSize), n.String());
			// fmt.Println("block hash ", blockSize, hlistarray, int(modSize), n.String());
			fmt.Printf("block hash %5d block size %5d/%d signature %s exponent %5d modulus %50s \n", blockNumber, blockSize, blockSize, hlistarray, modExp, n.String());
		} else {
			// mdfmt.EncodeBlock(format, remainder, hlistarray, int(modSize), n.String());
			fmt.Printf("block hash %5d block size %5d/%d signature %s exponent %5d modulus %50s \n", blockNumber, remainder, blockSize, hlistarray, modExp, n.String());
		}
		hlistarray = hlistarray[:0]
	}


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
