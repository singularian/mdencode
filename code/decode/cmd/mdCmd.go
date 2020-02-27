package main

// mdencode project
//
// a command line version of a modulus scan decoder
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
	"os"
	"strconv"
	"encoding/hex"
	"sync"
        "github.com/singularian/mdencode/code/decode/mdHashContextList"
        "github.com/singularian/mdencode/code/decode/mdUnzipMutex"
        "github.com/singularian/mdencode/code/decode/modScanFile"
)

// mdencode flag struct                       
type FlagData struct {
	///// inputFilename string
	///// outputFilename string
	blocksize string
	modsize string
	hashliststring string
	bhashlist string
	// bytes
	bytes      []byte
	// hex string
	hexstring string
	// signature keys
	keylist string
	// modulus
	modexp string
	modremainder string
	// thread parameters
        thread       int64
        threadStart int64
        threadEnd   int64
	// file post validation
	postval bool
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
	/////// flag.StringVar(&fd.outputFilename, "out", "", "Output Filename")
	flag.StringVar(&fd.modexp, "exp", "", "Modulus Exponent")
	flag.StringVar(&fd.modremainder, "rem", "", "Modulus Remainder")
	flag.Int64Var(&fd.thread, "thread", 16, "Go Routine Threadsize")
        // flag.Int64Var(&fd.threadStart, "start", 0, "Thread Start (Allows threads to be skipped for multiple computers)")
        // flag.Int64Var(&fd.threadEnd, "end", 0, "Thread End (Allows threads to be skipped for multiple computers)")
	////////// flag.BoolVar(&fd.postval, "val", false, "Run the File Hash List Post Validation")

	flag.Usage = printUsage
	flag.Parse()

	if argsNumber == 1 {
                flag.Usage()
                os.Exit(1)
        }

	// if the thread is zero set it to the default
	if fd.thread == 0 {
		fd.thread = 16
	}

	// decode the signature
	fd.mdDecode()

	return 0
}

func  (fd *FlagData) mdDecode() {
	// initialize the mdBlockSize object
	mdc := mdHashContextList.Init()
	// set the keylist
	mdc.SetHashListKey(fd.keylist)

	fmt.Println("Starting mddecode")
	fmt.Printf("modsize           %-30s\n", fd.modsize)
	fmt.Printf("hashliststring    %-30s\n",  fd.hashliststring)
	fmt.Printf("modexponent       %-30s\n", fd.modexp)
	fmt.Printf("modulus remainder %-30s\n\n", fd.modremainder)

	// set modulus scan variables
	// modexp       := int32(fd.modexp)
	// modSize, _   := strconv.ParseInt(fd.modsize, 10, 64)
	modExp, _    := strconv.ParseInt(fd.modexp, 10, 32)
	modRemainder := fd.modremainder
	hstring      := fd.hashliststring
	// =========================================
	blocklist := fd.hashliststring
	hashByteBlock, err := hex.DecodeString(fd.hexstring)
	if err != nil {
		panic(err)
		os.Exit(1)
	}
	// =========================================

	// n, err := strconv.ParseInt(s, 10, 64)
	blockSize, _ := strconv.ParseInt(fd.blocksize, 10, 64)
	modSize,   _ := strconv.ParseInt(fd.modsize, 10, 64)

	// create the mutex
        mutex := mdUnzipMutex.Init()

	// create the block hash context list for the threads
        // maybe rename this
        hcListArr := []*mdHashContextList.HashContextList{}

	var threadNum int64 = 0
	var threadCount int64 = 16
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
	for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
                        // fmt.Println("Kicking off thread ", threadNum, threadStart, threadEnd, blocklist,  mdp[threadNum].MatchFound())
                        // update the hash context list hash byte block
                        hcListArr[threadNum].SetBlockHash(hashByteBlock)

			// func (md *DecodeData) ModulusScanFileBytesOrig(modSize uint32, blocklist string, hashlist string, modRemainder string, blocksize int64, c *sync.WaitGroup) {
                        go mdp[threadNum].ModulusScanFileBytesOrig(uint32(modExp), blocklist, hstring, modRemainder, blockSize, &c)
	}

       // wait for the first modulus can result
       c.Wait()

       // if a mutex match is true write the decoded block to the output file
       if mutex.GetMatchStatus() {
		fmt.Printf("Found Match %X\n", mutex.GetFileBuffer())
       }

       // stop the modulus scan threads and reset the matchFound to false
       for threadNum = threadStart; threadNum < threadEnd; threadNum++ {
               mdp[threadNum].StopScan()
               mdp[threadNum].ResetMatchFound()
       }



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
  -thread string
        Go Routine Threadsize
    `)

	fmt.Println()

        fmt.Println("Examples:")
        fmt.Println("mdCmd -block=10 -mod=64 -h=sha1_64 -hex=17af4b9d3c68b6bb -exp=79 -rem=5324266269509948425 -thread=16")


	fmt.Println()

}
