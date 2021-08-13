package main

// mdencode project 
//
// randomFile.go
// Creates a random n-byte file
//
// https://github.com/singularian/mdencode/
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"flag"
	"fmt"
	"encoding/binary"
	"os"
_	"strconv"
	"github.com/singularian/mdencode/code/decode/sigRand"
)

var (
	Version   = "1.0.0"
	BuildTime = "2018-06-16-0431 UTC"
)

// mdencode flag struct
type FlagData struct {
        blocknum  uint64
        blocksize uint64
        zerosize  uint64
        firstbyte uint64
        // output file name
        outputfilename string
}

func main() {

	createRandFile()
	os.Exit(0)

}

// create random file
func createRandFile() int {

	var argsNumber int = len(os.Args)

	fd := new(FlagData)
	flag.Uint64Var(&fd.blocknum, "blocknum", 100, "Random File Blocks Number")
	flag.Uint64Var(&fd.blocksize,"bsize",    14,  "Random File Blocks Bytes Size")
	flag.Uint64Var(&fd.zerosize, "zsize",    4,   "Random File Blocks Zero Bytes Size")
	flag.Uint64Var(&fd.firstbyte,"first",    1,   "Random File First Block Byte")
	flag.StringVar(&fd.outputfilename, "out", "", "Output Filename")

	flag.Usage = printUsage

	flag.Parse()

	if argsNumber != 6 {
		flag.Usage()
		os.Exit(0)
	}

	var a uint64 = 0
	var b uint64 = 0
	// actually just create a bunch of random file blocks
	for a = 0; a < fd.blocknum; a++ {
		fmt.Println("Generating block ", a + 1)
		bytes, _ := sigRand.GenerateRandomBytes64(fd.blocksize)
		for b = 0; b < fd.zerosize; b++ {
			bytes[b] = 0;
		}
		if fd.zerosize > 1 {
			bytes[b-1] = uint8(fd.firstbyte)
		}
		fd.WriteFile(bytes)
	}

	fmt.Println("Randomfile ", fd.outputfilename, "Created")

	return 0
}

// Write the random file bytes
func (l *FlagData) WriteFile(bytes []byte) {

	outf, err := os.OpenFile(l.outputfilename, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0600) 
	if err != nil {
		fmt.Println("Counldn't open output file ", err)
		os.Exit(1)
	}
	defer outf.Close()

	binary.Write(outf,binary.LittleEndian,bytes)
}


func printUsage() {
	fmt.Printf("USAGE of %s:\n", os.Args[0])

	// prints the flag arguments in non sorted order
	fmt.Println(`
  -blocknum uint64
       Random File Bytes Size (default "100")
  -bsize uint64
       Random File Blocks Bytes Size (default "14")
  -zsize uint64
       Random File Blocks Zero Bytes Size (default "4")
  -first uint64
       Random File First Block Byte	   
  -out string
        Output Filename

   Examples:
	 ./randfile2 -blocknum=10 -bsize=14 -zsize=5 -first=14 -out=randfile

   Byteblock Examples
     Generate a random 11 byte block with 3 leading zeros and the first as byte as 1
     ./randfile -blocknum=10 -bsize=11 -zsize=3 -first=1 -out=randfile

     Blocksize 11 bytes
 	 0   0   1   127 231 57  115 27  53  37  74
 	 00  00  01  7F  E7  39  73  1B  35  25  4A    	

     Generate a random 14 byte block with 4 leading zeros and the first as byte as 14 or 0E hex
     ./randfile2 -blocknum=100 -bsize=14 -zsize=4 -first=14 -out=randomfile

     Blocksize 14 bytes
 	 00   00  00  0E 26  A2  FD 4E  48  1B  34  0F  21  02  

    `)

	fmt.Println()

}

