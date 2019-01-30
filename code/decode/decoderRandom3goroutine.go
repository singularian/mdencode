
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
//	"bytes"
)

// mdencode flag struct
type DecodeData struct {
	blocksize string
	modsize string
	modExp int
	blockBigInt *big.Int
	modulusBigInt *big.Int
	modulusBigIntRemainder *big.Int
	threadNumber int64
	threadCount int64
	// bytes
	byteblock []byte
        // log writer
        // log *log.Logger
	Logfile *log.Logger
}

// generates a random n byte array and then hashes it
// it then runs it through the modulus scan
func main() {

	mddecode()
	os.Exit(0)
}

// mdecode file
// func mddecode(argsNumber int) int {
func mddecode() int {

	var argsNumber int = len(os.Args)

        md := new(DecodeData)
	md.initLog()

        // test a random byte block
        // arguments blocksize bytes and modsize bits
        // more than 6 bytes takes a while 
        if argsNumber == 4 {
                var c chan string = make(chan string)

                blocksize  := os.Args[1]
                modsize    := os.Args[2]
                threadsize := os.Args[3]
                blocksizeint, _ := strconv.Atoi(blocksize)
		var threadNumber int64
		var i int64 
		threadNumber, _ = strconv.ParseInt(threadsize, 10, 64) 

		bytes := make([]byte, blocksizeint)
                _, _ = rand.Read(bytes)

		mdp:=[]*DecodeData{}
                for i = 0; i<threadNumber; i++ {
			md := new(DecodeData)
			md.threadNumber = i
			md.threadCount  = threadNumber
			md.byteblock = bytes
			md.initLog()
                	// go md.modulusScanRandom(blocksizeint, modsize, i, 10, c)
			mdp=append(mdp,md)
		}

		for i = 0; i < threadNumber; i++ {
		 	go mdp[i].modulusScanRandom(blocksizeint, modsize, i, 10, c)

		}

		for result := range c {
			fmt.Println("result ", result)
	
		}
        // test random
        } else {
           //       md.testmodulusScanRandom()
	   fmt.Println("Usage ", os.Args[0], " [BLOCKSIZE BYTES] [MODSIZE BITS] [THREADSIZE GOROUTINES]")
	   fmt.Println("Usage ", os.Args[0], " 8 32 10")
        }


	return 0

}


// test method
func (md *DecodeData) testmodulusScanRandom() {

	// fd := new(DecodeData)

        //// md.modulusScanRandom(6, "32")
        ///// md.modulusScanRandom(6, "24")

        // 8 bytes ran in 24 hours
        //      decode("8", "24", "61", "7544937", "b597b21cd5ddcde0944cc7734d2f5da9", "19cfdd42d9389ee1a7709194020ce055e2493e05")
        // modulusScanRandom(8, "24")

}


// run a modulus scan on a random byte array
func (md *DecodeData) modulusScanRandom(blockSize int, modSize string, threadNumber int64, threadCount int, c chan string) {
        // create a random byte array
	// nead to init once
        // bytes := make([]byte, blockSize)
	bytes := md.byteblock
        // _, _ = rand.Read(bytes)
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
        modexp := md.logN(blockBigInt, two)
        s := strconv.Itoa(modexp)

	// create a sha1 hash of the bytes
        h := sha1.New()
        h.Write([]byte(bytes))
        var shasum =  hex.EncodeToString(h.Sum(nil))

	// create an md5 hash of the bytes
        md5hash := md5.New()
        md5hash.Write([]byte(bytes))
        var md5sum = hex.EncodeToString(md5hash.Sum(nil))

        md.Println("Starting Modulus Scan Random")
        md.Println("random ", blockSize, " bytes ", bytes, " ", blockBigIntstring)
        md.Println("modulus size bits ", bitsize)
        md.Println("byte block modulus ", modulusBigIntString)
        md.Println("byte block modulus remainder ", blockmod)
        md.Println("modulus exponent ", modexp)

        md.Println("shasum ", shasum);
        md.Println("md5sum ", md5sum);

        _, buffer := md.decode(blockSizeStr, modSize, s, blockmod, md5sum, shasum, c)

	if(bytestring == buffer) {
		md.Println("random bytestring and modulusscan bytestring match ", bytestring, " ", buffer)
		s := "thread " + fmt.Sprint(md.threadNumber) + " random bytestring and modulusscan bytestring match " + bytestring + " " + buffer
		c <- s
		close(c)
	}

	return

}

