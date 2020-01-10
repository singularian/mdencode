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

	hexnum := sigRand.ConvertDecHexString2Int("0D0D0D0FFF")
	decnum := sigRand.ConvertDecHexString2Int("12893712837")

	fmt.Println("hexnum ", hexnum, " decnum ", decnum)
}
