package mdUnzipFile

// project mdencode
//
// mdUnzipFile.go 
//
// this package unzips and decodes mdzip files with block signatures and modular floors
// It calls the generate hash context list and creates the modulus scan decoder objects
// This will unzip a mdzip file and write the bytes to an output file
//
// copyright (C) Scott Ross 2019
// 
// https://github.com/singularian/mdencode
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
        "fmt"
        "encoding/binary"
	"io"
	"encoding/hex"
        "os"
        "bufio"
        "bytes"
        "strings"
        "math/big"
        "time"
	"sync"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
	"github.com/singularian/mdencode/code/mdzip_go/mdUnzipMutex"
        "github.com/singularian/mdencode/code/mdzip_go/modScanFile"
)


// mdencode mdUnzipFile struct
type FileData struct {
	commandArgs    int
	fileName       string
	filePath       string
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
	// argument hash list names
	// ax:sha1:sha256
	fileHashListString  string
	blockHashListString string
	keyListString string
	// argument hash list bit arrays
	// 01010101010101010
	// not currently used
	fileHashListArray  []string
	blockHashListArray []string
	// argument signature hash list names
	// ax blake2 sha2
	fileHashListNames  []string
	blockHashListNames []string
	// key list
	sigKeyHashListNames []string
	// hex list
	// 0033CdF0EE
	fileHashHexListArray []string
	// dictionary
	dictionary map[string]string
	// hash list for files
	mdc *mdHashContextList.HashContextList
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
	// input file
	inputFile string
	file *os.File
	// output file
	outputFile string
	outf *os.File
	// post validate
	postvalidate bool
	err error

}

type Hashlist struct {
	HashListString string
	HashListNames []string
	HexListNames []string
}


// Init returns a new mdUnzipFile object
func Init(inputFile string, outputFile string, threadCount uint64, postvalidate bool) (md *FileData) {

	mdata := new(FileData)

	mdata.logfile      = ""
	mdata.islogging    = false
	mdata.inputFile    = inputFile
	mdata.outputFile   = outputFile
	mdata.threadCount  = int64(threadCount)
	mdata.postvalidate = postvalidate

	// mdata.OpenFile()

	return mdata
}

// Open the input and output files
func (l *FileData) OpenFile() {

	// open the input file
	l.file, l.err = os.Open(l.inputFile)
	if l.err != nil {
                fmt.Println("Counldn't open input file ", l.err)
                os.Exit(1)
        }

	// open the output file
        l.outf, l.err = os.Create(l.outputFile)
        if l.err != nil {
                fmt.Println("Counldn't open output file ", l.err)
                os.Exit(1)
        }

}

