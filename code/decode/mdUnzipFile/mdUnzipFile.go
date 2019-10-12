package mdUnzipFile

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
        "fmt"
        "encoding/binary"
        "os"
        "bufio"
        "bytes"
        "strings"
        "math/big"
        "time"
_       "encoding/json"
_       "encoding/hex"
        "strconv"
        "github.com/singularian/mdencode/code/decode/modScanFile"
//      "runtime"
)


// mdencode struct
type FileData struct {
	commandArgs    int
	fileName       string
	filePath       string
	outputFileName string
	fileSize       uint64
	blockSize      uint64
	blockCount     uint64
	blockRemainder uint64
	modSize        uint64
	modExponent    uint64
	mdFormat       int
	mdVersion      string
	// key string
	key string
	// logfile
	logfile string
	// argument hash list bits
	fileHashListString  string
	blockHashListString string
	// argument hash list bit arrays
	fileHashListArray  []string
	blockHashListArray []string
	// argument signature hash list names
	fileHashListNames  []string
	blockHashListNames []string
	// dictionary
	dictionary map[string]string
	// hash list for files
	// hashList map[string]hash.Hash
	// hash list for blocks
	// hashListBlocks map[string]hash.Hash
	// byte buffer
	filebuffer []byte
	// modulus
	modExp                 int
	fileblockmodulusString string
	fileBlockModulus *big.Int
	// threads
	threadCount int64
	threadStart int64
	threadEnd   int64
	// log writer
	// log *log.Logger
	islogging bool
}

// Init returns a new mdEncode object
func Init() (md *FileData) {

	mdata := new(FileData)
	mdata.logfile      = ""
	mdata.islogging    = false

	return mdata
}

// DecodeFile
func (l *FileData) DecodeFile(inputFile string, outputFile string, threadCount uint64) int {

	file, err := os.Open(inputFile)
        if err != nil {
                fmt.Println("counldn't open input file ", err)
                os.Exit(1)
        }
        defer file.Close()

        outf, err := os.Create(outputFile)
                if err != nil {
                fmt.Println("counldn't open output file ", err)
                os.Exit(1)
        }
        defer outf.Close()

	// read the file into  an array
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

	var filelistarr []int
        //////// filesize, filelistarr = CalcHashSizeFile(filelist)
        _, filelistarr = l.CalcHashSizeFile(filelist)

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
        _, blocklistarr := l.CalcHashSizeFile(blocklist)
        blocks, remainder := l.calculateFileBlocks(fileSize, blockSize)

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
                start = end
                // end = end + fileblocksize + modByteSize + 4;
                // end = end + fileblocksize;
                // var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
                for j := 0; j < len(blocklistarr); j++ {
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
                // fmt.Println("sssssssssssss ", start, end, n.String(), modByteSize)
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

//              callGo( i, int64(currentBlocksize), int64(modExp), uint32(modExp), hstring, n.String())
                now := time.Now()
                var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())

                // set up the thread list of go routine objects
                mdp := []*modScanFile.DecodeData{}
                var thread int64 = 0
                // var threadCount int64 = 16
                var emptybytes []byte
                for thread = 0; thread < int64(threadCount); thread++ {
                        //md := modScan2.Init(blockSizeInt, modSizeInt, thread, threadCount, bytes, time)
                        md := modScanFile.Init(int64(currentBlocksize), int64(modSize), thread, int64(threadCount), emptybytes, time)
                        mdp = append(mdp, md)
                }

                // create a channel the size of the thread list
                // var count int64 = 16
		// fmt.Println("Threads ", threadCount)
                var c chan string = make(chan string, threadCount)

                // kick off the go routines
                var threadStart int64   = 0
                var threadEnd   int64   = int64(threadCount) 
                for thread = threadStart; thread < threadEnd; thread++ {
                        // fmt.Println("Kicking off thread ", thread, threadStart, threadEnd)
                        go mdp[thread].ModulusScanFileBytes(currentBlocksize, modExp, hstring, n.String(), c)
                }

                var cl int64 = 1

                // fmt.Println("starting modulus scan threads ", threadCount, " start thread ", threadStart, " end thread ", threadEnd, " byteblock size ", currentBlocksize, " byteblock ", bytes)

                for resp := range c {
                        // if the modScan result is found close the channel
                        if resp != "Not found" {
                                ri, _ := strconv.Atoi(resp)
                                // fmt.Println("Found block XXXX", ri, fmt.Sprintf("% x", mdp[int(ri)].Byteblock))
                                fmt.Println("Found block ", i, " thread ", ri, " ", fmt.Sprintf("% x", mdp[int(ri)].GetBytes()))
                                l.WriteFile(outf, mdp[int(ri)].GetBytes())
                                close(c)
                                break
                        // otherwise if the result count equals the thread count close the channel and break
                        } else if cl == int64(threadCount) && resp == "Not found" {
                                // fmt.Println("close the channel if the last thread has returned a value", cl)
                                // close(c)
                                fmt.Println("Not Found")
                        // otherwise increment the channel count
                        } else {
                                fmt.Println("incrementing thread counter ", cl)
                                cl++
                        }
		}
		hlistarray = hlistarray[:0]
	}
	return 0

}

// Write the decoded file blocks
func (l *FileData) WriteFile(outf *os.File, bytes []byte) {

	binary.Write(outf,binary.LittleEndian,bytes)
}

// convert 32
func (l *FileData) Convert32(data []byte) (uint32, error) {
        var v uint32
        err := binary.Read(bytes.NewReader(data), binary.BigEndian, &v)
        if err != nil {
                return 0, err
        }
        return v, nil
}

// convert 64
func (l *FileData) Convert64(data []byte) (uint64, error) {
        var v uint64
        err := binary.Read(bytes.NewReader(data), binary.BigEndian, &v)
        if err != nil  {
                return 0, err
        }
        return v, nil
}


func (l *FileData) CalcHashSizeFile (hashlist string) (uint64, []int) {
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

	 return blocksize, s
}

// calculate the number of file blocks
func (l *FileData) calculateFileBlocks(fileSize uint64, blockSize uint64) (uint64, uint64) {

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

// write to the log
func (l *FileData) Printlog (v ...interface{}) {

//	if l.islogging {
//		l.log.Println(v...)
//	}

}

// display the object type
func (l *FileData) PrintType() {
	fmt.Println("mdUnzipFile")

}
