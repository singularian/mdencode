package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"flag"
	_ "log"
	"os"
	"github.com/singularian/mdencode/code/mdencode/mdReports/mdReportsSQL"
)

func main() {

	// mddbreport [sqllite db] [number] [format]
	var argsNumber int = len(os.Args)
	argsReport(argsNumber)

	os.Exit(0)

}


func argsReport(argsNumber int) {

	var defaultFormat = 9 

	// var blocksize uint64 
	// var modsize uint64 
	// var appendfile bool
	// var byteblock bool
	// var byteblockint bool
	// var filehashline bool
	var filename string
	var fileid uint64 = 0
	// var fileid int = 0 
	var dumpAll bool
	var outfilename string
	var appendfile bool
	// logfilename
	// var logfilename string
	// initialize sqlite3 md db
	// var initdb string

	flag.IntVar(&defaultFormat, "format", 4, "Output Format")
	flag.StringVar(&filename, "file", "", "SQLite3 DB Signature Filename")
	flag.Uint64Var(&fileid, "fileid", 0, "Fileid")
	flag.BoolVar(&dumpAll, "dumpall", false, "Dump all the sql file signatures")
	flag.StringVar(&outfilename, "out", "", "Output Filename")
	flag.BoolVar(&appendfile, "append", false, "Append To Output File")

	flag.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage of %s:\n", os.Args[0])
		flag.PrintDefaults()
		fmt.Println()
		fmt.Println("Version: 1.0 復甦 復活\n")
	}

	flag.Parse()

	if argsNumber == 1 { 
		flag.Usage()

		fmt.Println("Examples:")
		fmt.Println("mddbreport -file=md.db -fileid=1 -format=10")
		fmt.Println("mddbreport -file=md.db -fileid=1 -format=10 -out=outputfile.txt -append=true")
		fmt.Println("mddbreport -file=md.db -fileid=1 -format=5000 -out=outputfile.xml -append=false")
		os.Exit(1)
	}

	// if just the file is given display the file mdencode list
	// make 0 display everything
	if argsNumber == 2 {
		// filename := os.Args[1]
		if _, err := os.Stat(filename); os.IsNotExist(err) {
			// log.Fatal("The mdfile db does not exist")
			fmt.Println("The mdfile db does not exist")
			os.Exit(3)
		}
		// md := mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		md := mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", outfilename, false) // test for the output filename
		md.PrintFileList(1, 0)
		os.Exit(0)
	} else {
		// display the file from the db with the fileid
		if fileid > 0 {
			md := mdReportSQL.Init(defaultFormat, filename, "", 0, 0, 0, "0", "0", outfilename, appendfile)
			md.PrintReport(defaultFormat, "filename", fileid)
		// if the fileid is zero display everything with the format
		} else if fileid == 0 {
			md := mdReportSQL.Init(defaultFormat, filename, "", 0, 0, 0, "0", "0", outfilename, appendfile)
			md.PrintEntireFileList(defaultFormat)
		}

	}


}

