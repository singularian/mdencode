
package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import ( 
	// "bytes"
	"crypto/rand"
	"fmt"
	"encoding/hex"
        "crypto/md5"
        "crypto/sha1"
        "math/big"
	"strconv"
	"os"
	"log"
	"time"
)

// mdencode flag struct
type DecodeData struct {
	blocksize string
	modsize string
	modulusBigInt *big.Int
	blockBigInt *big.Int
	fileblockmodulus *big.Int
        // log writer
        log *log.Logger
}

// generates a random n byte array and then hashes it
// it then runs it through the modulus scan
func main() {

	var argsNumber int = len(os.Args)

	// TODO: there are still some bugs in the floor setter

	// test a random byte block
	// arguments blocksize bytes and modsize bits
	// more than 6 bytes takes a while 
        if argsNumber == 3 {
                blocksize  := os.Args[1]
                modsize    := os.Args[2]
		blocksizeint, _ := strconv.Atoi(blocksize)
		modulusScanRandom(blocksizeint, modsize)
        // test random
        } else {
		testmodulusScanRandom()
	}
	os.Exit(0)
}


// test method
func testmodulusScanRandom() {

	// fd := new(DecodeData)

        modulusScanRandom(6, "32")
        modulusScanRandom(6, "24")

        // 8 bytes ran in 24 hours
        //      decode("8", "24", "61", "7544937", "b597b21cd5ddcde0944cc7734d2f5da9", "19cfdd42d9389ee1a7709194020ce055e2493e05")
        // modulusScanRandom(8, "24")

}


// run a modulus scan on a random byte array
func modulusScanRandom(blockSize int, modSize string) {
        // create a random byte array
        bytes := make([]byte, blockSize)
        _, _ = rand.Read(bytes)
	blockSizeStr := strconv.Itoa(blockSize)

	// convert the bytes to a string
	bytestring := fmt.Sprintf("%v", bytes)

        // process the modulus bitsize argument
        bitsize, _ := strconv.ParseInt(modSize, 10, 64)

        // create the modulus bigint 2 to the bitsize exponent
        var modulusBigInt, e = big.NewInt(2), big.NewInt(bitsize)
        modulusBigInt = modulusBigInt.Exp(modulusBigInt, e, nil)
        var modulusBigIntString = modulusBigInt.String()

        // create the biginteger representation of the bytes
        blockBigInt := new(big.Int)
        blockBigInt.SetBytes(bytes)
	var blockBigIntstring = blockBigInt.String()

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

        fmt.Println("random ", blockSize, " bytes ", bytes, " ", blockBigIntstring)
        fmt.Println("modulus size bits ", bitsize)
        fmt.Println("byte modulus ", modulusBigIntString)
        fmt.Println("block modulus ", blockmod)
        fmt.Println("modulus exponent ", modexp)

        fmt.Println("shasum ", shasum);
        fmt.Println("md5sum ", md5sum);

        _, buffer := decode(blockSizeStr, modSize, s, blockmod, md5sum, shasum)

	if(bytestring == buffer) {
		fmt.Println("random bytestring and modulusscan bytestring match ", bytestring, " ", buffer)
		fmt.Println("\n")
	}

}

