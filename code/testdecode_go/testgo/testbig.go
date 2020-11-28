package main

import (
	"fmt"
//	"bytes"
	"math/big"
)


func main() {


	ten := big.NewInt(10)


	fmt.Println("d ", ten)

	// bytestring := fmt.Sprintf("%v", md.byteblock)

	b := big.NewInt(0)
	// b, _ = b.SetString("1000000000000000000000000000000", 10)
	b, _ = b.SetString("18446744073709551615", 10)
	// 1067581114597973269817944
	// 2019/02/05 23:25:50 byte block [0 226 17 176 45 15 177 171 142 42 88]
	b, _ = b.SetString("1067581114597973269817944", 10)
	fmt.Println("b", b.Bytes())

}
