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
	"github.com/singularian/mdencode/code/mdencode/mdFormatsImport/mdFormatImport"
)


func main() {

	var filename = "default.mdbin"

	var argsNum int = len(os.Args)
	var format int = 1

	if argsNum == 2 {
		format, _ = strconv.Atoi(os.Args[1])
	} else if (argsNum == 3) {
		format, _ = strconv.Atoi(os.Args[1])
		filename  = os.Args[2]
	} else {
		var exe = os.Args[0]
		fmt.Printf("USAGE of %s:\n", os.Args[0])
		fmt.Println("Examples:")
		fmt.Println(exe, " [FORMAT] ")
		fmt.Println(exe, " [FORMAT] [FILENAME]")
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

	var fileName = ""
	var outputFile = ""
        // mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", md.outputFile)
        mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", outputFile)
        // mdload.SetAppendFile(md.appendfile)
        mdload.SetAppendFile(false)
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


	// mdfmt.EncodeFileHeader(format2, filename, filepath, int64(fileSize), int64(blockSize), filelist, blocklist, modSize)
	mdfmt.EncodeFileHeader(format, filename2, filepath, int64(fileSize), int64(blockSize), fl, bl, int64(modSize))

	//// var filesize uint64
	var filelistarr []int 
	//////// filesize, filelistarr = CalcHashSizeFile(filelist)
	_, filelistarr = CalcHashSizeFile(filelist)

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


	// get the file block list
	// var blocklistarr []int
        ////fileblocksize, blocklistarr := CalcHashSizeFile(blocklist)
        _, blocklistarr := CalcHashSizeFile(blocklist)
	// fmt.Println("blockhashlist ", blocklist)
        // fileblocksize, _ = CalcHashSizeFile(blocklist)
	// blocks, remainder := calculateFileBlocks(fileSize, blockSize)
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

func CalcHashSizeFile (hashlist string) (uint64, []int) {
	st := strings.Split(hashlist, ":")

	var blocksize uint64 = 0
	var s []int
	// var s = make([]int, 100)

	// s = append(s, 5)

	for i := 0; i < len(st); i++ {
		// fmt.Println("hashlist ", st[i])

		switch st[i] {
			case "blake2":
			s = append(s, 64)
                        case "blake2b":
			s = append(s, 64)
                        case "blake2s_128":
			s = append(s, 16)
                        case "blake2_256":
			s = append(s, 32)
			case "murmur3":
			s = append(s, 16)
			case "md4":
			s = append(s, 16)
			case "md5":
			s = append(s, 16)
			case "ripe160":
			s = append(s, 20)
			case "sha1":
			s = append(s, 20)
			case "sha224":
			s = append(s, 28)
			case "sha256":
			s = append(s, 32)
			case "sha512":
			s = append(s, 64)
			case "sha512_224":
			s = append(s, 28)
			case "sha512_256":
			s = append(s, 32)
			case "sha512_384":
			s = append(s, 48)
			case "sha3_224":
			s = append(s, 28)
			case "sha3_256":
			s = append(s, 32)
			case "skein_160":
			s = append(s, 20)
			case "skein_224":
			s = append(s, 28)
			case "skein_256":
			s = append(s, 32)
			case "skein_384":
			s = append(s, 48)
			case "skein_512":
			s = append(s, 64)
			case "skein_1024":
			s = append(s, 128)
			case "tiger":
			s = append(s, 48)
			case "whirlpool":
			s = append(s, 64)
		}

	}

	for i := range s {
		blocksize += uint64(s[i])
	}

	// fmt.Println("test ", s)
	return blocksize, s 
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
