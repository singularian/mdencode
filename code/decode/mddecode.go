package main

import (
	"fmt"
	"encoding/binary"
	"os"
	"bufio"
	"bytes"
	"strings"
	"math/big"
	"time"
_	"encoding/json"
_	"encoding/hex"
	"strconv"
//	"github.com/singularian/mdencode/code/mdencode/mdFormatsImport/mdFormatImport"
	"github.com/singularian/mdencode/code/decode/modScan2"
//	"runtime"
)

/*
// mdencode flag struct                       
type FlagData struct {
        blocksize    string  
        modsize      string
        threadsize   string
        threadlist   string                   
        blockSizeInt int64 
        modSizeInt   int64
        threadCount  int64
        thread       int64
        // thread parameters                  
        threadStart int64
        threadEnd   int64
        // bytes                              
        bytes      []byte
        bytestring string
}
*/

func main() {

	var filename = "default.mdbin"

	var argsNum int = len(os.Args)
	// var format int = 1
	// var fileName   = ""
	// var outputFile = ""

	if argsNum == 2 {
		filename  = os.Args[1]
	} else {
		var exe = os.Args[0]
		fmt.Printf("USAGE of %s:\n", os.Args[0])
		fmt.Println("Examples:")
		fmt.Println(exe, " [FILENAME] ")
		// doesn't have an output file
		os.Exit(0)
	}

	file, err := os.Open(filename)
        if err != nil {
                fmt.Println("counldn't open file ", err)
                os.Exit(1)
        }
        defer file.Close()

        // mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", md.outputFile)
        // mdload := mdFormatImport.Init(format, fileName, "", 0, 0, 0, "", "", outputFile)
        // mdload.SetAppendFile(md.appendfile)
        //mdload.SetAppendFile(false)
        //mdload.SetOutputFile(outputFile)
        //mdfmt := mdload.SetmdFormatNoSQL(true)

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

	fmt.Println("fileSize ", fileSize)
	fmt.Println("blockSize ", blockSize)
	fmt.Println("modSize ", modSize)
	fmt.Println("fileNameLength ", fileNameLen)
	fmt.Println("filePathLength ", filePathLen)
	fmt.Println("fileHashLength ", fileHashLen)

	// get the filename
	var start uint64
	var end   uint64
	// start = filePathLen 
	// end = start + fileNameLen
	// fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)
	start = 48 
	end = start + filePathLen
	var filepath = string(bytes[start:end])
	fmt.Println("filePath ", string(bytes[start:end]), start, end)
	fmt.Println("filePath ", filepath);

	// fileName
        start = end 
        end = start + fileNameLen
        // fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)

	var filename2 = string(bytes[start:end])

	fmt.Println("filename: ", filename2);

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

	// fl :=  strings.Split(hashlist[0], "-")
	// bl :=  strings.Split(hashlist[1], "-")


	// mdfmt.EncodeFileHeader(format2, filename, filepath, int64(fileSize), int64(blockSize), filelist, blocklist, modSize)
	/// // mdfmt.EncodeFileHeader(format, filename2, filepath, int64(fileSize), int64(blockSize), fl, bl, int64(modSize))
	

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
		fmt.Println("hashlistname ", st[i], " hex ", hexstring)
		fmt.Println("hashlistname ", st[i], " hex ", hexstring)
		////// mdfmt.EncodeFileHash(format, st[i], hexstring)
	}


	// get the file block list
	// var blocklistarr []int
        ////fileblocksize, blocklistarr := CalcHashSizeFile(blocklist)
        _, blocklistarr := CalcHashSizeFile(blocklist)
	// fmt.Println("blockhashlist ", blocklist)
        // fileblocksize, _ = CalcHashSizeFile(blocklist)
	// blocks, remainder := calculateFileBlocks(fileSize, blockSize)
	blocks, remainder := calculateFileBlocks(fileSize, blockSize)

	fmt.Println("blah ", remainder)
	var i uint64
	var modByteSize uint64 
	modByteSize = modSize / 8
	if modByteSize == 0 {
		modByteSize = 1 
	}
	// var hlistarray = strings.Split(hashhex, ":")
	var hlistarray []string
	// var blockbytes = make([]byte, blockSize)
	// fmt.Println("mod byte size ", modSize, " ", modByteSize)
	for i = 0; i < blocks; i++ {
	// for i = 0; i < 1; i++ { // test line
		start = end
		// end = end + fileblocksize + modByteSize + 4;
		// end = end + fileblocksize;
		// var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
		for j:= 0; j < len(blocklistarr); j++ {
			start = end
			end = end + uint64(blocklistarr[j])
			hlistarray = append(hlistarray, fmt.Sprintf("%x", string(bytes[start:end])))
		}
		hstring := strings.Join(hlistarray, ":")
		// should make modexp an int16
		start = end
		end = end + 4 
		modExp    := binary.BigEndian.Uint32(bytes[start:end])
		// this is the modulus bigint 
		start = end
		end = end + modByteSize
		n := new(big.Int)
		// fmt.Println("sssssssssssss ", start, end, bytes, n.String())
		fmt.Println("sssssssssssss ", start, end, n.String(), modByteSize)
		n = n.SetBytes(bytes[start:end])

		// fmt.Println("blockhashlist ", fileblocksize, " arr ", blocklistarr, " ", " hex ", hexstring, " modexp ", modSize, " modulus byte size ", modByteSize, " mod size ", modSize, " modulus ", n.String())
		// mdfmt.EncodeBlock(format, fileblocksize, hlistarray, modSize, n.String());
		var currentBlocksize uint64 = 0
		if i + 1 != blocks {
			// mdfmt.EncodeBlock(format, blockSize, hlistarray, int(modSize), n.String());
			fmt.Println("block hash ", i, " ", blockSize, hlistarray, int(modExp), n.String())
			currentBlocksize = blockSize
		} else {
			// mdfmt.EncodeBlock(format, remainder, hlistarray, int(modSize), n.String());
			fmt.Println("block hash ", i, " ", remainder, hlistarray, int(modExp), n.String())
			currentBlocksize = remainder 
		}

//		callGo( i, int64(currentBlocksize), int64(modExp), uint32(modExp), hstring, n.String())

//		fmt.Println("block hash ", blockSize, hlistarray, int(modSize), n.String());

		now := time.Now()
		var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())

		// set up the thread list of go routine objects
		mdp := []*modScan2.DecodeData{}
		var thread int64 = 0 
		var threadCount int64 = 16 
		var emptybytes []byte
		for thread = 0; thread < threadCount; thread++ {
			//md := modScan2.Init(blockSizeInt, modSizeInt, thread, threadCount, bytes, time)
			md := modScan2.Init(int64(currentBlocksize), int64(modSize), thread, threadCount, emptybytes, time)
			mdp = append(mdp, md)
		}

		// create a channel the size of the thread list
		var count int64 = 16
		var c chan string = make(chan string, count)

		// kick off the go routines
		var threadStart int64 = 0 
		var threadEnd int64   = 16
		for thread = threadStart; thread < threadEnd; thread++ {
			// fmt.Println("Kicking off thread ", thread)
			go mdp[thread].ModulusScanFileBytes(currentBlocksize, modExp, hstring, n.String(), c)
		}

		var cl int64 = 1

		// fmt.Println("starting modulus scan threads ", threadCount, " start thread ", threadStart, " end thread ", threadEnd, " byteblock size ", currentBlocksize, " byteblock ", bytes)

		// while {
		for resp := range c {
			// if the modScan result is found close the channel
			if resp != "Not found" {
				// fmt.Println("close on the first result \n", resp)
				fmt.Println("Found block XXXX", i, resp)
                                ri, _ := strconv.Atoi(resp)
                                fmt.Println("Found block XXXX", ri, fmt.Sprintf("% x", mdp[int(ri)].Byteblock))
				// var str = strings.Replace(resp, " ", ",", -1)
/*                        	var xbytes = make([]byte, currentBlocksize)
                        	err := json.Unmarshal([]byte(str), &xbytes)
                        	if err != nil {
                                	fmt.Println("JSON Conversion error in bytestring", err)
                                	os.Exit(1)
                        	}
                        	fmt.Println("buffer ", xbytes)
				fmt.Printf("buffer % x \n", xbytes)
				fmt.Println("")*/
				// bytes = bytes[:0]
				// fmt.Println("Found block ", hex.EncodeToString(resp))
				close(c)
				break
                        // otherwise if the result count equals the thread count close the channel and break
			} else if cl == count && resp == "Not found" {
                        	// fmt.Println("close the channel if the last thread has returned a value", cl)
                        	// close(c)
				fmt.Println("Not Found")
                        // otherwise increment the channel count
			} else {
				fmt.Println("incrementing thread counter ", cl)
				cl++
			}
		}

		
