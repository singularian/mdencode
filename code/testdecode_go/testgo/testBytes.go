package main

import (
    "crypto/sha1"
    "crypto/md5"
    "fmt"
     "encoding/hex"    
)

func main() {

    var bytes = []byte{ 112, 97, 99, 107, 97, 103, 101, 32, 109, 97 }

    h := sha1.New()

    h.Write([]byte(bytes))

    bs := h.Sum(nil)

    hasher := md5.New()
    hasher.Write([]byte(bytes))
    fmt.Println("md5 ", hex.EncodeToString(hasher.Sum(nil)))

    fmt.Printf("sha %x\n", bs)
}
