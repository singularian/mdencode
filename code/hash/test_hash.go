package main

import (
	"fmt"
	"io"
	// "encoding/hex"
	"crypto/sha1"
	"github.com/singularian/mdencode/code/hash/sha1_128"
)

func main() {
	sha1 := sha1.New()

	teststring := "est of the SHA1 128 hash"

	io.WriteString(sha1, teststring)
	fmt.Printf("sha1 hex   % x\n", sha1.Sum(nil))

	sha128 := sha1_128.New(4, 20)
	io.WriteString(sha128, teststring)
	fmt.Printf("sha128 hex % x\n", sha128.Sum(nil))

	// fmt.Printf("hex ", hex.EncodeToString(h.Sum(nil)))
	sha128.Reset()
	sha1_1281 := sha1_128.New(0, 16)
	io.WriteString(sha1_1281, teststring)
	// sha1string := hex.EncodeToString(h.Sum(nil))
	fmt.Printf("sha1281 hex % x\n", sha1_1281.Sum(nil))

	sha1282 := sha1_128.New()
	io.WriteString(sha1282, "Test of the SHA1 128 hash")
	fmt.Printf("hex %x\n", sha1282.Sum(nil))


}

