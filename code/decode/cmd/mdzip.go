package main

// mdencode project 
//
// mdzip.go
//
// https://github.com/singularian/mdencode/
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"flag"
	"fmt"
	"path/filepath"
        _ "io/ioutil"
	"github.com/singularian/mdencode/code/decode/mdZipFile"
	"os"
	"regexp"
)

var (
	Version   = "1.0.3"
	BuildTime = "2018-06-16-0431 UTC"
)

// mdencode flag struct
type FlagData struct {
	defaultFormat int
        // var blocksize uint64
        // var modsize uint64
        blocksize string
        modsize string
        fhashlist string
        bhashlist string
	uhashlist string
	// signature keys 
        key string
	keylist string
	// randomize the signature keys
	randKey bool
	// current working directory
	cwd string
	cwdoutputfile string
        // input signature filename
        filename string
        // input directory name
        directory string
        // output file name
        outputfilename string
        // logfilename
        logfilename string
	// mdEncodeAll mdZipFile variable
	md *mdZipFile.FileData
}

func main() {

	md()
	os.Exit(0)

}

// mdencode file
func md() int {

	var argsNumber int = len(os.Args)

	fd := new(FlagData)
	flag.StringVar(&fd.blocksize, "block", "40", "File Block Size Bytes")
	flag.StringVar(&fd.modsize,   "mod", "32", "Modulus Size in Bits")
	// flag.IntVar(&fd.defaultFormat, "format", 1000, "Output Format")
	flag.StringVar(&fd.fhashlist, "fh", "01001", "File Hash Bit String List")
	flag.StringVar(&fd.bhashlist, "bh", "01001", "Block Hash Bit String List")
	flag.StringVar(&fd.uhashlist, "uh", "", "Quaternian Hash String List")
	// flag.BoolVar(&fd.randomfilehash, "fr", false, "Generate A Random File Hash Bit String List")
	// flag.BoolVar(&fd.randomblockhash, "br", false, "Generate A Random Block Hash Bit String List")
	flag.StringVar(&fd.keylist,   "keylist", "", "Signature Key List")
	flag.StringVar(&fd.filename,  "file", "", "Input Filename")
	// flag.StringVar(&fd.directory, "dir", "", "Input Directory")
	flag.StringVar(&fd.outputfilename, "out", "", "Output Filename")
	// flag.StringVar(&fd.outputfilename, "output", "", "Output Filename")
	flag.StringVar(&fd.logfilename, "log", "", "Log Filename")


	flag.Usage = printUsage

	flag.Parse()

	if argsNumber == 1 {
		flag.Usage()
		os.Exit(0)
	}

	// set the current working directory
	fd.getCWD()

        // initial implimentation of the quaternian file or block hashlist arguments
        // this could eventually have blockgroups which makes it have another 4 states 0-7
	fd.SetQuaternian()

	// need to add the block group which is a hash of groups of blocks
	// blockgroup option and number etc
	// ie bg=10 or blockgroups are 10 times the block size
	// this eliminates or is one way to address collsions on a block level

	fd.mdzip()

	return 0
}


// mdzip file
func (fd *FlagData) mdzip() {

	// initialize the mdencode mdzip file object
	fd.md = mdZipFile.Init()

	// Set the mdzip parameters
	fd.md.SetKeyList(fd.keylist)

	// fd.md.SetLogFile(fd.logfilename)
	fd.md.SetOutputFile(fd.outputfilename)
	// set the default format
	// fd.md.SetMdFormat(fd.defaultFormat)
	fd.md.SetMdFormat(10)
	// set the hash lists
	fd.md.SetHashLists(fd.fhashlist, fd.bhashlist)


	// if the filename is specified
        // mdzip
        if fd.filename != "" {
                fd.md.MdencodeFile(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.filename, fd.outputfilename)
        }

	// if the drectory is specified
        // mdencode generate a directory signature of all the files
        // if fd.directory != "" {
                // fd.md.hashDirectory(fd.directory)
        //      fd.md.MdencodeDirectory(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.directory, fd.outputfilename)
        //}

}