// calculate the byte block assicated with a blocksize and modulus and modulus exponent with a sha1 and md5 hash
// this will run the modulus scan decode
func decode(blockSize string, modbitSize string, modexp string, remainder string, md5hex string, sha1hex string) (int, string) {


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

        // calculate the module exponent floor and ceiling
	// the base in the exponent is 2 it can also be the modulus to some power less than the big block int
        // floor = 2 to the power
	// ceil = 2 to the power + 1 number less than greater than the block number
	floor, _ := strconv.ParseInt(modexp, 10, 64)
	ceil := floor + 1
        var modfloor, modexponent = big.NewInt(2), big.NewInt(floor)
        var modceiltwo, modceil  = big.NewInt(2),  big.NewInt(ceil)
        modfloor.Exp(modfloor, modexponent, nil)
        modceil.Exp(modceiltwo, modceil, nil)
        modulofloorstr :=  fmt.Sprint(modfloor)
        moduloceilstr :=  fmt.Sprint(modceil)

	fmt.Println("modulo floor ", modulofloorstr, " ceil ", moduloceilstr, " ", modceiltwo)
	log.Println("modulo floor ", modulofloorstr, " ceil ", moduloceilstr)

	// I think there is a bug here 
	// with the exponent

        // fmt.Println("buf ", buf)

        // process the modulus bitsize argument
        // exponent
        // bitsize, err := strconv.ParseInt(modSize, 10, 64)
        // var i, e = big.NewInt(2), big.NewInt(bitsize)
        // i.Exp(i, e, nil)

        // mod := new(big.Int)
        // mod = z.Mod(z, i);

	log.Println("modulo floor ", modexp, " ", modulofloorstr, " ceil ", moduloceilstr)
	fmt.Println("modulo floor UUUU ", modexp, " ", modulofloorstr, " ceil ", moduloceilstr, " mod ", i)

        // calcluate the 2^exp mod floor
	// this converts the 2^exponent to modulus*n for the modfloor + the remainder
	modremainder := new(big.Int)
	fmt.Println("modulo floor ", modfloor)
	modremainder = convertFloorBase2(modfloor, i)
	// os.Exit(0)
        
/*
        log.Println("modulo floor ", fmt.Sprint(modfloor))
        log.Println("modulo i ", fmt.Sprint(i))
        log.Println("modulo remainder ", fmt.Sprint(modremainder))

	fmt.Println("modulo floor ", fmt.Sprint(modfloor))
        fmt.Println("modulo i ", fmt.Sprint(i))
        fmt.Println("modulo remainder ", fmt.Sprint(modremainder))

        // start with remainder
        remainderString, _ := strconv.ParseInt(remainder, 10, 64)
        modstart := big.NewInt(remainderString);

	fmt.Println("modulo remainder ", modstart)
	log.Println("modulo remainder ", modstart)

	// add the modular floor two to the exponent mod modulus
	// not working
	modstart = modstart.Add(modstart, modremainder)

	fmt.Println("modstart test result ", fmt.Sprint(modstart), " ", fmt.Sprint(modremainder))
	log.Println("modstart test result ", fmt.Sprint(modstart))
*/
	// this adds the modulus remainder + the modulus exponent converted from base 2
	//// bug here // remainderString, _ := strconv.ParseInt(remainder, 10, 64)
        // modstart := big.NewInt(remainderString);
	// this should be a bigint and it was a 64 bit int
     	modstart := new(big.Int)
        modstart.SetString(remainder, 10) 
        // modstart := big.NewInt(remainder);
	modstart = modstart.Add(modstart, modremainder)

        fmt.Println("modstart test result floor ", fmt.Sprint(modstart), " initial remainder ", fmt.Sprint(modremainder))
        log.Println("modstart test result floor ", fmt.Sprint(modstart))

	// create the hash contexts
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
                gt := modstart.Cmp(modceil) // have to check this

                if gt > 0 {
			fmt.Println("Not Found Block ", buf, " modstart ", modstart, " mod ceil ", modceil, " ", modceiltwo)
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

	bufstring := fmt.Sprintf("%v", buf)

	return 0, bufstring
}

// calculate the modulus exponent
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

// converts 2 ^ exp to the modulus floor
func convertFloorBase2 (modfloor *big.Int, modi *big.Int) *big.Int {

	mfloor := big.NewInt(0)
        mfloor = mfloor.Set(modfloor)
        zero := big.NewInt(0)
        modremainder := big.NewInt(0)   //new(big.Int)
        modremainder = modfloor.Mod(modfloor, modi);
        gt := modremainder.Cmp(zero)

	fmt.Println("mfloor ", mfloor, " ", modfloor)

        // if the remainder is zero set modremainder to modfloor
        if gt == 0 {
                fmt.Println("modulo floor equals zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
                modremainder = modremainder.Set(mfloor)
                fmt.Println("modulo floor equals zero setting ", fmt.Sprint(modremainder), " ", mfloor)
        // otherwise subtract the modremainder from modfloor
        } else {
                fmt.Println("modulo floor greater than zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
                // modremainder = mfloor
                modremainder = modremainder.Sub(modfloor, modremainder)
                fmt.Println("modulo floor sub ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
        }
	remstring := modremainder.String()
	fmt.Println("modremainder ", modremainder, " ", remstring)
	// return 0
	return modremainder
}

func setLog() {
/*
        logfile, err := os.OpenFile(fdata.logfile, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0666)
        if err != nil {
                fmt.Println("Failed to open log file:", err)
        }
        defer logfile.Close()
        fdata.log = log.New(logfile, "", log.Ldate|log.Ltime|log.Lshortfile)
*/
}
