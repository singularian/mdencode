package mdUnzipFileWG

// mdUnzipFileWG
// this package unzips and decodes mdzip files with block signatures and modular floors
//
// copyright (C) Scott Ross 2017
// 
// https://github.com/singularian/mdencode
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
	"sync"
        // "github.com/singularian/mdencode/code/decode/mdBlockSize"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
	"github.com/singularian/mdencode/code/decode/mdUnzipFileMutex"
        "github.com/singularian/mdencode/code/decode/modScanFileMutex"
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

// DecodeFile decodes and mdzip file and writes the decoded blocks to an output file
// it runs a modulus scan on the signature group
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

	// initialize the mdBlockSize object
	// mdBlock := mdBlockSize.Init()
	mdc := mdHashContextList.Init()

	// calculate and return the file signature list byte block size
	var filelistarr []int
	_, filelistarr = mdc.CalcHashBlockSize(filelist, 0)

        // fmt.Println("test array ", filelistarr)
        st := strings.Split(filelist, ":")
        // get the file hash list
        for i:= 0; i < len(filelistarr); i++ {
                start = end
                end = end + uint64(filelistarr[i])
                var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
                // filePathLen   := binary.BigEndian.(bytes[:40])
                fmt.Println("file hashlistname ", st[i], " hex ", hexstring)
        }

	// calculate and return the file signature block byte size list
	mdblocksize, blocklistarr := mdc.CalcHashBlockSize(blocklist, 1)
	// _, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)

	// calculate the file blocks count and the last file block size
	// ie if the file size is 100 and block size is 12 calculate the number of blocks and
	// remainder
	// 100 byte file contains 9 blocks with the last block 4 bytes
        blocks, remainder := l.calculateFileBlocks(fileSize, blockSize)

	// create the mutex
	mutex := mdUnzipFileMutex.Init()

        var blockNumber uint64
        var modByteSize uint64
        modByteSize = modSize / 8
        if modByteSize == 0 {
                modByteSize = 1
        }

        // var hlistarray = strings.Split(hashhex, ":")
        var hlistarray []string
        for blockNumber = 0; blockNumber < blocks; blockNumber++ {
                start = end
                // end = end + fileblocksize + modByteSize + 4;
                // end = end + fileblocksize;
                // var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))

		// create a byteblock array of the file block hash bytes
		var blockXsize = start + mdblocksize
		hashByteBlock := bytes[start:blockXsize]
		fmt.Printf("Hash ByteBlock %x", hashByteBlock)
		mdc.SetBlockHash(hashByteBlock)

		// create a string of the file block bytes
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
                n = n.SetBytes(bytes[start:end])

                // fmt.Println("blockhashlist ", fileblocksize, " arr ", blocklistarr, " ", " hex ", hexstring, " modexp ", modSize, " modulus byte size ", modByteSize, " mod size ", modSize, " modulus ", n.String())
                var currentBlocksize uint64 = 0
		fmt.Println(" ")

                if blockNumber + 1 != blocks {
                        fmt.Println("Processing block hash ", blockNumber, " blocksize ", blockSize, hlistarray, int(modExp), n.String())
                        currentBlocksize = blockSize
                } else {
                        fmt.Println("Proccesing block hash ", blockNumber, " blocksize ", remainder, hlistarray, int(modExp), n.String())
                        currentBlocksize = remainder
                }

                now := time.Now()
                var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())

                // set up the thread list of go routine objects
                mdp := []*modScanFileMutex.DecodeData{}
                var thread int64 = 0
                // var threadCount int64 = 16
                var emptybytes []byte
                for thread = 0; thread < int64(threadCount); thread++ {
                        md := modScanFileMutex.Init(int64(currentBlocksize), int64(modSize), int64(blockNumber), thread, int64(threadCount), emptybytes, mutex, time)
                        mdp = append(mdp, md)
                }

                // create the wait group
		var c sync.WaitGroup
		c.Add(1)

                // kick off the go routines
                var threadStart int64   = 0
                var threadEnd   int64   = int64(threadCount) 
                for thread = threadStart; thread < threadEnd; thread++ {
                        // fmt.Println("Kicking off thread ", thread, threadStart, threadEnd)
                        go mdp[thread].ModulusScanFileBytes(currentBlocksize, modExp, blocklist, hstring, n.String(), &c)
                }


		c.Wait()

		fmt.Println("testing mutex")
		if mutex.GetMatchStatus() {
				var mlastThread = mutex.GetLastThread() - 1
				fmt.Println("Processing ", mutex.GetMatchStatus())
				fmt.Println("Found block modscan ", blockNumber, " thread ", mlastThread, " block ", fmt.Sprintf("% x", mdp[mlastThread].GetBytes()))
				fmt.Println("Found block Mutex ", blockNumber, " thread ", mlastThread, " block ", fmt.Sprintf("% x", mutex.GetFileBuffer()))

				l.WriteFile(outf, mutex.GetFileBuffer())
		}
		fmt.Println("end testing mutex ", mutex.GetMatchStatus())

		for thread = threadStart; thread < threadEnd; thread++ {
			mdp[thread].Stop = true

		}

		mutex.ResetMutex()

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

// calculate the number of file blocks 
// calculate the size of the last file block remainder
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
