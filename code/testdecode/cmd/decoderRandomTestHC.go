package main

// project mdencode
// decoderRandomTestHC.go
//
// decoderRandomTestHC is a test program for the parallel modulus scan on a random generated byte block
// decoderRandomTestHC uses the hash context list 
//
// decoderRandomTestHC creates the byte block randomly with n-bytes and creates an n-bit modular floor
// decoderRandomTestHC will then run a parallel modulus scan to find the original n-byte block with a matching signature
// decoderRandomTestHC uses goroutines to run the scans in sequence and channels to communicate
//
// This currently uses md5 and SHA-1 as the signature tests
// It stores the random byte separately from the modulus scan bytes. If they match the test is a success
//
// This is the test case for mdzip and mdunzip which can use multiple signatures
//
// https://github.com/singularian/mdencode
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"crypto/rand"
	"flag"
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"
	"encoding/json"
	"encoding/hex"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
	"github.com/singularian/mdencode/code/testdecode/modScanHC"
	"runtime"
)

// mdencode flag struct
type FlagData struct {
	blocksize    string
	modsize      string
	bhashlist    string
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
	// hex string
	hexstring string
	// signature keys
	keylist string
}

// generates a random n byte array and then hashes it
// it then runs it through the modulus scan
func main() {

	var argsNumber int = len(os.Args)

	fd := new(FlagData)
	// there is a Uint64 setter
	flag.StringVar(&fd.blocksize, "block", "8", "File Block Size Bytes")
	flag.StringVar(&fd.modsize, "mod", "32", "Modulus Size in Bits")
	flag.StringVar(&fd.bhashlist, "bh", "01001", "Block Hash Bit String List")
	flag.StringVar(&fd.threadsize, "thread", "16", "Go Routine Threadsize")
	flag.Int64Var(&fd.threadStart, "start", 0, "Thread Start (Allows threads to be skipped for multiple computers)")
	flag.Int64Var(&fd.threadEnd, "end", 0, "Thread End (Allows threads to be skipped for multiple computers)")
	flag.StringVar(&fd.bytestring, "bytes", "", "Specify a byte string")
	flag.StringVar(&fd.hexstring, "hex", "", "Specify a HEX byte string")
	flag.StringVar(&fd.keylist,   "keylist", "", "Signature Key List")

	flag.Parse()

	if argsNumber == 1 {
		fmt.Println("Usage ", os.Args[0], " -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES] -start=[THREAD START] -end=[THREAD END] -bytes=[OPTIONAL JSON BYTESTRING] -hex=[OPTIONAL HEX BYTESTRING] keylist=[OPTIONAL HEX BYTESTRING]")
		fmt.Println("Usage ", os.Args[0], " -block=12 -mod=64 -thread=16 -bh=1010101")
		fmt.Println("Usage ", os.Args[0], " -block=9 -mod=64 -thread=10 -bh=11111 -bytes=[1,2,3,4,5]")
		fmt.Println("Usage ", os.Args[0], " -block=8 -mod=64 -thread=10 -bh=1 -bytes=[100,222,30,55,100]")
		fmt.Println("Usage ", os.Args[0], " -block=8 -mod=64 -thread=10 -bh=101 -hex=FF0C3FDDAF")
		fmt.Println("Usage ", os.Args[0], " -block=20 -mod=128 -thread=16 -bh=0000000000000000001 -keylist=aes8:F01100119900112FF11")
		fmt.Println("Usage ", os.Args[0], " -mod=64 -thread=16 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]")
		fmt.Println("Usage ", os.Args[0], " -mod=64 -thread=16 -start=2 -end=5 -hex=0F0F0F22CDFF")
		os.Exit(1)
	}

	// fmt.Println("bhashlist ", fd.bhashlist)

	fd.mddecode(fd.blocksize, fd.modsize, fd.bhashlist, fd.threadsize, fd.threadlist, fd.bytestring, fd.hexstring)
	os.Exit(0)
}