// DecodeFile decodes an mdzip file and writes the decoded blocks to an output file
// it runs a parallel modulus scan on the signature group
func (l *FileData) DecodeFile() int {

	var threadCount = l.threadCount

	l.OpenFile()
	// the close has to be deferred until the end of the function
	defer l.file.Close()
	defer l.outf.Close()

	// read the file into  an array
	// stats, err := l.file.Stat()
	stats, _ := l.file.Stat()
	var size int64 = stats.Size()
        bytes := make([]byte, size)

	// fmt.Println("size ", size)
	// os.Exit(1)

	// read the file into  an array
        bufr := bufio.NewReader(l.file)
        _, err := bufr.Read(bytes)
	if err != nil {
                fmt.Println("buffer creation error", err)
                os.Exit(3)
        }

	// fmt.Println("buffer ", bytes)

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
        // fmt.Println("FilePath ", string(bytes[start:end]), start, end)
        fmt.Printf("FilePath         %s\n", filepath);

        // fileName
        start = end
        end = start + fileNameLen
        // fmt.Println("filename ", string(bytes[start:end]), start, end, fileNameLen)

        var filename2 = string(bytes[start:end])

        fmt.Printf("Filename         %s\n", filename2);

        // get hashlist string
        start = end
        end = end + fileHashLen
        // fmt.Println("hashlistname ", string(bytes[start:end]))
        fmt.Printf("Hashlistname     %s\n", string(bytes[start:end]))
        // split hash list
        hlist := string(bytes[start:end])
        hashlist := strings.Split(hlist, "-")
        filelist := hashlist[0]
        blocklist := hashlist[1]
	l.fileHashListString  = hashlist[0]
	l.blockHashListString = hashlist[1]
	if len(hashlist) > 2 {
		l.keyListString = hashlist[2]
	}

	// l.fileHashListString  = filelist
	// l.blockHashListString = blocklist
        fmt.Println("hashlist        ", filelist, blocklist, l.keyListString)

	// check if the block list is specified
	// exit if it is not specified
	if blocklist == "" {
		fmt.Println("No file block specified")
		os.Exit(0)
	}

	// initialize the mdBlockSize object
	l.mdc = mdHashContextList.Init()

	// set the keylist
	// fmt.Println("Setting keylist ", l.keyListString)
	l.mdc.SetHashListKey(l.keyListString)

	// calculate and return the file signature list byte block size
	_, filelistarr := l.mdc.CalcHashBlockSize(filelist, 0)

        // fmt.Println("test array ", filelistarr)
	l.fileHashListNames = strings.Split(filelist, ":")
        // get the file hash hex list
	// var fhlistarray []string
        for i:= 0; i < len(filelistarr); i++ {
                start = end
                end = end + uint64(filelistarr[i])
                var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))
                // filePathLen   := binary.BigEndian.(bytes[:40])
                fmt.Printf("File hashlistname %-20s %s\n", l.fileHashListNames[i], hexstring)
		// fhlistarray = append(fhlistarray, hexstring)
		l.fileHashHexListArray = append(l.fileHashHexListArray, hexstring)
        }

	// calculate and return the file signature block byte size list
	mdblocksize, blocklistarr := l.mdc.CalcHashBlockSize(blocklist, 1)
	// _, blocklistarr := mdBlock.CalcHashBlockSize(blocklist)

	// calculate the file blocks count and the last file block size
	// ie if the file size is 100 and block size is 12 calculate the number of blocks and
	// remainder
	// 100 byte file contains 9 blocks with the last block 4 bytes
        blocks, remainder := l.calculateFileBlocks(fileSize, blockSize)

	// create the mutex
	mutex := mdUnzipMutex.Init()

	// create the block hash context list for the threads
	// maybe rename this
	hcListArr := []*mdHashContextList.HashContextList{}
	var threadNum int64 = 0

	// add the block hash context lists to the array list
	for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
		threadmdcl := mdHashContextList.Init()
		hcListArr = append(hcListArr, threadmdcl)
	}

	// initialize the file and block hash context list
	for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
		_, _ = hcListArr[threadNum].CalcHashBlockSize(filelist, 0) 
		_, _ = hcListArr[threadNum].CalcHashBlockSize(blocklist, 1) 
		hcListArr[threadNum].SetHashListKey(l.keyListString)
		hcListArr[threadNum].CreateHashListMap(blocklist, 1, 1)
		// fmt.Println("testing context list ", hcListArr[threadNum].GetBlockHash())
	}

        var blockNumber uint64
        var modByteSize uint64

	// calculate the modulus remainder byte block size
        // also consider odd number modulus sizes ie 33 or 65 bytes
        if modSize % 8 == 0 {
                modByteSize = modSize / 8
        } else if modSize <= 8 {
                modByteSize = 1;
        } else {
                modByteSize = (modSize / 8) + 1
        }

	// initialize the block hash modulus scan object list
	blockNumber = 0
	mdp := []*modScanFile.DecodeData{}
	for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
		// md := modScanFile.Init(int64(currentBlocksize), int64(modSize), int64(blockNumber), threadNum, int64(threadCount), mutex, hcListArr[thread])

		// todo rename block hash context list hcListArr
		md := modScanFile.Init(int64(blockSize), int64(modSize), 0, threadNum, int64(threadCount), mutex, hcListArr[threadNum])
		mdp = append(mdp, md)
	}

	var c sync.WaitGroup

	// set the start time
	startTime := time.Now()

        var hlistarray []string
	var modExp uint32 = 0

        for blockNumber = 0; blockNumber < blocks; blockNumber++ {
                start = end
                // end = end + fileblocksize + modByteSize + 4;
                // end = end + fileblocksize;
                // var hexstring = fmt.Sprintf("%x", string(bytes[start:end]))

		// create a byteblock array of the file block hash bytes
		var blockXsize = start + mdblocksize
		hashByteBlock := bytes[start:blockXsize]
		// fmt.Printf("Hash ByteBlock %x\n", hashByteBlock)
		l.mdc.SetBlockHash(hashByteBlock)

		// create a string of the file block bytes
                for j := 0; j < len(blocklistarr); j++ {
                        start = end
                        end = end + uint64(blocklistarr[j])
                        hlistarray = append(hlistarray, fmt.Sprintf("%x", string(bytes[start:end])))
                }
                hstring := strings.Join(hlistarray, ":")

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
			modExp    = binary.BigEndian.Uint32(bytes[start:end])
		}

		// calculate the modulus remainder bigint
                start = end
                end = end + modByteSize
                n := new(big.Int)
                n = n.SetBytes(bytes[start:end])

                // fmt.Println("blockhashlist ", fileblocksize, " arr ", blocklistarr, " ", " hex ", hexstring, " modexp ", modSize, " modulus byte size ", modByteSize, " mod size ", modSize, " modulus ", n.String())
                var currentBlocksize uint64 = 0
		fmt.Println("\n")

                if blockNumber + 1 != blocks {
                        currentBlocksize = blockSize
                } else {
                        currentBlocksize = remainder
                }

		// display the processing block message
		fmt.Printf("Processing block hash %d currentblocksize %d/%d hash block %v modulus exponenent %d modulus %s\n", blockNumber, currentBlocksize, blockSize, hlistarray, int(modExp), n.String())

                // add one to the wait group
		c.Add(1)

		mdp = []*modScanFile.DecodeData{}
		for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
			// md := modScanFile.Init(int64(currentBlocksize), int64(modSize), int64(blockNumber), threadNum, int64(threadCount), mutex, hcListArr[thread])
			// todo rename block hash context list hcListArr
			md := modScanFile.Init(int64(currentBlocksize), int64(modSize), int64(blockNumber), threadNum, int64(threadCount), mutex, hcListArr[threadNum])
			mdp = append(mdp, md)
		}

		// for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
		//hcListArr[threadNum].SetBlockHash(hashByteBlock)
		//}

                // kick off the modulus can go routines
                var threadStart int64   = 0
                var threadEnd   int64   = int64(threadCount) 
                for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
			// fmt.Println("Kicking off thread ", thread, threadStart, threadEnd, blocklist,  mdp[thread].MatchFound())
			// update the hash context list hash byte block
			hcListArr[threadNum].SetBlockHash(hashByteBlock)

			go mdp[threadNum].ModulusScanFileBytesOrig(modExp, blocklist, hstring, n.String(), int64(currentBlocksize), &c)

			// timing issue when I tried to split these
			// init the ModulusScanFileBytes
                        ///// mdp[threadNum].ModulusScanFileBytes(modExp, blocklist, hstring, n.String())
			// run the ModulusScanFileBytesRun go routine
			///// go mdp[threadNum].ModulusScanFileBytesRun(int64(blockNumber), int64(currentBlocksize), &c)
                }

		// wait for the first modulus can result
		c.Wait()

		// if a mutex match is true write the decoded block to the output file
		if mutex.GetMatchStatus() {
			l.WriteFile(l.outf, mutex.GetFileBuffer())
		}

		// stop the modulus scan threads and reset the matchFound to false
		for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
			// mdp[threadNum].Stop = true
			mdp[threadNum].StopScan()
			mdp[threadNum].ResetMatchFound()
		}

		mutex.ResetMutex()

		hlistarray = hlistarray[:0]
	}

	t := time.Now()
        elapsed := t.Sub(startTime)
	fmt.Println("\nTotal execution time ", elapsed)

	l.outf.Close()

	return 0

}

