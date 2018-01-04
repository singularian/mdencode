package main

import (
	"os"
	// "fmt"
        "github.com/singularian/mdencode/code/encode/mdFormats/mdFormatSQL"
)


func main() {

	var filename string = "newmd.db" 
        var argsNumber int = len(os.Args)
        // var mdname = os.Args[0]

        // create a new sqlite3 db file signature with the argument filename
	if argsNumber == 2 {
                filename := os.Args[1]

		md := mdFormatSQL.Init(0, filename, "", 0, 0, 0, "", "", filename)
		md.InitFile()
		md.EncodeEndFile(0)
	// create a db file signature with the default name
        } else {
		md := mdFormatSQL.Init(0, filename, "", 0, 0, 0, "", "", filename)
		md.InitFile()
		md.EncodeEndFile(0)
	}

        os.Exit(0)
}
