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
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
)


func main() {


	fmt.Println("Current mdencode signatures\n")

	full()

}

func simple() {

        mdl := mdBinaryList.Init()

        columns := [5]string{"ID:", "HashName", "Description", "Block Size"}
        fmt.Printf("%-6s %-11s   %-40s\n", columns[0], columns[1], columns[2])
        fmt.Printf("=======================================================================\n")

        hlistObject := mdl.GetHashListObject()
        for i := 0; i < len(hlistObject); i++ {
                number := fmt.Sprintf("%d:", i+1)

                fmt.Printf("%-6s %-11s   %-40s %d\n", number, hlistObject[i].HashName,  hlistObject[i].Description)
        }


}


func full() {

        mdl := mdBinaryList.Init()

        columns := [5]string{"ID:", "HashName", "Description", "Block Size"}
        fmt.Printf("%-6s %-11s   %-40s %s\n", columns[0], columns[1], columns[2], columns[3])
        fmt.Printf("=======================================================================\n")

        hlistObject := mdl.GetHashListObject()
        for i := 0; i < len(hlistObject); i++ {
                number := fmt.Sprintf("%d:", i+1)

                fmt.Printf("%-6s %-11s   %-40s %d\n", number, hlistObject[i].HashName,  hlistObject[i].Description, hlistObject[i].BlockSize)
        }



}