//*/
		hlistarray = hlistarray[:0]
	}

	// fmt.Println("lll ", filesize, fileblocksize)
	// return bytes, err

	// fmt.Println("test count ", CalcHashSizeFile("sha1:md5"))
	// fmt.Println("test count ", CalcHashSizeFile("sha1:md5:ripe160:sha224"))

        // endcode the end of file formatter
        ///// mdfmt.EncodeEndFile(format)

}

func callGo (blocknumber uint64, currentBlocksize int64, modSize int64, modSize2 uint32, hashlist string, modRemainder string  ) {


                now := time.Now()
                var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())

		// var xbytes = make([]byte, currentBlocksize)

                // set up the thread list of go routine objects
                mdp := []*modScan2.DecodeData{}
                var thread int64 = 0
                var threadCount int64 = 16
                var emptybytes []byte
                for thread = 0; thread < threadCount; thread++ {
                        //md := modScan2.Init(blockSizeInt, modSizeInt, thread, threadCount, bytes, time)
                        md := modScan2.Init(int64(currentBlocksize), int64(modSize), thread, threadCount, emptybytes, time)
                        mdp = append(mdp, md)
                }

                // create a channel the size of the thread list
                var count int64 = 16
                var c chan string = make(chan string, count)

                // kick off the go routines
                var threadStart int64 = 0
                var threadEnd int64   = 16
                for thread = threadStart; thread < threadEnd; thread++ {
                        // fmt.Println("Kicking off thread ", thread)
                        go mdp[thread].ModulusScanFileBytes(uint64(currentBlocksize), modSize2, hashlist, modRemainder, c)

                }

		// fmt.Println("starting modulus scan threads ", threadCount, " start thread ", threadStart, " end thread ", threadEnd, " byteblock size ", currentBlocksize, " byteblock ", hashlist, modSize2, modRemainder)

		var cl int64 = 1
                // while {
                for resp := range c {
                        // if the modScan result is found close the channel
                        if resp != "Not found" {
                                // fmt.Println("close on the first result \n", resp)
                                fmt.Println("Found block XXXX", blocknumber, resp)
				ri, _ := strconv.Atoi(resp)
                                fmt.Println("Found block XXXX", blocknumber, mdp[int(ri)].Byteblock)
                                /* var str = strings.Replace(resp, " ", ",", -1)
                                err := json.Unmarshal([]byte(str), &xbytes)
                                if err != nil {
                                        fmt.Println("JSON Conversion error in bytestring", err)
                                        os.Exit(1)
                                }
                                fmt.Println("buffer ", xbytes)
                                fmt.Printf("buffer % x \n", xbytes)
                                fmt.Println("")
				*/
                                // bytes = bytes[:0]
                                // fmt.Println("Found block ", hex.EncodeToString(resp))
                                close(c)
                                break
                        // otherwise if the result count equals the thread count close the channel and break
                        } else if cl == count && resp == "Not found" {
                                // fmt.Println("close the channel if the last thread has returned a value", cl)
                                // close(c)
                                fmt.Println("Not Found")
                        // otherwise increment the channel count
                        } else {
                                fmt.Println("incrementing thread counter ", cl)
                                cl++
                        }
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
