package main

import (
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdUnzipMutex"
)
func main() {

	// test the file mutex
	b := []byte{1, 2, 3, 0, 10, 0, 11, 22}
	mutex := mdUnzipMutex.Init()
	mutex.SetFileBuffer(1, b)
	fmt.Println("mutex ", mutex.GetMatchStatus(), " ", mutex.GetFileBuffer())

}
