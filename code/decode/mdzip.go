package main

// mdencode
// copyright (C) Scott Ross 2017
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
        key string
        appendfile bool
        byteblock bool
        byteblockint bool
        filehashline bool
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
	// mdEncodeAll variable
	md *mdEncodeALL.FileData
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
	// flag.BoolVar(&fd.randomfilehash, "fr", false, "Generate A Random File Hash Bit String List")
	//flag.BoolVar(&fd.randomblockhash, "br", false, "Generate A Random Block Hash Bit String List")
	//flag.BoolVar(&fd.randomfileblockhash, "fbr", false, "Generate A Random File and Block Hash Bit String List")
	//flag.BoolVar(&fd.randomfileblocksize, "blockr", false, "Generate A Random File Block Size")
	//flag.BoolVar(&fd.randomModSize, "modr", false, "Generate A Random File Modulus Size")
	// flag.BoolVar(&fd.randomEverything, "all", false, "Randomize Everything")
	// specify a bigint number for the modulus or the bit modulus plus the bigint modulus
	// flag.StringVar(&fd.bigint, "big", "011", "Bigint Modulus")

	// flag.BoolVar(&fd.randomfilehash, "fhr", false, "Generate A Random File Hash Boolean String List")
	// flag.BoolVar(&fd.randomblockhash, "bhr", false, "Generate A Random Block Hash Boolean String List")
	// flag.BoolVar(&fd.randomfileblockhash, "fbhr", false, "Generate A Random File and Block Hash Boolean String List")

	// flag.StringVar(&fd.key, "key", "LomaLindaSanSerento9000", "Signature Key (Minimum 16 bytes for siphash)")
	flag.StringVar(&fd.filename, "file", "", "Input Filename")
	// flag.StringVar(&fd.directory, "dir", "", "Input Directory")
	// flag.BoolVar(&fd.appendfile, "append", false, "Append To Output File")
	// flag.BoolVar(&fd.byteblock, "byte", false, "Append the File Byteblock to the Block Hash List")
	// flag.BoolVar(&fd.byteblockint, "blockint", false, "Append the File Byteblock Bigint to the Block Hash List")
	// flag.BoolVar(&fd.filehashline, "line", false, "File Hash as one line")
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


	// need to add the block group which is a hash of groups of blocks
	// blockgroup option and number etc
	// ie bg=10 or blockgroups are 10 times the block size
	// this eliminates or is one way to address collsions on a block level

	// initialize the mdencode file object
	// var md = mdEncodeALL.Init()
	fd.md = mdEncodeALL.Init()
	fd.md.SetByteBlock(fd.byteblock)
	fd.md.SetByteBlockBigInt(fd.byteblockint)
	// fd.md.SetAppendFile(fd.appendfile)
	// fd.md.SetFileHashLine(fd.filehashline)
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
	// fmt.Fprintf(os.Stderr, "Usage of %s:\n", os.Args[0])
	// prints the flag arguments in sorted order
	// flag.PrintDefaults()

	// prints the flag arguments in non sorted order
	fmt.Println(`
  -file string
        Input Filename
  -block string
        File Block Size Bytes (default "40")
  -fh string
        File Hash Bit String List (default "011")
  -bh string
        Block Hash Bit String List (default "011")
  -mod string
        Modulus Size in Bits (default "32")
  -format int
        Output Format (default 10)
  -out string
        Output Filename
  -log string
        Log Filename
    `)

	fmt.Println()

//	fmt.Println("Formats:")
/*	fmt.Println("0 .. 98 - Text")
	fmt.Println("101     - CSV")
	fmt.Println("102     - CSV")
	fmt.Println("1000    - Binary")
	fmt.Println("2000    - SQL Lite 3 DB File")
	fmt.Println("3000    - Inform")
	fmt.Println("4000    - JSON")
	fmt.Println("4300    - Mark Down")
	fmt.Println("4500    - HTML")
	fmt.Println("5000    - XML GO")
	fmt.Println("5001    - XML")
	fmt.Println("8000    - PNG Image Signature")
	fmt.Println("8001    - PNG Image Signature\n")

	fmt.Println("Examples:")
	fmt.Println("md -file=Makefile -block=100 -fh=1 -bh=0 -format=0 -line=true")
	fmt.Println("md -file=Makefile -block=100 fh=1111111 -bh=0 -format=10 -line=false")
	fmt.Println("md -file=Makefile -block=300 -fh=0 -bh=111 -format=10 -line")
	fmt.Println("md -file=Makefile -block=100 -fh=1011111 -bh=1100111 -format=19 -line=true -out=outputfile")
	fmt.Println("md -file=Makefile -block=100 -fh=10100111 -bh=100111   -format=19 -append=true -line=true -out=outputfile")
	fmt.Println("md -file=Makefile -block=100 -fh=1011111101 -bh=10011101 -format=3000 -append=false -line=true -out=outputfile.inform")
	fmt.Println("md -file=Makefile -block=100 -fh=1011111001 -bh=100111   -format=4000 -append=true -line=true -out=outputfile.json")
	fmt.Println("md -file=Makefile -block=1000 -mod=0 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml")
	fmt.Println("md -file=Makefile -block=100 -mod=64 -fh=1011111 -bh=100111 -format=5000 -append=true -line=true -out=outputfile.xml")
	fmt.Println("md -file=Makefile -block=100 -mod=128 -fh=1011111 -bh=100111  -format=5000 -append=true -line=true -out=outputfile.xml")
	fmt.Println("md -file=Makefile -block=350 -mod=32 -fh=101011 -br")
	fmt.Println("md -file=Makefile -block=300 -mod=32 -fr -br")
*/
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
