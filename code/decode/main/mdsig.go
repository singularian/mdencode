package main

import (
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
)


func main() {


	fmt.Println("Current mdencode signatures")


	mdl := mdBinaryList.Init()
	hlist := mdl.GetHashList()

        for i := 0; i < len(hlist); i++ {
                fmt.Printf("%3d: %s\n", i+1, hlist[i])
        }


}
