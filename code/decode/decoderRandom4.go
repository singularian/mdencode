package main

// decoder4
// decoder4 is an example of a prototype parallel modulus scan on a random generated byte block
//
// decoder4 creates the byte block randomly with n-bytes and creates an n-bit modular floor
// decoder4 will then run a parallel modulus scan to find the original n-byte block with a matching signature
// decoder4 uses goroutines to run the scans in sequence
//
// copyright (C) Scott Ross 2019
// https://github.com/singularian/mdencode/blob/master/LICENSE

import ( 
	"flag"
	"crypto/rand"
	"fmt"
	"strconv"
	"os"
	// "log"
	"time"
	"strings"
	// "encoding/binary"
	"encoding/json"
	"github.com/singularian/mdencode/code/decode/modScan"
	"runtime"
)


// mdencode flag struct
type FlagData struct {
	blocksize string
        modsize string
	threadsize string
	threadlist string
	blockSizeInt int64
	modSizeInt int64
	threadCount int64
	thread int64
	// thread parameters
	threadStart int64
	threadEnd int64
	// bytes
	bytes []byte
	bytestring string
}

// generates a random n byte array and then hashes it
// it then runs it through the modulus scan
func main() {

	var argsNumber int = len(os.Args)

	fd := new(FlagData)
	// there is a Uint64 setter
	flag.StringVar(&fd.blocksize, "block", "8", "File Block Size Bytes")
        flag.StringVar(&fd.modsize, "mod", "32", "Modulus Size in Bits")
        flag.StringVar(&fd.threadsize, "thread", "16", "Go Routine Threadsize")
        flag.Int64Var(&fd.threadStart, "start", 0, "Thread Start (Allows threads to be skipped for multiple computers)")
        flag.Int64Var(&fd.threadEnd, "end", 0, "Thread End (Allows threads to be skipped for multiple computers)")
        flag.StringVar(&fd.bytestring, "bytes", "", "Specify a byte string")

	flag.Parse()

	if argsNumber == 1 {
           fmt.Println("Usage ", os.Args[0], " -block=[BLOCKSIZE BYTES] -mod=[MODSIZE BITS] -thread=[THREADSIZE GOROUTINES] -start=[THREAD START] -end=[THREAD END] -bytes=[OPTIONAL JSON BYTESTRING]")
           fmt.Println("Usage ", os.Args[0], " -block=12 -mod=64 -thread=16")
           fmt.Println("Usage ", os.Args[0], " -block=9 -mod=64 -thread=10 -bytes=[1,2,3,4,5]")
           fmt.Println("Usage ", os.Args[0], " -block=8 -mod=64 -thread=10 -bytes=[100,222,30,55,100]")
           fmt.Println("Usage ", os.Args[0], " -mod=64 -thread=16 -start=2 -end=5 -bytes=[100,222,30,55,100,11,123]")
           os.Exit(1)
        }

	fd.mddecode(fd.blocksize, fd.modsize, fd.threadsize, fd.threadlist, fd.bytestring)
	os.Exit(0)
}

// mdecode file
func (fd *FlagData) mddecode(blocksize string, modsize string, threadsize string, threadlist string, bytestring string) int {

        // test a random byte block
        // arguments:
        //   blocksize bytes 
        //   modsize bits 
        //   thread size of goroutines
	var blockSizeInt int64
	var modSizeInt   int64
	var threadCount  int64
	var thread       int64 
	blockSizeInt, _ = strconv.ParseInt(blocksize, 10, 64) 
	modSizeInt  , _ = strconv.ParseInt(modsize, 10, 64) 
	threadCount,  _ = strconv.ParseInt(threadsize, 10, 64) 

	runtime.GOMAXPROCS(int(threadCount))

	var bytes []byte
	// create a random n byte size byte block
	if len(bytestring) < 1  {
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

	// there is a bug in modScan 0 * n + remainder have to fix
	// if I use mod 8 it works
	// create a random n byte size byte block
	// test failure with this byteblock there is a bug with the modular exponent
	// 8 bytes 40 bit mod
	// bytes = []byte{ 0, 10, 22, 38, 240, 171, 146, 123 }
	// _, _ = rand.Read(bytes)

	// set the timestamp
	now := time.Now()
	var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())

	// set up the thread list of go routine objects
	mdp:=[]*modScan.DecodeData{}
        for thread = 0; thread<threadCount; thread++ {
		md := modScan.Init(blockSizeInt, modSizeInt, thread, threadCount, bytes, time)
		mdp=append(mdp,md)
	}

	// set the thread start and thread end
	var threadStart int64 = 0
	var threadEnd   int64 = threadCount
	if (fd.threadStart > 0) && (fd.threadStart < threadCount) {
		threadStart = fd.threadStart 
	}
	if (fd.threadEnd > 0) && (fd.threadEnd < threadCount) {
		threadEnd = fd.threadEnd
	}

	// fmt.Println("start ", threadStart, " ", threadEnd)
	// fmt.Println("start ", fd.threadStart, " ", fd.threadEnd)
	fmt.Println("starting modulus scan threads ", threadCount, " start thread ", threadStart, " end thread ", threadEnd, " byteblock size ", blockSizeInt, " byteblock ", bytes)

	// kick off the thread list go routines
	// for thread = threadStart; thread < threadCount; thread++ {
	var count int64 
	count = threadEnd - threadStart
	// create a channel the size of the thread list
	var c chan string = make(chan string, count)
	// kick off the go routines
	for thread = threadStart; thread < threadEnd; thread++ {
		go mdp[thread].ModulusScanBytes(c)
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

