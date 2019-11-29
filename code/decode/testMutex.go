package main

import (
	"fmt"
//    "math/rand"
//	"sync"
//    "sync/atomic"
//	"time"
	"github.com/singularian/mdencode/code/decode/mdUnzipFileMutex"
)
func main() {


	//var mutex = &sync.Mutex{}
	b := []byte{1, 2, 3, 0, 0, 0}
	mutex := mdUnzipFileMutex.Init()
	mutex.SetFileBuffer(1, b)
	fmt.Println("mutex ", mutex.GetMatchStatus(), " ", mutex.GetFileBuffer())






}
