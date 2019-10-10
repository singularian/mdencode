package main

import (
	"fmt"
	// "strconv"
	//"bytes"
)

// check copying bytes of differing length
func main() {

	a := []byte{ 0, 10, 22, 38, 240, 171, 146, 123 }
	b := make([]byte, 13)
        // copy(b[5:13], a[:])
	copy(b[5:13], a)

		
	fmt.Println(a)
	fmt.Println(b)
	fmt.Println("len ", len(b), len(a))
}

