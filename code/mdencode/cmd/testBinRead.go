package main

import (
	"fmt"
	"encoding/binary"
	"os"
	"bufio"
	"bytes"
	"strings"
	"math/big"
	"github.com/singularian/mdencode/code/decode/mdSignatureList"
)


func main() {

	var filename = "default.mdbin"
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
	// fileSize,_      := Convert64(bytes[0:8])
	fileSize      := binary.BigEndian.Uint64(bytes[0:8])
	blockSize     := binary.BigEndian.Uint64(bytes[8:16])
	modSize       := binary.BigEndian.Uint64(bytes[16:24])
	fileNameLen   := binary.BigEndian.Uint64(bytes[24:32])
	filePathLen   := binary.BigEndian.Uint64(bytes[32:40])
	fileHashLen   := binary.BigEndian.Uint64(bytes[40:48])

	// need to add the version

	fmt.Println("fileSize ", fileSize)
	fmt.Println("blockSize ", blockSize)
	fmt.Println("modSize ", modSize)
	fmt.Println("fileNameLength ", fileNameLen)
	fmt.Println("filePathLength ", filePathLen)
	fmt.Println("fileHashLength ", fileHashLen)

	// get the filename
	// home/uestes/projects/src/github.com/singularian/mdencode/code/mdencode/
	var start uint64
	var end   uint64
	// start = filePathLen 
	// end = start + fileNameLen
	// fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	start = 48 
	end = start + filePathLen
	fmt.Println("filePath ", string(bytes[start:end]), start, end)
	// fileName
        start = end 
        end = start + fileNameLen
        fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	// get hashlist string
	start = end
	end = end + fileHashLen
	fmt.Println("hashlistname ", string(bytes[start:end]))
	// split hash list
	hlist := string(bytes[start:end])
	hashlist := strings.Split(hlist, "-")
	filelist := hashlist[0]
	blocklist := hashlist[1]
	fmt.Println("hashlist ", filelist, blocklist)

        // initialize the mdBlockSize object
	mdBlock := mdSignatureList.Init()

        // calculate and return the file signature list byte block size
        var filelistarr []int
        _, filelistarr = mdBlock.CalcHashBlockSize(filelist)

	fmt.Println("test array ", filelistarr)
	st := strings.Split(filelist, ":")
	// get the file hash list
	for i:= 0; i < len(filelistarr); i++ {
		start = end
		end = end + uint64(filelistarr[i])
		var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		// filePathLen   := binary.BigEndian.(bytes[:40])
		fmt.Println("hashlistname ", st[i], " hex ", hexstring)
	}

        // calculate and return the file signature block byte size list
        // mdblocksize, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)
        fileblocksize, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)
	fmt.Println("blockhashlist ", blocklist)

        // calculate the file blocks count and the last file block size
        // ie if the file size is 100 and block size is 12 calculate the number of blocks and
        // remainder
        // 100 byte file contains 9 blocks with the last block 4 bytes
        // blocks, remainder := calculateFileBlocks(fileSize, blockSize)
        blocks, _ := calculateFileBlocks(fileSize, blockSize)

	var i uint64
	var modByteSize uint64 
	modByteSize = modSize / 8
	if modByteSize == 0 {
		modByteSize = 1 
	}
	fmt.Println("mod byte size ", modSize, " ", modByteSize)

	for i = 0; i < blocks; i++ {
		start = end
		// end = end + fileblocksize + modByteSize + 4;
		end = end + fileblocksize;
		var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		start = end
		end = end + 4 
		modSize       := binary.BigEndian.Uint32(bytes[start:end])
		start = end
		end = end + modByteSize
		n := new(big.Int)
		n = n.SetBytes(bytes[start:end])

		fmt.Println("blockhashlist ", fileblocksize, " arr ", blocklistarr, " ", " hex ", hexstring, " modexp ", modSize, " modulus byte size ", modByteSize, " mod size ", modSize, " modulus ", n.String())
		
	}

	// fmt.Println("lll ", filesize, fileblocksize)
	// return bytes, err

	// fmt.Println("test count ", CalcHashSizeFile("sha1:md5"))
	// fmt.Println("test count ", CalcHashSizeFile("sha1:md5:ripe160:sha224"))

}

func Convert(data []byte) (uint32, error) {
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
