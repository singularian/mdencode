package mdEncodeALL_test

import (
	"fmt"
	"math/big"
	"testing"
	"github.com/singularian/mdencode/code/encode/mdEncode/mdEncodeALL"
)

func TestString(t *testing.T) {
	fmt.Println("go tests")
	md := mdEncodeALL.Init()
	md.SetByteBlock(true)

/*	two := big.NewInt(2)
        modstart := new(big.Int)
	var remainder = "1800000"
        modstart.SetString(remainder, 10) 
*/	// var aba = md.logN(modstart, two)
	// var mode = md.PrintType()
	// var mode = "aaaaa"
	// fmt.Println("test ", mode)

	/* if mode != "A" {
        	t.Errorf("blah want %q", mode)
	} */
}

func TestLogN(t *testing.T) {
	two := big.NewInt(2)
	modstart := new(big.Int)
	var remainder = "1800000"
	modstart.SetString(remainder, 10) 

	var exponent int = 1
	fileblockintcopy := big.NewInt(0)
	fileblockintcopy.Add(fileblockintcopy, modstart)
        for {
                fileblockintcopy.Quo(fileblockintcopy, two)
                gt := fileblockintcopy.Cmp(two)

                if gt < 0 {
                        break
                }
                exponent = exponent + 1
        }

	if exponent != 20 {
		t.Errorf("blah want %d", exponent)
	}



}

func TestLogN2(t *testing.T) {
        two := big.NewInt(2)
        modstart := new(big.Int)
        var remainder = "20000000000"
        modstart.SetString(remainder, 10) 

        var exponent int = 1
        fileblockintcopy := big.NewInt(0)
        fileblockintcopy.Add(fileblockintcopy, modstart)
        for {
                fileblockintcopy.Quo(fileblockintcopy, two)
                gt := fileblockintcopy.Cmp(two)

                if gt < 0 {
                        break
                }
                exponent = exponent + 1
        }

        if exponent != 34 {
                t.Errorf("blah want %d", exponent)
        }



}


