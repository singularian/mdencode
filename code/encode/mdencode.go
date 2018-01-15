package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"crypto/rand"
	"encoding/binary"
	"flag"
	"fmt"
        // "io"
        "io/ioutil"
	"github.com/singularian/mdencode/code/encode/mdEncode/mdEncodeALL"
	"os"
)

var (
	Version   = "1.0.0"
	BuildTime = "2015-06-16-0431 UTC"
)

// mdencode struct
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
        // input signature filename
        filename string
        // input directory name
        directory string
        // output file name
        outfilename string
        // logfilename
        logfilename string
        // initialize sqlite3 md db
        initdb string
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
	flag.IntVar(&fd.defaultFormat, "format", 4, "Output Format")
	flag.StringVar(&fd.fhashlist, "fh", "011", "File Hash Boolean String List")
	flag.StringVar(&fd.bhashlist, "bh", "011", "Block Hash Boolean String List")
	flag.BoolVar(&fd.randomfilehash, "fr", false, "Generate Random File Hash Boolean String List")
	flag.BoolVar(&fd.randomblockhash, "br", false, "Generate Random Block Hash Boolean String List")

	flag.StringVar(&fd.key, "key", "LomaLindaSanSerento9000", "Signature Key (Minimum 16 bytes for siphash)")
	flag.StringVar(&fd.filename, "file", "", "Filename")
	flag.StringVar(&fd.directory, "dir", "", "Directory")
	flag.BoolVar(&fd.appendfile, "append", false, "Append To Output File")
	flag.BoolVar(&fd.byteblock, "byte", false, "Append the File Byteblock to the Block Hash List")
	flag.BoolVar(&fd.byteblockint, "blockint", false, "Append the File Byteblock Bigint to the Block Hash List")
	flag.BoolVar(&fd.filehashline, "line", false, "File Hash as one line")
	flag.StringVar(&fd.outfilename, "out", "", "Output Filename")
	flag.StringVar(&fd.logfilename, "log", "", "Log Filename")
	flag.StringVar(&fd.initdb, "initdb", "", "Create SQLite3 Empty DB File")

	flag.Usage = myUsage

	flag.Parse()

	if argsNumber == 1 {
		flag.Usage()
		os.Exit(1)
	}

	if fd.randomfilehash {
		fd.fhashlist = getRandomBits(32)
	}
	if fd.randomblockhash {
		fd.bhashlist = getRandomBits(32)
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

	// if the filename is specified
	// mdencode generate a file signature
	if fd.filename != "" {
		fd.md.Mdencode(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fd.filename, fd.outfilename)
	}

	// if the drectory is specified
	// mdencode generate a directory signature of all the files
	if fd.directory != "" {
		fd.hashDirectory(fd.directory)
	}

	// initialize an empty sqlite3 signature db if specified
	fd.md.InitDB(fd.initdb)
	return 0
}

func myUsage() {
	fmt.Printf("Usage of %s:\n", os.Args[0])
	flag.PrintDefaults()
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

// generate a rand bit string for the file hash list or block hash list
func getRandomBits(length int) string {
	var result string = ""

	var n int32
	binary.Read(rand.Reader, binary.LittleEndian, &n)
	c := n % 50
	if c < 0 {
		c = 5
	}

	b := make([]byte, c)
	_, _ = rand.Read(b)

	// The slice should now contain random bytes instead of only zeroes.
	for v := range b {
		//	s := fmt.Sprintf("%b", b[v])
		s := fmt.Sprintf("%b", b[v]%2)
		result += s
	}
	if result == "" {
		return "1"
	}

	return result
}

func randint64() (int64, error) {
	var b [8]byte
	if _, err := rand.Read(b[:]); err != nil {
		return 0, err
	}
	var result = int64(binary.LittleEndian.Uint64(b[:])) % 5
	// return int64(binary.LittleEndian.Uint64(b[:])), nil
	if result == 0 {
		result = 1
	}
	return result, nil
}

// test directory hash code
func (fd *FlagData) hashDirectory(directory string) {
        files, _ := ioutil.ReadDir(directory)
        for _, file := range files {
                fileName := directory + string(os.PathSeparator) + file.Name()
		// need to skip the output file directory
                if file.IsDir() {
                        fd.hashDirectory(fileName)
                } else {
			fd.md.Mdencode(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fileName, fd.outfilename)
                }
        }

}
