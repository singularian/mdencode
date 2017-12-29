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

	// add a key variable or replace or append variable???
	// byteblock variable

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
		fmt.Println("5000    - GO XML")
		fmt.Println("5001    - XML\n")

		fmt.Println("Examples:")
		fmt.Println("md -file=Makefile -block=100 -line=false -bh=0 -fh=1111111 -format=19 -line")
		fmt.Println("md -file=Makefile -block=100 -line=false -bh=111 -fh=0 -format=19 -line")
		fmt.Println("md -file=Makefile -block=100 -line=false -bh=1100111 -fh=1011111 -format=19 -line=true -out=outputfile")
		fmt.Println("md -file=Makefile -block=100 -line=false -bh=100111 -fh=11000111 -format=19 -append=true -line=true -out=outputfile\n")
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

	// mdencode generate a file signature
	md.Mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist2, filename, outfilename)
        return 0
}
