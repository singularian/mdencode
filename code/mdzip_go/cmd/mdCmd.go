package main

// mdencode project
//
// This is a command line version of a modulus scan decoder
// this will eventually allow a parallel distribution of work between nodes
//
// mdCmd.go
// https://github.com/singularian/mdencode/ 
// copyright (C) Scott Ross 2019
// 
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"flag"
	"fmt"
	"io/ioutil"
	"os"
	"strconv"
	"encoding/hex"
	"sync"
        "github.com/singularian/mdencode/code/decode/mdHashContextList"
        "github.com/singularian/mdencode/code/mdzip_go/mdUnzipMutex"
        "github.com/singularian/mdencode/code/mdzip_go/modScanFile"
)

// mdencode flag struct                       
type FlagData struct {
	///// inputFilename string
	outputFilename string
	blocksize      string
	modsize        string
	hashliststring string
	bhashlist      string
	// bytes
	bytes      []byte
	// hex string
	hexstring      string
	// signature keys
	keylist        string
	// modulus
	modexp         string
	modremainder   string
	// thread parameters
        thread          int64
        threadStart     int64
        threadEnd       int64
}

func main() {

	md()
        os.Exit(0)

}


// md decode a signature block
// this will eventually be a distributed node
// to allow parallel distribution of decode tasks accross servers 
func md() int {

	var argsNumber int = len(os.Args)

	fd := new(FlagData)

	flag.StringVar(&fd.blocksize, "block", "17", "File Block Size Bytes")
	flag.StringVar(&fd.modsize,   "mod", "32", "Modulus Size in Bits")
	flag.StringVar(&fd.hashliststring, "h", "aes8", "Block Hash String List aes8:sha1")
	flag.StringVar(&fd.bhashlist, "bh", "01001", "Block Hash Bit String List")
	flag.StringVar(&fd.keylist,   "keylist", "", "Signature Key List")
	flag.StringVar(&fd.hexstring, "hex", "", "Specify a Hash Byte Block HEX byte string")
	flag.StringVar(&fd.outputFilename, "out", "", "Output Filename")
	flag.StringVar(&fd.modexp, "exp", "", "Modulus Exponent")
	flag.StringVar(&fd.modremainder, "rem", "", "Modulus Remainder")
	flag.Int64Var(&fd.thread, "thread", 16, "Go Routine Threadsize")
        flag.Int64Var(&fd.threadStart, "start", 0, "Thread Start (Allows threads to be divided between multiple computers)")
        flag.Int64Var(&fd.threadEnd, "end", 0, "Thread End (Allows threads to be divided between multiple computers)")

	flag.Usage = printUsage
	flag.Parse()

	if argsNumber == 1 {
                flag.Usage()
                os.Exit(0)
        }

	// if the thread is zero set it to the default
	if fd.thread == 0 {
		fd.thread = 16
	}

	// decode the signature
	fd.mdDecode()

	return 0
}