// Write the decoded file blocks
func (l *FileData) WriteFile(outf *os.File, bytes []byte) {

	binary.Write(outf,binary.LittleEndian,bytes)
}

// post validate the output mdunzipped file with the file hash signature list
func (l *FileData) PostValidate() {

	if l.postvalidate {
		// this should use os.Open instead of os.Create which seems to change the first signature
		f, err := os.Open(l.outputFile)
		if err != nil {
			fmt.Println("Counldn't open output file ", err)
			os.Exit(1)
		}
		defer f.Close()

		// create the file hash list map
		l.mdc.CreateHashListMap(l.fileHashListString, 0, 1)
		var hashListFile = l.mdc.HashList

		fmt.Println("\nValidating file signature list ", l.fileHashListNames)

		for i, hashvalue := range l.fileHashListNames {
			h := hashListFile[hashvalue]
			h.Reset()

			// fmt.Println("Validating file signature ", hashvalue, i, h, hashListFile)
			fmt.Printf("Validating file signature %-20s %d\n", hashvalue, i)

			if _, err := io.Copy(h, f); err != nil {
				fmt.Println("file validation error ", err)
				os.Exit(1)
			}

			// fmt.Println("validation sign ", hashvalue, " ", hex.EncodeToString(h.Sum(nil)), l.fileHashHexListArray[i])
			if hex.EncodeToString(h.Sum(nil)) != l.fileHashHexListArray[i] {
				fmt.Println("File Failed file signature validation ", hashvalue)
				os.Exit(1)
			}
		}

		fmt.Println("File signature validated True")

	}
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
