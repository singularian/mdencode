package main

import (
	"fmt"
	"encoding/binary"
	"os"
	"bufio"
	"bytes"
	"strings"
)


func main() {

	var filename = "default.mdbin"
	file, _ := os.Open(filename)

	// if err != nil {
	//    return nil, err
	//}
	defer file.Close()

	stats, _ := file.Stat()
	var size int64 = stats.Size()
	bytes := make([]byte, size)

	bufr := bufio.NewReader(file)
	_,_ = bufr.Read(bytes)

	fmt.Println("buf ", bytes)
	// fileSize,_      := Convert64(bytes[0:8])
	fileSize      := binary.BigEndian.Uint64(bytes[0:8])
	blockSize     := binary.BigEndian.Uint64(bytes[8:16])
	modSize       := binary.BigEndian.Uint64(bytes[16:24])
	fileNameLen   := binary.BigEndian.Uint64(bytes[24:32])
	filePathLen   := binary.BigEndian.Uint64(bytes[32:40])
	fileHashLen   := binary.BigEndian.Uint64(bytes[40:48])

	fmt.Println("fileSize ", fileSize)
	fmt.Println("blockSize ", blockSize)
	fmt.Println("modSize ", modSize)
	fmt.Println("fileNameLength ", fileNameLen)
	fmt.Println("filePathLength ", filePathLen)
	fmt.Println("fileHashLength ", fileHashLen)

	// get the filename
	// /home/nsross/projects/src/github.com/singularian/mdencode/code/mdencode/
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
	var filesize uint64 = CalcHashSizeFile(filelist)
	var fileblocksize uint64 = CalcHashSizeFile(blocklist)

	fmt.Println("lll ", filesize, fileblocksize)
	// return bytes, err

	fmt.Println("test count ", CalcHashSizeFile("sha1:md5"))
	fmt.Println("test count ", CalcHashSizeFile("sha1:md5:ripe160:sha224"))

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

func CalcHashSizeFile (hashlist string) (uint64) {
	s := strings.Split(hashlist, ":")

	var blocksize uint64 = 0

	for i := 0; i < len(s); i++ {
		fmt.Println("hashlist ", s[i])

		switch s[i] {
			case "blake2":
			blocksize = blocksize + 64
                        case "blake2b":
			blocksize = blocksize + 64
                        case "blake2s_128":
                        blocksize = blocksize + 16
                        case "blake2_256":
                        blocksize = blocksize + 32
			case "murmur3"
			blocksize += 16 
			case "md4":
			blocksize = blocksize + 16
			case "md5":
			blocksize = blocksize + 16
			case "ripe160":
			blocksize = blocksize + 20 
			case "sha1":
			blocksize = blocksize + 20
			case "sha224":
			blocksize += 28
			case "sha256":
			blocksize += 32
			case "sha512":
			blocksize += 64
			case "sha512_224":
			blocksize += 28
			case "sha512_256":
			blocksize += 32
			case "sha512_384":
			blocksize += 48
			case "sha3_224":
			blocksize += 28
			case "skein_160":
			blocksize += 20
			case "skein_224":
			blocksize += 28
			case "skein_256":
			blocksize += 32 
			case "skein_384":
			blocksize += 48
			case "skein_512":
			blocksize += 64
			case "skein_1024":
			blocksize += 128
			case "tiger":
			blocksize += 48
			case "whirlpool":
			blocksize += 64 

		}

	}

	return blocksize 
}
