package main

// mdencode project
// 
// mdsig.go
// 
// this displays the signature list 
//
// https://github.com/singularian/mdencode
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"flag"
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
)


func main() {

	var count int
	flag.IntVar(&count, "f", 0, "MDSig Format")
	flag.Parse()

	// fmt.Println("format ", count)

	if count == 0 {
		full()
	} else {
		simple()
	}
	// add a mark down table option

}

func simple() {

        mdl := mdBinaryList.Init()

	fmt.Println("Current mdencode signatures\n")

        columns := [5]string{"ID:", "HashName", "Description", "Block Size"}
        fmt.Printf("%-6s %-11s   %-40s\n", columns[0], columns[1], columns[2])
        fmt.Printf("=======================================================================\n")

        hlistObject := mdl.GetHashListObject()
        for i := 0; i < len(hlistObject); i++ {
                number := fmt.Sprintf("%d:", i+1)

                fmt.Printf("%-6s %-11s   %-40s\n", number, hlistObject[i].HashName,  hlistObject[i].Description)
        }


}


func full() {

        mdl := mdBinaryList.Init()

	fmt.Println("Current mdencode signatures\n")

        columns := [5]string{"ID:", "HashName", "Description", "Block Size"}
        fmt.Printf("%-6s %-11s   %-40s %s\n", columns[0], columns[1], columns[2], columns[3])
        fmt.Printf("=======================================================================\n")

        hlistObject := mdl.GetHashListObject()
        for i := 0; i < len(hlistObject); i++ {
                number := fmt.Sprintf("%d:", i+1)

                fmt.Printf("%-6s %-11s   %-40s %d\n", number, hlistObject[i].HashName,  hlistObject[i].Description, hlistObject[i].BlockSize)
        }



}