// mdDecode the input block parameters
func  (fd *FlagData) mdDecode() {
	// initialize the mdBlockSize object
	mdc := mdHashContextList.Init()
	// set the keylist
	mdc.SetHashListKey(fd.keylist)

	// set modulus scan variables
	// modexp       := int32(fd.modexp)
	// modSize, _   := strconv.ParseInt(fd.modsize, 10, 64)
	modExp, _    := strconv.ParseInt(fd.modexp, 10, 32)
	modRemainder := fd.modremainder
	hstring      := fd.hashliststring
	blocklist    := fd.hashliststring
	hashByteBlock, err := hex.DecodeString(fd.hexstring)
	if err != nil {
		panic(err)
		os.Exit(1)
	}

	blockSize, _ := strconv.ParseInt(fd.blocksize, 10, 64)
	modSize,   _ := strconv.ParseInt(fd.modsize, 10, 64)

	if blockSize == 0 {
		fmt.Println("Blocksize can't be zero")
		os.Exit(1)
	}

        fmt.Println("Starting MDdecode")
        fmt.Printf("Block Size          %-30s\n", fd.blocksize)
        fmt.Printf("Hashlist String     %-30s\n", fd.hashliststring)
        fmt.Printf("Modulus Size        %-30s\n", fd.modsize)
        fmt.Printf("Modulus Exponent    %-30s\n", fd.modexp)
        fmt.Printf("Modulus Remainder   %-30s\n", fd.modremainder)

	// create the mutex
        mutex := mdUnzipMutex.Init()

	// create the block hash context list for the threads
        // maybe rename this
        hcListArr := []*mdHashContextList.HashContextList{}

	var threadNum int64 = 0
	var threadCount int64 = fd.thread 
	// add the block hash context lists to the array list
        for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
                threadmdcl := mdHashContextList.Init()
                hcListArr = append(hcListArr, threadmdcl)
        }

	// initialize the file and block hash context list
	// in this case just the block hash list
        for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
                _, _ = hcListArr[threadNum].CalcHashBlockSize(blocklist, 1)
                hcListArr[threadNum].SetHashListKey(fd.keylist)
                hcListArr[threadNum].CreateHashListMap(blocklist, 1, 1)
                // fmt.Println("testing context list ", hcListArr[threadNum].GetBlockHash())
        }


        mdp := []*modScanFile.DecodeData{}
        for threadNum = 0; threadNum < int64(threadCount); threadNum++ {
                // md := modScanFile.Init(int64(currentBlocksize), int64(modSize), int64(blockNumber), threadNum, int64(threadCount), mutex, hcListArr[thread])

                md := modScanFile.Init(int64(blockSize), int64(modSize), 0, threadNum, int64(threadCount), mutex, hcListArr[threadNum])
                mdp = append(mdp, md)
        }

        var c sync.WaitGroup

	// add one to the wait group
	c.Add(1)

	var threadStart int64   = 0
	var threadEnd   int64   = int64(threadCount)

	// set the threadStart and threadEnd if specified
	// ie threadStart 3, theadEnd 6 out threadCount 16
	threadStart, threadEnd  = fd.setThread()

	for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
                        // fmt.Println("Kicking off thread ", threadNum, threadStart, threadEnd, blocklist,  mdp[threadNum].MatchFound())
                        // update the hash context list hash byte block
                        hcListArr[threadNum].SetBlockHash(hashByteBlock)

			// TODO: Need to change the ModulusScanFileBytesOrig modExp argument name since it is referring to the modulus bit size instead of modulus exponent
			// func (md *DecodeData) ModulusScanFileBytesOrig(modSize uint32, blocklist string, hashlist string, modRemainder string, blocksize int64, c *sync.WaitGroup) {
                        go mdp[threadNum].ModulusScanFileBytesOrig(uint32(modExp), blocklist, hstring, modRemainder, blockSize, &c)
	}

       // wait for the first modulus can result
       c.Wait()

       // if a mutex match is true display the result or write the decoded byte block to the output file
       if mutex.GetMatchStatus() {
		fmt.Printf("Found Match %X\n", mutex.GetFileBuffer())

		// if the output file is specified write the bytes to a output file
		fd.writeFile(fmt.Sprintf("%X", mutex.GetFileBuffer()))
       }

       // stop the modulus scan threads and reset the matchFound to false
       for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
               mdp[threadNum].StopScan()
               mdp[threadNum].ResetMatchFound()
       }



}

// write the decoded hex string to an output file if it is specified
func  (fd *FlagData) writeFile(hexbytestring string) {


	if fd.outputFilename != "" {
		err := ioutil.WriteFile(fd.outputFilename, []byte(hexbytestring), 0644)

		if err != nil {
			panic(err)
		}
	}


}

// set the threadStart and threadEnd
func  (fd *FlagData) setThread() (start, end int64) {

	var threadStart int64 = 0
        var threadEnd   int64 = 0
	var threadCount int64 = fd.thread

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
	fmt.Printf("Thread Start        %-30d\n", fd.threadStart)
	fmt.Printf("Thread End          %-30d\n", fd.threadEnd)
	fmt.Printf("Threads             %-30d\n\n", fd.thread)

	return threadStart, threadEnd
}


// printUsage
func printUsage() {
        fmt.Printf("USAGE of %s:\n", os.Args[0])
        fmt.Println(`
  -block string
        File Block Size Bytes
  -mod
        Modulus Size in Bits
  -h
        Block Hash String List aes8:sha1
  -bh
        Block Hash Bit String List
  -keylist
        Signature Key List
  -hex
        Specify a Hash Byte Block HEX byte string
  -exp
        Modulus Exponent
  -rem
        Modulus Remainder
  -out
        Output Filename
  -start
        Thread Start (Allows threads to be divided between multiple computers)
  -end
        Thread End (Allows threads to be divided between multiple computers)
  -thread string
        Go Routine Threadsize
    `)

	fmt.Println()

        fmt.Println("Examples:")
        fmt.Println("mdcmd -block=10 -mod=64 -h=sha1_64 -hex=17af4b9d3c68b6bb -exp=79 -rem=5324266269509948425 -thread=16")
        fmt.Println("mdcmd -block=10 -mod=64 -h=sha1_64 -hex=17af4b9d3c68b6bb -exp=79 -rem=5324266269509948425 -thread=16 -thread=16 -start=1 -end=14")
        fmt.Println("mdcmd -block=11 -mod=64 -h=aes8:ax -hex=d72f0e3e362e899364aa114c88fa69d5bdd40ff776883374 -exp=87 -rem=8990898166113033087 -thread=16")
        fmt.Println("mdcmd -block=11 -mod=64 -h=aes8:ax -hex=d72f0e3e362e899364aa114c88fa69d5bdd40ff776883374 -exp=87 -rem=8990898166113033087 -thread=16 -start=1 -end=16")


	fmt.Println()

}
