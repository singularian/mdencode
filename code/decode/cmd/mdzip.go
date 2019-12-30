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
	// "github.com/singularian/mdencode/code/mdencode/mdRand"
	"os"
//	"strconv"
	"regexp"
)

var (
	Version   = "1.0.0"
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
	hwkey string
	// boolean arguments
        // appendfile bool
        // random hashlist booleans
        randomfilehash bool
        randomblockhash bool
        randomfileblockhash bool
        randomfileblocksize bool
        randomModSize bool
        randomEverything bool
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

	var argsNumber int = len(os.Args)
	argsSimple(argsNumber)
	os.Exit(0)

}

// mdencode file
func argsSimple(argsNumber int) int {

	fd := new(FlagData)
	flag.StringVar(&fd.blocksize, "block", "40", "File Block Size Bytes")
	flag.StringVar(&fd.modsize, "mod", "32", "Modulus Size in Bits")
	flag.IntVar(&fd.defaultFormat, "format", 10, "Output Format")
	flag.StringVar(&fd.fhashlist, "fh", "01001", "File Hash Bit String List")
	flag.StringVar(&fd.bhashlist, "bh", "01001", "Block Hash Bit String List")
	flag.StringVar(&fd.uhashlist, "uh", "", "Quaternian Hash String List")
	// flag.BoolVar(&fd.randomfilehash, "fr", false, "Generate A Random File Hash Bit String List")
	//flag.BoolVar(&fd.randomblockhash, "br", false, "Generate A Random Block Hash Bit String List")
	//flag.BoolVar(&fd.randomfileblockhash, "fbr", false, "Generate A Random File and Block Hash Bit String List")
	//flag.BoolVar(&fd.randomfileblocksize, "blockr", false, "Generate A Random File Block Size")
	//flag.BoolVar(&fd.randomModSize, "modr", false, "Generate A Random File Modulus Size")
	// flag.BoolVar(&fd.randomEverything, "all", false, "Randomize Everything")

	// flag.BoolVar(&fd.randomfilehash, "fhr", false, "Generate A Random File Hash Boolean String List")
	// flag.BoolVar(&fd.randomblockhash, "bhr", false, "Generate A Random Block Hash Boolean String List")
	// flag.BoolVar(&fd.randomfileblockhash, "fbhr", false, "Generate A Random File and Block Hash Boolean String List")

	// flag.StringVar(&fd.key, "key", "LomaLindaSanSerento9000", "Signature Key (Minimum 16 bytes for siphash)")
	flag.StringVar(&fd.key, "hwkey", "", "High Way Hash Signature Key (32 bytes)")
	flag.StringVar(&fd.filename, "file", "", "Input Filename")
	// flag.StringVar(&fd.directory, "dir", "", "Input Directory")
	flag.StringVar(&fd.outputfilename, "out", "", "Output Filename")
	flag.StringVar(&fd.outputfilename, "output", "", "Output Filename")
	flag.StringVar(&fd.logfilename, "log", "", "Log Filename")

	//fmt.Println("block ", fd.bhashlist)
	//os.Exit(1)

	flag.Usage = printUsage

	flag.Parse()

	if argsNumber == 1 {
		flag.Usage()
		os.Exit(1)
	}

	// set the current working directory
	fd.getCWD()

        // initial implimentation of the quaternian file or block hashlist arguments
        // this could eventually have blockgroups which makes it have another 4 states 0-7
	if fd.uhashlist != "" {
                var quaternianHashListArray  []string
                var fhashlist string
                var bhashlist string
                re := regexp.MustCompile("[0123]")
                quaternianHashListArray = re.FindAllString(fd.uhashlist, -1)
                for _, element := range quaternianHashListArray {
                        // if it is zero then no hash for that signature
                        if element == "0" {
                                fhashlist += "0"
                                bhashlist += "0"
                        // if it is one than add the file hash list
                        } else if element == "1" {
                                fhashlist += "1"
                                bhashlist += "0"
                        // if it is two add the block hash list
                        } else if element == "2" {
                                fhashlist += "0"
                                bhashlist += "1"
                        // if it is three add file and block hash list
                        } else if element == "3" {
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

	// need to add the block group which is a hash of groups of blocks
	// blockgroup option and number etc
	// ie bg=10 or blockgroups are 10 times the block size
	// this eliminates or is one way to address collsions on a block level

	// initialize the mdencode file object
	// var md = mdEncodeALL.Init()
	fd.md = mdZipFile.Init()
	fd.md.SetByteBlock(false)
	fd.md.SetByteBlockBigInt(false)
	//fd.md.SetKeyFile(fd.key)
	////////////////////fd.md.SetLogFile(fd.logfilename)
	fd.md.SetOutputFile(fd.outputfilename)
	// set the default format
	//fd.md.SetMdFormat(fd.defaultFormat)
	fd.md.SetMdFormat(10)
	// set the hash lists
	fd.md.SetHashLists(fd.fhashlist, fd.bhashlist)

	// if the filename is specified
	// mdencode generate a file signature
	if fd.filename != "" {
		fd.md.MdencodeFile(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.filename, fd.outputfilename)
	}

	// if the drectory is specified
	// mdencode generate a directory signature of all the files
	// if fd.directory != "" {
		// fd.md.hashDirectory(fd.directory)
	//	fd.md.MdencodeDirectory(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.directory, fd.outputfilename)
	//}

	return 0
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
  -hwkey string
        High Way Hash Signature Key (32 Bytes)    
  -log string
        Log Filename
    `)

	fmt.Println()

	fmt.Printf("\n\nBuild Time: %s\n", BuildTime)
	fmt.Printf("Version:    %s 復甦 復活\n", Version)
}

// https://jsonlint.com/
// probably don't need this
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
