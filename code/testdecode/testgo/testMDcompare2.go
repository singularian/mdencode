package main

import (
    "fmt"
   //  "encoding/binary"
"crypto/md5"
"io"
"bytes"

)

func main() {


	h := md5.New()
	io.WriteString(h, "ZX")
	// fmt.Printf("%x: %s\n", string(h.Sum(nil)), "ZX")

	h2 := md5.New()
	io.WriteString(h2, "ZX")

	// var aba = bytes.Compare(h[:], h2[:]) 
	fmt.Println(bytes.Equal(h.Sum(nil), h2.Sum(nil)))
	// fmt.Println("result ", aba)



}
