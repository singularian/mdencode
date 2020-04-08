package main

import (
	"fmt"
	_"encoding/hex"
	"github.com/singularian/mdencode/code/decode/mdSignatureList"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
)

// HashList Struct         
type TestList struct {
        HashName    string
        Description string
        BlockSize int
        Key bool
}

func main() {

	// test the md hash context list
	mdc := mdHashContextList.Init()
	// mdc.CreateHashListMap("blake2:md5:sha1", 1, 1)
	// mdc.CalcHashBlockSize("blake2:md5:sha1", 1)
	mdc.CreateHashListMap("md4:md5:sha1", 1, 1)
	mdc.CalcHashBlockSize("md4:md5:sha1", 1)

	// convert the signature list to a name list
	mdl := mdSignatureList.Init()
	// l  := mdl.CreateHashBlockList("11111111111111111111111111111111111111111111")
	var csvstring = "1,2,3,4,5-6"
	l  := mdl.CreateHashBlockListCSV(csvstring)
	fmt.Println("hash list ", csvstring, l, len(l))

	fmt.Println("test2")
	csvstring = "1,11,23,44"
	list  := mdl.CreateHashBlockListCSV(csvstring)
	fmt.Println("hash list ", csvstring, list, len(list))

	fmt.Println("test3")
	csvstring = "1,11"
	list2 := mdl.CreateHashBlockListCSV(csvstring)
	fmt.Println("hash list ", csvstring, list2, len(list2))

	fmt.Println("test4")
	csvstring = "1-91"
	list3 := mdl.CreateHashBlockListCSV(csvstring)
	fmt.Println("hash list ", csvstring, list3, len(list3))

        fmt.Println("test4")
        csvstring = "1-91,80,80,4,3"
        list3 = mdl.CreateHashBlockListCSV(csvstring)
        fmt.Println("hash list ", csvstring, list3, len(list3))

	fmt.Println("test4")
	csvstring = "1,11,aaaa,"
	list4 := mdl.CreateHashBlockListCSV(csvstring)
	fmt.Println("hash list ", csvstring, list4, len(list4))


}
