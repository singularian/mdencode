package main

import (
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdZipFile"
)

// TestList Struct
type TestList struct {
	key string
	match bool
}

var tlist = []TestList{
	{ key: "aes:F9382498734298",	match: true },
	{ key: "aes:F9382498734298,",	match: true },
	{ key: "aes:F9382498734298,ax:90238409394393",	match: true },
	{ key: "aes:F9382498734298,ax:90238409394393,",	match: true },
	{ key: "aes:F9382498734298&&,ax:90238409394393,",	match: false },
	{ key: "aes:F9382498734298&&,ax:90238409394393,",	match: false },
	{ key: "aes:F9382498734298ZZZ,ax:90238409394393,",	match: false },

}

func main() {

	var keylist = "aes:F9382498734298"

	// test the mdzip file key list
	mdz := mdZipFile.Init()

	res := mdz.SetKeyList(keylist)

	var length = len(tlist)
        for i := 0; i < length; i++ {
                var keylist  = tlist[i].key
                var m        = tlist[i].match
		res = mdz.SetKeyList(keylist)
		if m == res {
			fmt.Println("Good Test ", keylist, " ", m, res)
		} else {
			fmt.Println("Failed Test ", keylist, " ", m, res)
		}
	}
}
