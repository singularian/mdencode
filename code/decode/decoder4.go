
package main

import ( 
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
/*
Makefile
.
682:4:6:24:md5:sha1:2017-09-30 21:56:43.373118166 -0400 EDT
z: md5 ed9d7d0fc6e44b05994d71c443862a30
z: sha1 da39a3ee5e6b4b0d3255bfef95601890afd80709
buffer bytes  [35 32 84 104 105 115]   [35 32 84 104 105 115]   38621762054515
6:1a688e0466fbec570405a7b87fdb1e97:819aa5762ce7030c37a9b61ee8e5d687581ebc5c:45:6842739

8 byte block

Makefile
.
682:4:8:24:md5:sha1:2017-09-30 22:06:44.653143191 -0400 EDT
z: md5 ed9d7d0fc6e44b05994d71c443862a30
z: sha1 da39a3ee5e6b4b0d3255bfef95601890afd80709
buffer bytes  [35 32 84 104 105 115 32 105]   [35 32 84 104 105 115 32 105]   2531115798004703337
8:b597b21cd5ddcde0944cc7734d2f5da9:19cfdd42d9389ee1a7709194020ce055e2493e05:61:7544937
*/
func main() {


//	var hashone string  = "1a688e0466fbec570405a7b87fdb1e97"
//	var hashtwo string  = "819aa5762ce7030c37a9b61ee8e5d687581ebc5c"

	decode("6", "24", "45", "6842739", "1a688e0466fbec570405a7b87fdb1e97", "819aa5762ce7030c37a9b61ee8e5d687581ebc5c")
	// buffer bytes  [32 110 97 109 101 32]   [32 110 97 109 101 32]   35658453050656
	// 6:7d93680ecf5b2929037b94c64f9c1a1d:c6d3a02fc7ad2f253bc05758385a87bc5238de00:45:7169312
	decode("6", "24", "45", "7169312", "7d93680ecf5b2929037b94c64f9c1a1d", "c6d3a02fc7ad2f253bc05758385a87bc5238de00")
// ran in 24 hours
//	decode("8", "24", "61", "7544937", "b597b21cd5ddcde0944cc7734d2f5da9", "19cfdd42d9389ee1a7709194020ce055e2493e05")
	os.Exit(0)
}

func decode(blockSize string, modbitSize string, modexp string, remainder string, md5hex string, sha1hex string) int {


        var hashone string  = md5hex
        var hashtwo string  = sha1hex

	//create log file with desired read/write permissions
        f, err := os.OpenFile("decoder4.log", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
        if err != nil {
                log.Fatal(err)
        }
        // defer to close when you're done with it, not because you think it's idiomatic!
        defer f.Close()
        // set output of logs to f
        log.SetOutput(f)

	start := time.Now()
	log.Println("Starting decoder4")

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
				fmt.Println("Found Block ", buf, " ", bigbytes)
				log.Println("Found Block ", buf, " ", bigbytes)
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

