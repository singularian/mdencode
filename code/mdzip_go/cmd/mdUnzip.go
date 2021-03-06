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
	"github.com/singularian/mdencode/code/mdzip_go/mdUnzipFile"
)

// mdencode flag struct                       
type FlagData struct {
	inputFilename string
	outputFilename string
	// thread parameters
        thread       int64
        threadStart int64
        threadEnd   int64
	// file post validation
	postval bool
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
	flag.Int64Var(&fd.thread, "thread", 16, "Go Routine Threadsize")
        // flag.Int64Var(&fd.threadStart, "start", 0, "Thread Start (Allows threads to be skipped for multiple computers)")
        // flag.Int64Var(&fd.threadEnd, "end", 0, "Thread End (Allows threads to be skipped for multiple computers)")
	flag.BoolVar(&fd.postval, "val", false, "Run the File Hash List Post Validation")

	flag.Usage = printUsage
	flag.Parse()

	if argsNumber == 1 {
                flag.Usage()
                os.Exit(0)
        }

	// if the thread is zero set it to the default
	if fd.thread == 0 {
		fd.thread = 16
	}

	// initialize the unzipFile object
	// unzipFile := new(mdUnzipFile)
	unzipFile := mdUnzipFile.Init(fd.inputFilename, fd.outputFilename, uint64(fd.thread), fd.postval)

	// decode the file
	unzipFile.DecodeFile()

	// validate the output file
	unzipFile.PostValidate()

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
  -val bool
        Run the File Hash List Post Validation
    `)

	fmt.Println()

        fmt.Println("Examples:")
        fmt.Println("mdzip -mod=64 -block=12 -file=randomfile -out=randomfile.mdz -bh=000000000001001 -fh=111")
        fmt.Println("mdzip -mod=64 -block=11 -file=randomfile -out=randomfile.mdz -bh=0000000000000000001 -fh=1111111111 -keylist=hw64:11111111111111111111111111111111FC00FD0033FD22FF990F0C0D0F0FF00E")

	fmt.Println("mdunzip -file=randomfile.mdz -out=randomfile.mdz.out")
	fmt.Println("mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -val")

	fmt.Println()

}
