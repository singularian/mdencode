package main

import (
	"fmt"
	"github.com/singularian/mdencode/code/decode/sigRand"
)
func main() {

	m, _ := sigRand.GenerateRandomBytes(32)
	xint, _ := sigRand.RandUint64()
	str, _ := sigRand.GenerateRandomString(32)
	hw, _ := sigRand.GenRandomHighwayKey()

	fmt.Println("key ", m)
	fmt.Println("int64 ", xint)
	fmt.Println("str ", str)
	fmt.Println("hw ", hw)

}