// mdecode file
func (fd *FlagData) mddecode(blocksize string, modsize string, blocklist string, threadsize string, threadlist string, bytestring string, hexstring string) int {

	// test a random byte block
	// arguments:
	//   blocksize bytes
	//   modsize bits
	//   thread size of goroutines
	var blockSizeInt int64
	var modSizeInt int64
	var threadCount int64
	// var thread int64
	blockSizeInt, _ = strconv.ParseInt(blocksize, 10, 64)
	modSizeInt,   _ = strconv.ParseInt(modsize, 10, 64)
	threadCount,  _ = strconv.ParseInt(threadsize, 10, 64)

	cores := runtime.NumCPU()
	runtime.GOMAXPROCS(cores)

	var bytes []byte
	// create a random n byte size byte block
	if len(bytestring) < 1 {
		bytes = make([]byte, blockSizeInt)
		_, _ = rand.Read(bytes)
	}

	// check if the bytes are defined
	if bytestring != "" {
		var replacestring = bytestring
		// treats [10 01] as separate arg
		// replacestring = strings.Replace(replacestring, " ", ",", -2)
		// replacestring = strings.Replace(bytestring, ",,", ",", -1)
		// fmt.Println("bytestring ", replacestring, bytestring)
		s := strings.Split(replacestring, ",")
		length := len(s)
		if length > 0 {
			bytes = make([]byte, length)
			err := json.Unmarshal([]byte(replacestring), &bytes)
			if err != nil {
				fmt.Println("JSON Conversion error in bytestring", err)
				os.Exit(1)
			}
			fmt.Println("buffer ", bytes)
			blockSizeInt = int64(length)
		}
	}

	// check if the hex bytes are defined
	if hexstring != "" {
		bsrc := []byte(hexstring)
		bytes = make([]byte, hex.DecodedLen(len(bsrc)))
		// length := hex.DecodedLen(len(bsrc))
		// nx, err := hex.Decode(bytes, bsrc)
		_, err := hex.Decode(bytes, bsrc)
		if err != nil {
			fmt.Println("Error encoding hex ",err)
			os.Exit(1)
		}
		blockSizeInt = int64(len(bytes))
		// fmt.Println("hex ", bytes, length)
	}

	// example byte slice
	// bytes = []byte{ 0, 10, 22, 38, 240, 171, 146, 123 }
	// _, _ = rand.Read(bytes)

	// set the timestamp
	now := time.Now()
	var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())

	// =================================================
	// initialize the mdBlockSize object
	// mdc := mdHashContextList.Init()
	// create the block hash context list for the threads
	// maybe rename this
	hcListArr := []*mdHashContextList.HashContextList{}
	// var threadNum int64 = 0

	// add the block hash context lists to the array list
	// set the thread start and thread end
        var threadNum int64 = 0
        // var threadStart int64 = 0
        // var threadEnd int64 = threadCount
	for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
	// if (fd.threadStart > 0) && (fd.threadStart < threadCount) {
		threadmdcl := mdHashContextList.Init()
		hcListArr = append(hcListArr, threadmdcl)
	}

        // initialize the Hash Context List
        // this also creates the mdBinaryList object
        mdc := mdHashContextList.Init()
	blockHlist  := mdc.MdBlockSize.CreateHashBlockList(blocklist)

	if len(blockHlist) < 1 {
		fmt.Println("Unrecognized hashlist boolean")
		os.Exit(1)
	}

	//*********************************************************** 
	// convert the block list hash array to string
	//***********************************************************
	fmt.Println("Running Decode")
	fmt.Println("Block Size       ", blockSizeInt)
	fmt.Println("Hashlist         ", blockHlist)
	fmt.Println("Binary Hashlist  ", blocklist)
	// fmt.Println("Thread Size      ", threadCount)

	// create hash list string from the array
	var hashListString = strings.Join(blockHlist, ":")
	// os.Exit(1)

	// initialize the file and block hash context list
        for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
                // _, _ = hcListArr[threadNum].CalcHashBlockSize(filelist, 0)
                _, _ = hcListArr[threadNum].CalcHashBlockSize(hashListString, 1)
		// hcListArr[threadNum].SetHashListKey(l.keyListString)
		hcListArr[threadNum].SetHashListKey(fd.keylist)
                hcListArr[threadNum].CreateHashListMap(hashListString, 1, 1)
                // fmt.Println("testing context list ", hcListArr[threadNum].GetBlockHash())
        }


	// set up the thread list of go routine objects
	mdp := []*modScanHC.DecodeData{}
	for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
		md := modScanHC.Init(blockSizeInt, modSizeInt, threadNum, threadCount, bytes, hashListString, hcListArr[threadNum], time)
		mdp = append(mdp, md)
	}

	var count int64
	// count = threadEnd - threadStart
	// make sure this is counted correctly
	count = threadCount 

	// create a channel the size of the thread list
	var c chan string = make(chan string, count)

	var threadStart int64   = 0
        var threadEnd   int64   = int64(threadCount)

        // set the threadStart and threadEnd if specified
        // ie threadStart 3, theadEnd 6 out threadCount 16
        threadStart, threadEnd  = fd.setThread()

	// kick off the go routines
	for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
		mdp[threadNum].SetModulusScanBytes()
		if threadNum == threadStart {
			fmt.Println("Mod Size         ", modSizeInt)
			fmt.Println("Mod Exponent     ", mdp[threadNum].GetModExponent())
			fmt.Println("Mod Remainder    ", mdp[threadNum].GetModRemainder(), "\n")
		}
		go mdp[threadNum].RunDecode(c)
	}

	var cl int64 = 1
	for resp := range c {
		// if the modScan result is found close the channel
		if resp != "Not found" {
			// fmt.Println("close on the first result \n", resp)
			fmt.Println("Found block ", resp)
			close(c)
			break
			// otherwise if the result count equals the thread count close the channel and break
		} else if cl == count && resp == "Not found" {
			// fmt.Println("close the channel if the last thread has returned a value", cl)
			close(c)
			// otherwise increment the channel count
		} else {
			// fmt.Println("incrementing thread counter ", cl)
			cl++
		}
	}

	return 0

}

// set the threadStart and threadEnd                                                              
func  (fd *FlagData) setThread() (start, end int64) {
        var threadStart int64 = 0
        var threadEnd   int64 = 0
	var threadCount int64 = 0
        // var threadCount int64 = fd.threadsize
	threadCount,  _ = strconv.ParseInt(fd.threadsize, 10, 64)

	threadStart = fd.threadStart
        threadEnd   = fd.threadEnd

        if fd.threadStart == 0 && fd.threadEnd == 0 {
                threadStart = 0
                threadEnd   = threadCount
        } else if fd.threadEnd == 0 {
                fd.threadEnd = threadCount
        } else if fd.threadEnd < fd.threadStart {
                fd.threadEnd = threadCount
        } else if fd.threadStart > threadCount || fd.threadEnd > threadCount {                    
                fmt.Println("ThreadStart or threadEnd can't exceed threadCount")                  
                os.Exit(2)
        }

        // fmt.Println("SetThread ", threadStart, threadEnd, fd.threadStart, fd.threadEnd)        
        fmt.Printf("Thread Start      %-30d\n", fd.threadStart)
        fmt.Printf("Thread End        %-30d\n", fd.threadEnd)
        // fmt.Printf("Threads           %-30d\n\n", fd.thread)
        fmt.Printf("Threads           %-30d\n", threadCount)
	return threadStart, threadEnd
}

