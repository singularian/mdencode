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
        blocksize uint64 
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
	flag.Uint64Var(&fd.blocksize, "size", 100, "Random File Block Bytes Size")
	flag.StringVar(&fd.outputfilename, "out", "", "Output Filename")

	flag.Usage = printUsage

	flag.Parse()

	if argsNumber != 3 {
		flag.Usage()
		os.Exit(0)
	}

	bs := []byte{0,0,0,0,0}

	var a uint64 = 0
	// actually just create a bunch of random file blocks
	for a = 0; a < fd.blocksize; a++ {
		fd.WriteFile(bs)
		bytes, _ := sigRand.GenerateRandomBytes64(9)
		fd.WriteFile(bytes)
	}

	fmt.Println("Randomfile ", fd.outputfilename, "Created")

	return 0
}

// Write the random file bytes
func (l *FlagData) WriteFile(bytes []byte) {

	outf, err := os.OpenFile(l.outputfilename, os.O_APPEND|os.O_WRONLY|os.O_CREATE, 0600) 
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
  -size uint64
       Random File Bytes Size (default "100")
  -out string
        Output Filename
    `)

	fmt.Println()

}

