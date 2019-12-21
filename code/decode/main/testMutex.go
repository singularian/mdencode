package main

import (
//	"os"
	"fmt"
	"encoding/hex"
//    "math/rand"
//	"sync"
//    "sync/atomic"
//	"time"
	"github.com/singularian/mdencode/code/decode/mdUnzipMutex"
	"github.com/singularian/mdencode/code/decode/mdBlockSize"
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

	// test the block size calculator
	mdBlock := mdBlockSize.Init()
	var blocksize uint64 = 0
        var s []int
	blocksize, s = mdBlock.CalcHashBlockSize("blake2:md5:sha1")
	fmt.Println("mdBlocksize ", s, blocksize)

	// test the md hash context list
	mdc := mdHashContextList.Init()
	// mdc.CreateHashListMap("blake2:md5:sha1", 1, 1)
	// mdc.CalcHashBlockSize("blake2:md5:sha1", 1)
	mdc.CreateHashListMap("md4:md5:sha1", 1, 1)
	mdc.CalcHashBlockSize("md4:md5:sha1", 1)
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

	// for hashkey, hashvalue := range mdc.HashListBlocks {
	mm  := mdc.GetBlockHash()
	fmt.Println("sssss ", mm)
	// for _, hashvalue := range mdc.blockHashListNames {
	for _, hashvalue := range mm {
		fmt.Println("hash ", hashvalue)
		h := mdc.HashListBlocks[hashvalue]
		// h := [hashvalue]
		// ===========================
		h.Write([]byte(c))
		// ===========================
		bytestring += fmt.Sprintf("%x", h.Sum(nil))
		fmt.Printf("hash %s %x\n", hashvalue, h.Sum(nil))
		h.Reset()
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
	if mdc.CheckFileHashBlock(b) {
		fmt.Println("\nResult = true")
	}
	fmt.Println("")

	// convert binary list to a name list
	mdl := mdBinaryList.Init()
	l  := mdl.CreateHashBlockList("11111111111111111111111111111111111111111111")
	fmt.Println("hash list ", l, len(l))

	fmt.Println("test2")
	list  := mdl.CreateHashBlockList("111")
	fmt.Println("hash list ", list, len(list))

	fmt.Println("test3")
	list2 := mdl.CreateHashBlockList("1000011")
	fmt.Println("hash list ", list2, len(list2))

	hlist := mdl.GetHashList()

	for i := 0; i < len(hlist); i++ {
		fmt.Printf("%3d: %s\n", i+1, hlist[i])
	}

	// _,_  = mdBlock.CreateHashBlockList("00110001110011")

}
