package main

import (
	"os"
	// "fmt"
        "github.com/singularian/mdencode_private/code/encode/mdFormats/mdFormatSQL"
)


func main() {

	var filename string = "newmd.db" 
        var argsNumber int = len(os.Args)
        // var mdname = os.Args[0]

        // sqlite3 db file signature generator
	if argsNumber == 2 {
                filename := os.Args[1]
                /* if _, err := os.Stat(filename); os.IsNotExist(err) {
                        ("The mdfile db does not exist")
                        os.Exit(3)
                }*/
                // fdata := fileData{}
                // fdata.md = mdFormatSQL.Init(0, filename, "", 0, 0, 0, filename)
                // fdata.md.InitFile()
		// fdata.md.EncodeEndFile(format)
		md := mdFormatSQL.Init(0, filename, "", 0, 0, 0, "", "", filename)
		md.InitFile()
		md.EncodeEndFile(0)
        } else {
		/* fdata := fileData{}
                fdata.md = mdFormatSQL.Init(0, filename, "", 0, 0, 0, defaultDBname)
                fdata.md.InitFile()
		fdata.md.EncodeEndFile(0)
		*/
		md := mdFormatSQL.Init(0, filename, "", 0, 0, 0, "", "", filename)
		md.InitFile()
		md.EncodeEndFile(0)
	}

        os.Exit(0)
}
