package main

import (
    "fmt"
    "strings"
   //  "encoding/binary"
"encoding/json"
"crypto/md5"
"io"
"bytes"

)

func main() {

    // stringSlice := []string{"hello", "world"}
    stringSlice := []string{"12", "13"}

    stringByte := strings.Join(stringSlice, " ")

    // Byte array value
    fmt.Println([]byte(stringByte))

    // Corresponding string value
    fmt.Println(string([]byte(stringByte)))
    fmt.Println([]byte(stringByte))

    bs := []byte(fmt.Sprintf("%b", 7))
    fmt.Println("lksjdfklsjfsdlkfjlskdfj ", bs)

// b := make([]byte, 8)
//binary.LittleEndian.(b, uint8(8))

//fmt.Println(b)


 str := "[2,15,23]"
    var ints []byte
    _ = json.Unmarshal([]byte(str), &ints)
 //   if err != nil {
 //       log.Fatal(err)
  //  }
    fmt.Printf("%v", ints)


	h := md5.New()
	io.WriteString(h, "ZX")
	fmt.Printf("%x: %s\n", string(h.Sum(nil)), "ZX")

	h2 := md5.New()
	io.WriteString(h2, "ZX")

	// var aba = bytes.Compare(h[:], h2[:]) 
	fmt.Println(bytes.Equal(h.Sum(nil), h2.Sum(nil)))
	// fmt.Println("result ", aba)


	// if "7d40f0d6c4baa81950b1759d3712a225" == string(h.Sum(nil)) {
	if h == h2 {
		fmt.Printf("compare worked")
	} else {
		fmt.Printf("Compare did not work")
	}



}