// calculate the byte block assicated with a blocksize and modulus and modulus exponent with a sha1 and md5 hash
// this will run the modulus scan decode
func (md *DecodeData) decode(blockSize string, modbitSize string, modexp string, remainder string, md5hex string, sha1hex string, c chan string) (int, string) {


        var hashone string  = md5hex
        var hashtwo string  = sha1hex

	start := time.Now()
	md.Println("Starting decoderRandom")

        blocksize, _ := strconv.ParseInt(blockSize, 10, 64)
        buf := make([]byte, blocksize)

        // process the modulus bitsize argument
        // raise 2 to the bitsize exponent
        bitsize, _ := strconv.ParseInt(modbitSize, 10, 64)
        var i, e = big.NewInt(2), big.NewInt(bitsize)
        i.Exp(i, e, nil)
        modulostr := fmt.Sprint(i)

	md.Println("modulo bigint", modulostr)

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

	md.Println("modulo floor ", modulofloorstr, " ceil ", moduloceilstr, " modceiltwo ", modceiltwo)

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

	md.Println("modulo floor ", modexp, " ", modulofloorstr, " ceil ", moduloceilstr)
	fmt.Println("modulo floor exponent ", modexp, " modulo floor ", modulofloorstr, " modulo ceil ", moduloceilstr, " mod ", i)

        // calcluate the 2^exp mod floor
	// this converts the 2^exponent to modulus*n for the modfloor + the remainder
	modremainder := new(big.Int)
	modremainder = md.convertFloorBase2(modfloor, i)
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
	// thread count
	var threadNumber = md.threadNumber
	var threadCount  = md.threadCount
        // mult2
	// modulus * the threadnumber
        mult2           := big.NewInt(threadNumber)
        mult2 = mult2.Mul(i, mult2)
        modstart = modstart.Add(modstart, mult2)

	// modulus * the threadcount 
	mult            := big.NewInt(threadCount)
	// modOffSet       := new(big.Int)
	// i = modOffSet.Mul(i, mult)
	i = mult.Mul(i, mult)

       // md.Println("thread ", threadNumber, " ", threadCount, " modstart test result floor ", fmt.Sprint(modstart), " initial remainder ", fmt.Sprint(modremainder))
       md.Println("thread ", threadNumber, " ", threadCount, " modstart test result floor ", fmt.Sprint(modstart), " initial remainder ", fmt.Sprint(modremainder), " iterator ", fmt.Sprint(i), " mult = mod * thc ", fmt.Sprint(mult),  " mult2 = m * thnum ", fmt.Sprint(mult2))

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
				md.Println("Found Block ", buf)
				// var buffer bytes.Buffer
				// buffer.WriteString("Found Block ")
				// s := string(buf[:])
				// s := fmt.Sprintf("%x", buf)
				// buffer.WriteString(s)
				// md.Println("buffer ", buffer.String())
				// st := buffer.String()
				// c <- st
				// c <- buffer.String()
				// break
				// done <- true
				// close(c)
                                // return
				break
			}
			sha1.Reset()
		}
                md5.Reset()

                modstart := modstart.Add(modstart, i)
                gt := modstart.Cmp(modceil) // have to check this

                if gt > 0 {
			md.Println("Not Found Block ", buf, " modstart ", modstart, " mod ceil ", modceil, " ", modceiltwo)
			break
                }

		if lineCount >= 1000000 {
			md.Printlog("blockbuffer value ", modstart)
			lineCount = 0
		}

		lineCount++

        }

	t := time.Now()
        elapsed := t.Sub(start)

	md.Println("Total time ", elapsed)

	bufstring := fmt.Sprintf("%v", buf)

	return 0, bufstring
}

// calculate the modulus exponent
func (md *DecodeData) logN(fileblockint *big.Int, base *big.Int) int {

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
func (md *DecodeData) convertFloorBase2 (modfloor *big.Int, modi *big.Int) *big.Int {

	mfloor := big.NewInt(0)
        mfloor = mfloor.Set(modfloor)
        zero := big.NewInt(0)
        modremainder := big.NewInt(0)   //new(big.Int)
        modremainder = modfloor.Mod(modfloor, modi);
        gt := modremainder.Cmp(zero)

	md.Println("mfloor ", mfloor, " ", modfloor)

        // if the remainder is zero set modremainder to modfloor
        if gt == 0 {
                md.Println("modulo floor equals zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
                modremainder = modremainder.Set(mfloor)
                md.Println("modulo floor equals zero setting ", fmt.Sprint(modremainder), " ", mfloor)
        // otherwise subtract the modremainder from modfloor
        } else {
                md.Println("modulo floor greater than zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
                // modremainder = mfloor
                modremainder = modremainder.Sub(modfloor, modremainder)
                md.Println("modulo floor sub ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
        }
	remstring := modremainder.String()
	md.Println("modremainder ", modremainder, " ", remstring)
	// return 0
	return modremainder
}

// initialize the logfile
func (md *DecodeData) initLog() {

	// now := time.Now()
	var logfilename = "decodeRandom."
	// could use random number
	// var time = fmt.Sprintf("%d%d%d%d%d", now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second())
	// logfilename = logfilename  + time + ".log"
	s := fmt.Sprintf("%d", md.threadNumber)
	logfilename = logfilename  + s + ".log"
	// var logfile = md.logfile

        logfile, err := os.OpenFile(logfilename, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0666)
        if err != nil {
                fmt.Println("Failed to open log file:", err)
        }
	// using defer close causes it to close the logfile
	// and not write
        // defer logfile.Close()
        md.Logfile = log.New(logfile, "", log.Ldate|log.Ltime)

}

// write to log and standard out
func (md *DecodeData) Println (v ...interface{}) {
	fmt.Println(v...)
        md.Logfile.Println(v...)

}

// write to the log
func (md *DecodeData) Printlog (v ...interface{}) {
        md.Logfile.Println(v...)

}
