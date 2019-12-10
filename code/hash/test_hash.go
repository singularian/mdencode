package main

import (
	"fmt"
	"io"
	// "encoding/hex"
	"github.com/singularian/mdencode/code/hash/sha1_128"
)

func main() {
	h := sha1_128.New()
	io.WriteString(h, "Test of the SHA1 128 hash")
	fmt.Printf("% x\n", h.Sum(nil))

	// fmt.Printf("hex ", hex.EncodeToString(h.Sum(nil)))
	h.Reset()
	io.WriteString(h, " 'taint yours and 'taint mine.")
	// sha1string := hex.EncodeToString(h.Sum(nil))
	fmt.Printf("hex %x\n", h.Sum(nil))
}