// set the Quaternian 
// initial implimentation of the quaternian file or block hashlist arguments
// this could eventually have blockgroups which makes it have another 4 states 0-7
func (fd *FlagData) SetQuaternian() {
	if fd.uhashlist != "" {
		var quaternianHashListArray  []string
		var fhashlist string
		var bhashlist string
		re := regexp.MustCompile("[0123]")
		quaternianHashListArray = re.FindAllString(fd.uhashlist, -1)
		for _, element := range quaternianHashListArray {
			switch element {
			// if it is zero then no hash for that signature
			case "0":
				fhashlist += "0"
				bhashlist += "0"
			// if it is one than add the file hash list
			case "1":
				fhashlist += "1"
				bhashlist += "0"
			// if it is two add the block hash list
			case "2":
				fhashlist += "0"
				bhashlist += "1"
			// if it is three add file and block hash list
			case "3":
				fhashlist += "1"
				bhashlist += "1"
			}
		}
		fd.fhashlist = fhashlist
		fd.bhashlist = bhashlist
		// fmt.Println("Quarternian Hashlist ", fd.uhashlist)
		// fmt.Println("Quarternian File Hashlist ", fd.fhashlist)
		// fmt.Println("Quarternian Block Hashlist ", fd.bhashlist)
	}

}

// printUsage 
func printUsage() {
	fmt.Printf("USAGE of %s:\n", os.Args[0])

	// prints the flag arguments in non sorted order
	fmt.Println(`
  -file string
        Input Filename
  -block string
        File Block Size Bytes (default "40")
  -fh string
        File Hash Bit String List (default "011")
  -bh string
        Block Hash Bit String List (default "01001")
  -uh string
        Quarternian Hash String List 
  -mod string
        Modulus Size in Bits (default "32")
  -format int
        Output Format (default 10)
  -out string
        Output Filename
  -keylist string
        Signature Hash Keylist  
  -log string
        Log Filename
    `)

	fmt.Println()

	fmt.Println("MDzip Examples:")
	fmt.Println("mdzip -mod=64 -block=12 -file=randomfile -out=randomfile.mdz -bh=000000000001001 -fh=111")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E")

	fmt.Println("\nMDunzip Examples:")
	fmt.Println("mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=16 -val")

	fmt.Println("")
	fmt.Println("Signature Keys Format:")
	fmt.Println("A signature Key parameter \"-keylist\" is delineatead with a keyname colon hexvalue comma")
	fmt.Println("")
	fmt.Println("  -keylist=keyname:hexvalue")
	fmt.Println("  -keylist=keyname:hexvalue,key2:hexvalue")
	fmt.Println("  -keylist=keyname:hexvalue,key2:hexvalue")
	fmt.Println("  -keylist=keyname:hexvalue,key2:hexvalue,key3:keyvalue,...")
	fmt.Println("")
	fmt.Println("Keylist:")
	fmt.Println("aes8        - uint64 integer 1 to 18 numbers")
	fmt.Println("ax1         - (xxhash128 key 1) uint64 integer 1 to 18 numbers")
	fmt.Println("ax2         - (xxhash128 key 2) uint64 integer 1 to 18 numbers")
	fmt.Println("blake2s_128 - 16+ hex characters")
	fmt.Println("blake2s_256 - 16+ hex characters")
	fmt.Println("hw32        - 1 to 64 hex characters")
	fmt.Println("fh64        - uint64 integer 1 to 18 numbers")
	fmt.Println("hw64        - 1 to 64 hex characters")
	fmt.Println("hw128       - 1 to 64 hex characters")
	fmt.Println("hw256       - 1 to 64 hex characters")
	fmt.Println("murmur3     - uint64 integer 1 to 18 numbers")
	fmt.Println("sip64       - 1 to 32 hex characters")
	fmt.Println("sip128      - 1 to 32 hex characters")
	fmt.Println("xxh32       - uint32 integer 1 to 9 numbers")
	fmt.Println("xxhash64    - uint64 integer 1 to 18 numbers")

	fmt.Println("")
	fmt.Println("Keylist Examples:")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=1 -fh=110011 -keylist=aes8:12345")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000111 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E,hw128:000FFFFFFF,hw256:0011")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000000000000000000000000000001 -fh=110011 -keylist=sip64:FFF11CCAA09")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000000000000000000000000000000000000000000000001 -fh=110011 -keylist=sip128:FFF11CCAA09")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=00000000000000000000000000000000000000000000000000000001 -fh=110011 -keylist=xxhash:112222201")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=000011 -fh=110011 -keylist=blake2s_128:123456789012345672222211000,blake2s_256:123456789A12345F")
	fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=01 -fh=110011 -keylist=ax1:123456,ax2:789347")

	fmt.Printf("\n\nBuild Time: %s\n", BuildTime)
	fmt.Printf("Version:    %s 復甦 復活\n", Version)
}

// get the current working directory for the mdencode
func (fd *FlagData) getCWD() string {
	cwddir, err := filepath.Abs(filepath.Dir(os.Args[0]))
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	fd.cwd = cwddir

	return cwddir
}
