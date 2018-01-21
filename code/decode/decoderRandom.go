
package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import ( 
	"crypto/rand"
	"fmt"
	"encoding/hex"
        _ "golang.org/x/crypto/md4"
        "crypto/md5"
        "crypto/sha1"
        _ "crypto/sha256"
        _ "crypto/sha512"
        "math/big"
	"strconv"
	"os"
	"log"
	"time"
)

// generates a random n byte array and then hashes it
// it then runs it through the modulus scan
func main() {

	modulusScanRandom(6)

	// 8 bytes ran in 24 hours
	//	decode("8", "24", "61", "7544937", "b597b21cd5ddcde0944cc7734d2f5da9", "19cfdd42d9389ee1a7709194020ce055e2493e05")
	os.Exit(0)
}

// run a modulus scan on a random byte array
func modulusScanRandom(blockSize int) {
        // create a random 6 byte array
        bytes := make([]byte, blockSize)
        _, _ = rand.Read(bytes)

        // process the modulus bitsize argument
        var modSize = "32"
        bitsize, _ := strconv.ParseInt(modSize, 10, 64)

        // create the modulus bigint 2 to the bitsize exponent
        var modulusBigInt, e = big.NewInt(2), big.NewInt(bitsize)
        modulusBigInt = modulusBigInt.Exp(modulusBigInt, e, nil)
        var modulusBigIntString = modulusBigInt.String()

        // create the biginteger representation of the bytes
        blockBigInt := new(big.Int)
        blockBigInt.SetBytes(bytes)

        // calculate the modulus remainder
        fileblockmodulus := new(big.Int)
        fileblockmodulus = fileblockmodulus.Mod(blockBigInt, modulusBigInt)
        var blockmod = fileblockmodulus.String()

        // calculate the modulus exponent
        two := big.NewInt(2)
        modexp := logN(blockBigInt, two)
        s := strconv.Itoa(modexp)

	// create a sha1 hash of the bytes
        h := sha1.New()
        h.Write([]byte(bytes))
        var shasum =  hex.EncodeToString(h.Sum(nil))

	// create an md5 hash of the bytes
        md5hash := md5.New()
        md5hash.Write([]byte(bytes))
        var md5sum = hex.EncodeToString(md5hash.Sum(nil))

        fmt.Println("random bytes ", bytes)
        fmt.Println("modulus size bits ", bitsize)
        fmt.Println("byte modulus ", modulusBigIntString)
        fmt.Println("block modulus ", blockmod)
        fmt.Println("modulus exponent ", modexp)

        fmt.Println("shasum ", shasum);
        fmt.Println("md5sum ", md5sum);


        decode("6", modSize, s, blockmod, md5sum, shasum)


}

func decode(blockSize string, modbitSize string, modexp string, remainder string, md5hex string, sha1hex string) int {


        var hashone string  = md5hex
        var hashtwo string  = sha1hex

	//create log file with desired read/write permissions
        f, err := os.OpenFile("decoderRandom.log", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
        if err != nil {
                log.Fatal(err)
        }
        // defer to close when you're done with it, not because you think it's idiomatic!
        defer f.Close()
        // set output of logs to f
        log.SetOutput(f)

	start := time.Now()
	log.Println("Starting decoderRandom")

        blocksize, _ := strconv.ParseInt(blockSize, 10, 64)
        buf := make([]byte, blocksize)

        // process the modulus bitsize argument
        // raise 2 to the bitsize exponent
        bitsize, _ := strconv.ParseInt(modbitSize, 10, 64)
        var i, e = big.NewInt(2), big.NewInt(bitsize)
        i.Exp(i, e, nil)
        modulostr := fmt.Sprint(i)
        fmt.Println("modulo bigint ", modulostr)
	log.Println("modulo bigint", modulostr)

        // module exponent floor and ceiling
        // 2 to the power 1 number less than greater than the block number
	floor, _ := strconv.ParseInt(modexp, 10, 64)
	ceil := floor + 1
        var modfloor, modexponent = big.NewInt(2), big.NewInt(floor)
        var modceiltwo, modceil  = big.NewInt(2),  big.NewInt(ceil)
        modfloor.Exp(modfloor, modexponent, nil)
        modceil.Exp(modceiltwo, modceil, nil)
        modulofloorstr :=  fmt.Sprint(modfloor)
        moduloceilstr :=  fmt.Sprint(modceil)

        fmt.Println("modulo floor ", modulofloorstr)
        fmt.Println("modulo ceil ", moduloceilstr)
	log.Println("modulo floor ", modulofloorstr, " ceil ", moduloceilstr)

	// calcluate the 2^exp mod floor
	mod := new(big.Int)
        mod = modfloor.Mod(modfloor, i);
	mod = mod.Sub(modfloor, mod)


        // fmt.Println("buf ", buf)

        // process the modulus bitsize argument
        // exponent
        // bitsize, err := strconv.ParseInt(modSize, 10, 64)
        // var i, e = big.NewInt(2), big.NewInt(bitsize)
        // i.Exp(i, e, nil)

        // mod := new(big.Int)
        // mod = z.Mod(z, i);

        // start with remainder
        remainderString, _ := strconv.ParseInt(remainder, 10, 64)
        modstart := big.NewInt(remainderString);

	log.Println("modulo remainder ", modstart)

	// add the modular floor two to the exponent mod modulus
	modstart = modstart.Add(modstart, mod)


        md5  := md5.New()
        sha1 := sha1.New()


	var lineCount uint64 = 1
        for {
		bigbytes := modstart.Bytes()

                copy(buf[:], bigbytes) 
		// fmt.Println("bigint ", buf, " ", bigbytes)

                md5.Write([]byte(buf))
		md5string := hex.EncodeToString(md5.Sum(nil))

		if md5string == hashone {
			sha1.Write([]byte(buf))
			sha1string := hex.EncodeToString(sha1.Sum(nil))
			// fmt.Println("md5 sha1 ", hex.EncodeToString(md5.Sum(nil)), " ", hex.EncodeToString(sha1.Sum(nil)))

			if md5string == hashone && sha1string == hashtwo {
				fmt.Println("Found Block ", buf)
				log.Println("Found Block ", buf)
				break
			}
			sha1.Reset()
		}
                md5.Reset()

                modstart := modstart.Add(modstart, i)
                gt := modstart.Cmp(modceiltwo)

                if gt < 0 {
                       break
                }

		if lineCount >= 1000000 {
			log.Println("blockbuffer value ", modstart)
			lineCount = 0
		}

		lineCount++

        }

	t := time.Now()
        elapsed := t.Sub(start)
	fmt.Println("total time ", elapsed)
	log.Println("Total time ", elapsed)

	return 0
}

func logN(fileblockint *big.Int, base *big.Int) int {

        var exponent int = 1
        gt := fileblockint.Cmp(base)

        if gt < 0 {
                return 0
        }
        fileblockintcopy := big.NewInt(0)
        fileblockintcopy.Add(fileblockintcopy, fileblockint)
        for {
                fileblockintcopy.Quo(fileblockintcopy, base)
                gt = fileblockintcopy.Cmp(base)

                if gt < 0 {
                        break
                }
                exponent = exponent + 1
        }

        return exponent

}
