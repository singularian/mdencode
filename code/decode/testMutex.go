package main

import (
//	"os"
	"fmt"
	"encoding/hex"
//    "math/rand"
//	"sync"
//    "sync/atomic"
//	"time"
	"github.com/singularian/mdencode/code/decode/mdUnzipFileMutex"
	"github.com/singularian/mdencode/code/decode/mdBlockSize"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
)
func main() {

	// test the file mutex
	b := []byte{1, 2, 3, 0, 0, 0}
	mutex := mdUnzipFileMutex.Init()
	mutex.SetFileBuffer(1, b)
	fmt.Println("mutex ", mutex.GetMatchStatus(), " ", mutex.GetFileBuffer())

	// test the block size calculator
	mdBlock := mdBlockSize.Init()
	var blocksize uint64 = 0
        var s []int
	blocksize, s = mdBlock.CalcHashBlockSize("blake2:md5:sha1")
	fmt.Println("mdBlocksize ", s, blocksize)

	// test the md hash context list
	mdc := mdHashContextList.Init()
	mdc.CreateHashListMap("blake2:md5:sha1", 1, 1)
	mdc.CalcHashBlockSize("blake2:md5:sha1", 1)
	// var defaultkey = []byte("LomaLindaSanSerento9000")
	// for _, hashvalue := range l.blockHashListNames {
	// for context, hashvalue := range mdc.HashListBlocks {
	// for hashkey, hashvalue := range mdc.HashListBlocks {
	var bytestring = ""
	// for hashkey, hashvalue := range mdc.HashListBlocks {
	for hashkey, _ := range mdc.HashListBlocks {
		fmt.Println("hash ", hashkey)
/*		hashvalue.Write(defaultkey)
		// h := l.hashListBlocks[hashvalue]
		///// h := mdc.HashListBlocks[hashvalue]
		// h := context
		// h.Write([]byte(l.filebuffer))
		///h.Write(defaultkey)
		fmt.Printf("hash %s bytes hex %x\n", hashkey, hashvalue.Sum(nil))
		bytestring += fmt.Sprintf("%x", hashvalue.Sum(nil))
		hashvalue.Reset()
*/
	}

	for hashkey, hashvalue := range mdc.HashListBlocks {
		fmt.Println("hash ", hashkey)
		hashvalue.Write([]byte(b))
		bytestring += fmt.Sprintf("%x", hashvalue.Sum(nil))
		fmt.Printf("hash %s %x\n", hashkey, hashvalue.Sum(nil))
		hashvalue.Reset()
	}
	// os.Exit(0)

	bsrc := []byte(bytestring)
	bdst := make([]byte, hex.DecodedLen(len(bsrc)))
	nx, err := hex.Decode(bdst, bsrc)
	
	// mdc.SetBlockHash(bdst[:nx])
	mdc.SetBlockHash(bdst[:nx])

	fmt.Printf("%x\n%s\n", bdst, bytestring)

	// const hexstring = "48656c6c6f20476f7068657221"
	// decoded, err := hex.DecodeString(hexstring)
	src := []byte("48656c6c6f20476f7068657221")
	dst := make([]byte, hex.DecodedLen(len(src)))
	n, err := hex.Decode(dst, src)
	if err != nil {
		fmt.Println(err)
	}
	fmt.Printf("% x\n", dst[:n])

	fmt.Println("\ntest b")
	mdc.CheckFileHashBlock(b)

	fmt.Println("\ntest defaultkey")
	mdc.CheckFileHashBlock(b)

}
