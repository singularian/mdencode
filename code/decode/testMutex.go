package main

import (
_	"crypto/hmac"
_	"crypto/md5"
_	"github.com/singularian/mdencode/code/hash/sha1_128"
_	"crypto/sha1"
_	"crypto/sha256"
_	"crypto/sha512"
_	"github.com/codahale/blake2"
	"fmt"
//    "math/rand"
//	"sync"
//    "sync/atomic"
//	"time"
	"github.com/singularian/mdencode/code/decode/mdUnzipFileMutex"
	"github.com/singularian/mdencode/code/decode/mdBlockSize"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
_	"github.com/codahale/blake2"
_	"golang.org/x/crypto/md4"
_	"hash/fnv"
_	"github.com/aead/skein"
_	"github.com/asadmshah/murmur3"
/*	"github.com/cxmcc/tiger"
	"github.com/dchest/siphash"
	"github.com/jzelinskie/whirlpool"
	"github.com/steakknife/keccak"
	"golang.org/x/crypto/blake2s"
	"golang.org/x/crypto/ripemd160"
	"golang.org/x/crypto/sha3"
*/
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
	var defaultkey = []byte("LomaLindaSanSerento9000")
	// for _, hashvalue := range l.blockHashListNames {
	// for context, hashvalue := range mdc.HashListBlocks {
	// for hashkey, hashvalue := range mdc.HashListBlocks {
	for hashkey, hashvalue := range mdc.HashListBlocks {
		fmt.Println("hash ", hashkey)
		hashvalue.Write(defaultkey)
		// h := l.hashListBlocks[hashvalue]
		///// h := mdc.HashListBlocks[hashvalue]
		// h := context
		// h.Write([]byte(l.filebuffer))
		///h.Write(defaultkey)
		fmt.Printf("hash %s bytes hex % x\n", hashkey, hashvalue.Sum(nil))
		hashvalue.Reset()

	}
}
