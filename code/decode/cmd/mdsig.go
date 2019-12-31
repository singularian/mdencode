package main

import (
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
)


func main() {


	fmt.Println("Current mdencode signatures\n")


	mdl := mdBinaryList.Init()
/* 
	hlist := mdl.GetHashList()

	for i := 0; i < len(hlist); i++ {
		number := fmt.Sprintf("%d:", i+1)
		fmt.Printf("%-3s %s\n", number, hlist[i])
	}
*/

	hlistObject := mdl.GetHashListObject()
	for i := 0; i < len(hlistObject); i++ {
		number := fmt.Sprintf("%d:", i+1)
		fmt.Printf("%-3s %-11s - %s\n", number, hlistObject[i].HashName,  hlistObject[i].Description)
	}

}
