package main

// mdencode project
//
// mdUnzip.go
// https://github.com/singularian/mdencode/ 
// copyright (C) Scott Ross 2019
// 
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"flag"
	"fmt"
	"os"
	"strconv"
	"github.com/singularian/mdencode/code/decode/mdUnzipFile"
)

// mdencode flag struct                       
type FlagData struct {
	inputFilename string
	outputFilename string
	// thread parameters
        // threadCount  int64
        threadCount  string 
        thread       int64
        threadStart int64
        threadEnd   int64
}

func main() {

	md()
        os.Exit(0)

}


// mdunzip file
func md() int {

	var argsNumber int = len(os.Args)

	fd := new(FlagData)

	flag.StringVar(&fd.inputFilename, "file", "", "Input Filename")
	flag.StringVar(&fd.outputFilename, "out", "", "Output Filename")
        flag.StringVar(&fd.threadCount, "thread", "16", "Go Routine Threadsize")
        // flag.Int64Var(&fd.threadStart, "start", 0, "Thread Start (Allows threads to be skipped for multiple computers)")
        // flag.Int64Var(&fd.threadEnd, "end", 0, "Thread End (Allows threads to be skipped for multiple computers)")

	flag.Usage = printUsage
	flag.Parse()

	if argsNumber == 1 {
                flag.Usage()
                os.Exit(1)
        }


	// unzipFile := new(mdUnzipFile)
	unzipFile := mdUnzipFile.Init()

	threads, err := strconv.ParseInt(fd.threadCount, 10, 64)
	if err != nil {
		fmt.Printf("Invalid Threads Argument ", err, threads)
		os.Exit(0)
	}
	unzipFile.DecodeFile(fd.inputFilename, fd.outputFilename, uint64(threads))

	return 0
}

// printUsage
func printUsage() {
        fmt.Printf("USAGE of %s:\n", os.Args[0])
        fmt.Println(`
  -file string
        Input Filename
  -out string
        Output Filename
  -thread string
        Go Routine Threadsize
    `)

	fmt.Println()

}
