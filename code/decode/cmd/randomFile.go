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
        filesize uint64 
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
	flag.Uint64Var(&fd.filesize, "size", 100, "Random File Bytes Size")
	flag.StringVar(&fd.outputfilename, "out", "", "Output Filename")

	flag.Usage = printUsage

	flag.Parse()

	if argsNumber != 3 {
		flag.Usage()
		os.Exit(1)
	}

	bytes, _ := sigRand.GenerateRandomBytes64(fd.filesize)

	fd.WriteFile(bytes)

	fmt.Println("Randomfile ", fd.outputfilename, "Created")

	return 0
}

// Write the random file bytes
func (l *FlagData) WriteFile(bytes []byte) {

	outf, err := os.Create(l.outputfilename)
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

