package main

import (
_	"bufio"
_	"fmt"
_	"io/ioutil"
	"os"
	"encoding/binary"
	"math/big"
)

func check(e error) {
	if e != nil {
		panic(e)
	}
}

func main() {

	f, err := os.Create("dat2")
	check(err)

	defer f.Close()

	//d2 := []byte{15, 11, 119, 121, 10}
	//n2, err := f.Write(d2)
	//check(err)
	//fmt.Printf("wrote %d bytes\n", n2)

	var number uint32 = 1294
	// var number  = uint32(modExp)
        // fmt.Print("Modexp \n", modExp, number)
        // fmt.Printf("hex num %x \n", number)
        err = binary.Write(f, binary.BigEndian, &number)

	var bignum, ok = new(big.Int).SetString("218882428714186575617", 0)
	err = binary.Write(f, binary.BigEndian, &bignum)

}
