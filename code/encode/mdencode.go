package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"flag"
	"os"
	"github.com/singularian/mdencode/code/encode/mdEncode/mdEncodeALL"
)

func main() {

        var argsNumber int = len(os.Args)

	argsSimple(argsNumber)

        os.Exit(0)

}


// mdencode file 
func argsSimple(argsNumber int) int {

        var defaultFormat int = 4 
	// var blocksize uint64 
	// var modsize uint64 
	var blocksize string
	var modsize string
	var hashlist string
	var hashlist2 string
	var key string
	var appendfile bool
	var byteblock bool
	var byteblockint bool
	var filehashline bool
	var filename string
	var outfilename string
	// logfilename
	var logfilename string
	// initialize sqlite3 md db
	var initdb string

	flag.StringVar(&blocksize, "block", "40", "File Block Size Bytes")
	flag.StringVar(&modsize, "mod", "32", "Modulus Size in Bits")
	flag.IntVar(&defaultFormat, "format", 4, "Output Format")
	flag.StringVar(&hashlist, "fh", "011", "File Hash Boolean String List")
	flag.StringVar(&hashlist2, "bh", "011", "Block Hash Boolean String List")
	flag.StringVar(&key, "key", "LomaLindaSanSerento9000", "Signature Key (Minimum 16 bytes for siphash)")
	flag.StringVar(&filename, "file", "", "Filename")
	flag.BoolVar(&appendfile, "append", false, "Append To Output File")
	flag.BoolVar(&byteblock, "byte", false, "Append the File Byteblock to the Hash List")
	flag.BoolVar(&byteblockint, "blockint", false, "Append the File Byteblock Bigint to the Hash List")
	flag.BoolVar(&filehashline, "line", false, "File Hash as one line")
	flag.StringVar(&outfilename, "out", "", "Output Filename")
	flag.StringVar(&logfilename, "log", "", "Log Filename")
	flag.StringVar(&initdb, "initdb", "", "Create SQLite3 Empty DB File")

	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage of %s:\n", os.Args[0])
		flag.PrintDefaults()
		fmt.Println()
		fmt.Println("Version: 1.0 復甦 復活\n")
	}

	flag.Parse()

	if argsNumber == 1 { 
		flag.Usage()
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
		fmt.Println("md -file=Makefile -block=100 -bh=0 -fh=1 -format=0 -line=true")
		fmt.Println("md -file=Makefile -block=100 -bh=0 -fh=1111111 -format=10 -line=false")
		fmt.Println("md -file=Makefile -block=300 -bh=111 -fh=0 -format=10 -line")
		fmt.Println("md -file=Makefile -block=100 -bh=1100111  -fh=1011111 -format=19 -line=true -out=outputfile")
		fmt.Println("md -file=Makefile -block=100 -bh=100111   -fh=11000111 -format=19 -append=true -line=true -out=outputfile")
		fmt.Println("md -file=Makefile -block=100 -bh=10011101 -fh=11000111 -format=3000 -append=false -line=true -out=outputfile.inform")
		fmt.Println("md -file=Makefile -block=100 -bh=100111   -fh=11000111 -format=4000 -append=true -line=true -out=outputfile.json")
		fmt.Println("md -file=Makefile -block=1000 -mod=0 -bh=100111 -fh=11000111 -format=5000 -append=true -line=true -out=outputfile.xml")
		fmt.Println("md -file=Makefile -block=100 -mod=64 -bh=100111 -fh=11000111 -format=5000 -append=true -line=true -out=outputfile.xml")
		fmt.Println("md -file=Makefile -block=100 -mod=128 -bh=100111 -fh=11000111 -format=5000 -append=true -line=true -out=outputfile.xml\n")
		os.Exit(1)
	}

	// initialize the mdencode file object
	var md = mdEncodeALL.Init()
	md.SetByteBlock(byteblock)
	md.SetByteBlockBigInt(byteblockint)
	md.SetAppendFile(appendfile)
	md.SetFileHashLine(filehashline)
	md.SetKeyFile(key)
	md.SetLogFile(logfilename)

	// if the filename is specified
	// mdencode generate a file signature
	if filename != "" {
		md.Mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist2, filename, outfilename)
	}

	// initialize an empty sqlite3 signature db if specified
	md.InitDB(initdb)
        return 0
}
