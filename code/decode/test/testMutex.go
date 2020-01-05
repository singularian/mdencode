package main

import (
	"fmt"
	"encoding/hex"
	"github.com/singularian/mdencode/code/decode/mdUnzipMutex"
	// "github.com/singularian/mdencode/code/decode/mdBlockSize"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
)
func main() {

	// test the file mutex
	b := []byte{1, 2, 3, 0, 10, 0, 11, 22}
	c := []byte{1, 2, 3, 0, 10, 0, 11, 22, 1, 1, 1}
	mutex := mdUnzipMutex.Init()
	mutex.SetFileBuffer(1, b)
	fmt.Println("mutex ", mutex.GetMatchStatus(), " ", mutex.GetFileBuffer())

}
