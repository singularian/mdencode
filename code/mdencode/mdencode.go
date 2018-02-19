package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"flag"
	"fmt"
	"path/filepath"
        _ "io/ioutil"
	"github.com/singularian/mdencode/code/encode/mdEncode/mdEncodeALL"
	"github.com/singularian/mdencode/code/encode/mdRand"
	"os"
	"strconv"
)

var (
	Version   = "1.0.0"
	BuildTime = "2015-06-16-0431 UTC"
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
        // initialize sqlite3 md db
        initdb string
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
	flag.StringVar(&fd.fhashlist, "fh", "011", "File Hash Bit String List")
	flag.StringVar(&fd.bhashlist, "bh", "011", "Block Hash Bit String List")
	flag.BoolVar(&fd.randomfilehash, "fr", false, "Generate A Random File Hash Bit String List")
	flag.BoolVar(&fd.randomblockhash, "br", false, "Generate A Random Block Hash Bit String List")
	flag.BoolVar(&fd.randomfileblockhash, "fbr", false, "Generate A Random File and Block Hash Bit String List")
	flag.BoolVar(&fd.randomfileblocksize, "blockr", false, "Generate A Random File Block Size")
	flag.BoolVar(&fd.randomModSize, "modr", false, "Generate A Random File Modulus Size")
	flag.BoolVar(&fd.randomEverything, "all", false, "Randomize Everything")
	// flag.BoolVar(&fd.randomfilehash, "fhr", false, "Generate A Random File Hash Boolean String List")
	// flag.BoolVar(&fd.randomblockhash, "bhr", false, "Generate A Random Block Hash Boolean String List")
	// flag.BoolVar(&fd.randomfileblockhash, "fbhr", false, "Generate A Random File and Block Hash Boolean String List")

	flag.StringVar(&fd.key, "key", "LomaLindaSanSerento9000", "Signature Key (Minimum 16 bytes for siphash)")
	flag.StringVar(&fd.filename, "file", "", "Input Filename")
	flag.StringVar(&fd.directory, "dir", "", "Input Directory")
	flag.BoolVar(&fd.appendfile, "append", false, "Append To Output File")
	flag.BoolVar(&fd.byteblock, "byte", false, "Append the File Byteblock to the Block Hash List")
	flag.BoolVar(&fd.byteblockint, "blockint", false, "Append the File Byteblock Bigint to the Block Hash List")
	flag.BoolVar(&fd.filehashline, "line", false, "File Hash as one line")
	flag.StringVar(&fd.outputfilename, "out", "", "Output Filename")
	flag.StringVar(&fd.logfilename, "log", "", "Log Filename")
	flag.StringVar(&fd.initdb, "initdb", "", "Create SQLite3 Empty DB File")

	flag.Usage = printUsage

	flag.Parse()

	if argsNumber == 1 {
		flag.Usage()
		os.Exit(1)
	}

	// set the current working directory
	fd.getCWD()

	// need to add a combination of fixed and random file signatures
	// ie you specify 111 for the filehash and -fbr for random and it appends to the file signature instead of over writing it
	if fd.randomfilehash {
		fd.fhashlist = mdRand.GetRandomBits(32)
	}
	if fd.randomblockhash {
		fd.bhashlist = mdRand.GetRandomBits(32)
	}
	if fd.randomfileblockhash {
		fd.fhashlist = mdRand.GetRandomBits(32)
		fd.bhashlist = mdRand.GetRandomBits(32)
	}
	if fd.randomfileblocksize {
		fd.blocksize = strconv.FormatUint(mdRand.GetRandomBlockSize(), 10)
	}
	if fd.randomModSize {
		fd.modsize = strconv.FormatUint(mdRand.GetRandomModSize(), 10)
	}
	if fd.randomEverything {
		fd.fhashlist = mdRand.GetRandomBits(32)
		fd.bhashlist = mdRand.GetRandomBits(32)
		fd.blocksize = strconv.FormatUint(mdRand.GetRandomBlockSize(), 10)
		fd.modsize = strconv.FormatUint(mdRand.GetRandomModSize(), 10)
	}

	// initialize the mdencode file object
	// var md = mdEncodeALL.Init()
	fd.md = mdEncodeALL.Init()
	fd.md.SetByteBlock(fd.byteblock)
	fd.md.SetByteBlockBigInt(fd.byteblockint)
	fd.md.SetAppendFile(fd.appendfile)
	fd.md.SetFileHashLine(fd.filehashline)
	fd.md.SetKeyFile(fd.key)
	fd.md.SetLogFile(fd.logfilename)
	// set the default format
	fd.md.SetMdFormat(fd.defaultFormat)
	// set the hash lists
	fd.md.SetHashLists(fd.fhashlist, fd.bhashlist)

	// if the filename is specified
	// mdencode generate a file signature
	if fd.filename != "" {
		fd.md.MdencodeFile(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.filename, fd.outputfilename)
	}

	// if the drectory is specified
	// mdencode generate a directory signature of all the files
	if fd.directory != "" {
		// fd.md.hashDirectory(fd.directory)
		fd.md.MdencodeDirectory(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.directory, fd.outputfilename)
	}

	// initialize an empty sqlite3 signature db if specified
	fd.md.InitDB(fd.initdb)
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
  -fr   
        Generate A Random File Hash Bit String List
  -br   
        Generate A Random Block Hash Bit String List
  -fbr  
        Generate A Random File Hash and Block Hash Bit String List
  -blockr
        Generate A Random File Block Size
  -modr
        Generate A Random File Block Modulus
  -all
        Randomize everything
  -out string
        Output Filename
  -dir string
        Input Directory
  -append
        Append To Output File
  -line
        File Hash as one line
  -byte 
        Append the File Byteblock to the Block Hash List
  -blockint
        Append the File Byteblock Bigint to the Block Hash List
  -log string
        Log Filename
  -initdb string
        Create SQLite3 Empty DB File
  -key string
        Signature Key (Minimum 16 bytes for siphash) (default "LomaLindaSanSerento9000")
    `)

	fmt.Println()

	fmt.Println("Formats:")
	fmt.Println("0 .. 98 - Text")
	fmt.Println("101     - CSV")
	fmt.Println("102     - CSV")
	fmt.Println("1000    - Binary")
	fmt.Println("2000    - SQL Lite 3 DB File")
	fmt.Println("3000    - Inform")
	fmt.Println("4000    - JSON")
	fmt.Println("5000    - XML GO")
	fmt.Println("5001    - XML\n")

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
