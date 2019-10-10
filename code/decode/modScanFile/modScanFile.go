package modScanFile

// modScan
// modScan is an example of a prototype parallel modulus scan on a random generated byte block
// modScan can also use a user defined byte block
//
// modScan recives the input byte block with n-bytes and creates an n-bit modular floor
// modScan will then run a parallel modulus scan to find the original n-byte block with a matching signature
// decoderRandom4 uses goroutines to run the modScan decoders in sequence
//
// copyright (C) Scott Ross 2019
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	// "bytes"
	// "crypto/rand"
	"crypto/md5"
	"crypto/sha1"
	"encoding/hex"
	"fmt"
	"math/big"
	// "strconv"
	// "bytes"
	"hash"
	"log"
	"os"
	"time"
	"strings"
	//"runtime"
)

// modScan flag struct
type DecodeData struct {
	blocksize    string
	blocksizeInt int64
	// modulus size
	modsize    string
	modsizeInt int64
	// modulus exponent
	modExp int64
	// byte block bigint
	blockBigInt *big.Int
	// modulus data
	modulusBigInt          *big.Int
	modulusBigIntString    string
	modulusBigIntRemainder *big.Int
	// threads
	threadNumber int64
	threadCount  int64
	// thread incrmenter
	modulusThreadNumber *big.Int
	modulusThreadCount  *big.Int
	modulusStart        *big.Int
	// bytes buffer
	Byteblock []byte
	Result []byte
	// matchFound
	matchFound   bool
	collisionCnt int64
	// time
	timeStarted string
	// signatures
	md5hash  hash.Hash
	sha1hash hash.Hash
	// signatures results
	sha1hex   string
	md5hex    string
	mdDigHex  string
	mdDig2Hex string
	// sha1/md5 bytesblock
	sha1byteblock []byte
	md5byteblock []byte
	// log writer
	islogging bool
	Logfile   *log.Logger
}

// Init returns a new modScan object
func Init(blocksize int64, modsize int64, thread int64, threadCount int64, bytes []byte, time string) (md *DecodeData) {
	mdata := new(DecodeData)
	mdata.blocksizeInt = blocksize
	mdata.modsizeInt = modsize
	mdata.modExp = 0
	mdata.threadNumber = thread
	mdata.threadCount = threadCount
	mdata.Byteblock = bytes
	mdata.matchFound = false
	mdata.collisionCnt = 0
	mdata.timeStarted = time
	mdata.initLog()
	mdata.islogging = false
	return mdata
}

// run a parallel modulus scan on a user defined or random byte block array
func (md *DecodeData) ModulusScanBytes(c chan string) {

	//runtime.LockOSThread()

	// process the modulus bitsize argument
	bitsize := md.modsizeInt

	// convert the bytes to a string
	bytestring := fmt.Sprintf("%v", md.Byteblock)

	// create the biginteger representation of the bytes
	blockBigInt := new(big.Int)
	md.blockBigInt = blockBigInt.SetBytes(md.Byteblock)

	// create the modulus bigint 2 to the bitsize exponent
	// ie if it is 8 then it is 2 to the bitsize nth
	modulusBigInt := big.NewInt(bitsize)
	base := big.NewInt(2)
	md.modulusBigInt = modulusBigInt.Exp(base, modulusBigInt, nil)
	md.modulusBigIntString = modulusBigInt.String()

	// calculate the modulus remainder
	fileblockmodulus := new(big.Int)
	fileblockmodulus = fileblockmodulus.Mod(md.blockBigInt, md.modulusBigInt)
	md.modulusBigIntRemainder = fileblockmodulus

	// calculate the modulus exponent
	two := big.NewInt(2)
	var modexp = md.logN(md.blockBigInt, two)
	md.modExp = modexp

	// calculate the modulus thread multiple
	// this code makes the modulus parallel
	// modulus * threadnumber
	modthreadNumber := big.NewInt(md.threadNumber)
	md.modulusThreadNumber = modthreadNumber.Mul(md.modulusBigInt, modthreadNumber)

	// calculate the modulus threadcount
	// this is the modulus increment value + the modulus threadnumber
	// modulus * threadcount
	modthreadCount := big.NewInt(md.threadCount)
	md.modulusThreadCount = modthreadCount.Mul(md.modulusBigInt, modthreadCount)

	// calculate the modulus start
	md.modulusStart = md.modulusBigIntRemainder
	// add the modulusthreadNumber to the modStart
	md.modulusStart = md.modulusStart.Add(md.modulusStart, md.modulusThreadNumber)

	// create a sha1 hash of the bytes
	// create an md5 hash of the bytes
	md.setSignature()

	// log the starting modScan data
	md.modScanData()

	_, buffer := md.decode()

	// run until found
	// if bytestring == buffer {
	md.Println("random bytestring and modulusscan bytestring match ", bytestring, " ", buffer)
	// s := "thread " + fmt.Sprint(md.threadNumber) + " random bytestring and modulusscan bytestring match " + bytestring + " " + buffer
	s := buffer
	c <- s
	// }

	// c <- "Not found"
	return

}

// run a parallel modulus scan on a user defined or random byte block array
func (md *DecodeData) ModulusScanFileBytes(blockSize uint64, modSize uint32, hashlist string, modRemainder string, c chan string) {

        //runtime.LockOSThread()

        // process the modulus bitsize argument
        bitsize := md.modsizeInt
	// md.blocksizeInt = int64(blockSize)

        // convert the bytes to a string
        // bytestring := fmt.Sprintf("%v", md.byteblock)

        // create the biginteger representation of the bytes
        // blockBigInt := new(big.Int)
        //md.blockBigInt = blockBigInt.SetBytes(md.byteblock)

        // create the modulus bigint 2 to the bitsize exponent
        // ie if it is 8 then it is 2 to the bitsize nth
        modulusBigInt := big.NewInt(bitsize)
        base := big.NewInt(2)
        md.modulusBigInt = modulusBigInt.Exp(base, modulusBigInt, nil)
        md.modulusBigIntString = modulusBigInt.String()

        // calculate the modulus remainder
        // fileblockmodulus := new(big.Int)
        //fileblockmodulus = fileblockmodulus.Mod(md.blockBigInt, md.modulusBigInt)
        //md.modulusBigIntRemainder = fileblockmodulus
	fileblockmodulus := new(big.Int)
        fileblockmodulus, _ = fileblockmodulus.SetString(modRemainder, 10)
        md.modulusBigIntRemainder = fileblockmodulus

        // calculate the modulus exponent
        // two := big.NewInt(2)
        // var modexp = md.logN(md.blockBigInt, two)
        md.modExp = int64(modSize)

        // calculate the modulus thread multiple
        // this code makes the modulus parallel
        // modulus * threadnumber
        modthreadNumber := big.NewInt(md.threadNumber)
        md.modulusThreadNumber = modthreadNumber.Mul(md.modulusBigInt, modthreadNumber)

        // calculate the modulus threadcount
        // this is the modulus increment value + the modulus threadnumber
        // modulus * threadcount
        modthreadCount := big.NewInt(md.threadCount)
        md.modulusThreadCount = modthreadCount.Mul(md.modulusBigInt, modthreadCount)

        // calculate the modulus start
        md.modulusStart = md.modulusBigIntRemainder
        // add the modulusthreadNumber to the modStart
        md.modulusStart = md.modulusStart.Add(md.modulusStart, md.modulusThreadNumber)
	md.Printlog("thread ", md.threadNumber)
	md.Printlog("modulus start ",  md.modulusBigIntRemainder.String())
	md.Printlog("modulus plus thread number ", md.modulusThreadNumber)
	md.Printlog("Starting modulus ", md.modulusStart)

        // create a sha1 hash of the bytes
        // create an md5 hash of the bytes
 	// convert the string hash list block to an array
        hs := strings.Split(hashlist, ":")
        md5 := hs[0]
        a, err := hex.DecodeString(md5)
        if err != nil {
                panic(err)
        }
        md.md5byteblock = a

        sha1 := hs[1]
        b, err := hex.DecodeString(sha1)
        if err != nil {
                panic(err)
        }
        md.sha1byteblock = b
        md.setFileSignature()

        // log the starting modScan data
        md.modScanData()

        _, buffer := md.decode()

        // run until found
        // if bytestring == buffer {
        // md.Println("random bytestring and modulusscan bytestring match ", buffer)
        // s := "thread " + fmt.Sprint(md.threadNumber) + " random bytestring and modulusscan bytestring match " + buffer
        // s := "thread " + fmt.Sprint(md.threadNumber) + " found XXX bytestring match " + buffer
        // s := "" + buffer
	// md.result = buffer
	// md.Result = copy(md.Result, buffer)
	if md.matchFound == true {
		// fmt.Println("abba ", md.Result, buffer)
		md.Println("buffer ", buffer)
		fmt.Printf("Found Block % x\n", md.Byteblock)
        	s := "" + fmt.Sprint(md.threadNumber) 
        	// c <- s
		// c <- md.threadNumber
		md.write(c, s)
	}
        // }
        // c <- "Not found"
        return

}

// how to write on possibly closed channel
func (md *DecodeData) write(out chan string, msg string) (err error) {

	defer func() {
		// recover from panic caused by writing to a closed channel
		if r := recover(); r != nil {
			err = fmt.Errorf("%v", r)
			// fmt.Printf("write: error writing %d on channel: %v\n", i, err)
			return
		}

		// fmt.Printf("write: wrote on channel\n", md.modulusThreadNumber)
	}()

	out <- msg // write on possibly closed channel

	return err
}


// *************************** New
// fun the modulus scan on an unknown file byte block
func (md *DecodeData) ModulusScanFileBytes2(blockSize uint64, modSize uint32, hashlist string, modRemainder string, c chan string) {
// func (md *DecodeData) ModulusScanFileBytes(c chan string) {

	// set the blocksize
	md.blocksizeInt = int64(blockSize)

        // process the modulus bitsize argument
	bitsize := md.modsizeInt

        // create the modulus bigint 2 to the bitsize exponent
        // ie if it is 8 then it is 2 to the bitsize nth
        modulusBigInt := big.NewInt(bitsize)
        base := big.NewInt(2)
        md.modulusBigInt = modulusBigInt.Exp(base, modulusBigInt, nil)
        md.modulusBigIntString = modulusBigInt.String()

	// set the modulus remainder
	fileblockmodulus := new(big.Int)
	fileblockmodulus, _ = fileblockmodulus.SetString(modRemainder, 10)
        md.modulusBigIntRemainder = fileblockmodulus

	// calculate the modulus exponent
	md.modExp = int64(modSize)

	// calculate the modulus thread multiple
        // this code makes the modulus parallel
        // modulus * threadnumber
        modthreadNumber := big.NewInt(md.threadNumber)
        md.modulusThreadNumber = modthreadNumber.Mul(md.modulusBigInt, modthreadNumber)

        // calculate the modulus threadcount
        // this is the modulus increment value + the modulus threadnumber
        // modulus * threadcount
        modthreadCount := big.NewInt(md.threadCount)
        md.modulusThreadCount = modthreadCount.Mul(md.modulusBigInt, modthreadCount)

	// calculate the modulus start
        md.modulusStart = md.modulusBigIntRemainder
        // add the modulusthreadNumber to the modStart
        md.modulusStart = md.modulusStart.Add(md.modulusStart, md.modulusThreadNumber)

	// convert the string hash list block to an array
	hs := strings.Split(hashlist, ":")
	md5 := hs[0]
	a, err := hex.DecodeString(md5)
	if err != nil {
		panic(err)
	}
	md.md5byteblock = a

	sha1 := hs[1]
        b, err := hex.DecodeString(sha1)        
        if err != nil {                            
                panic(err)                         
        }
	md.sha1byteblock = b

	// fmt.Printf("md5 % X\n", a)
	// fmt.Printf("sha % X\n", b)
	md.setFileSignature()

        // log the starting modScan data
        md.modScanData()

	_, buffer := md.decode()


        // md.Println("random bytestring and modulusscan bytestring match ", bytestring, " ", buffer)
	// if (buffer != "") {
		md.Println("found modulusscan file bytestring ", buffer)
		// s := "thread " + fmt.Sprint(md.threadNumber) + " random bytestring and modulusscan bytestring match " + bytestring + " " + buffer
		s := "thread " + fmt.Sprint(md.threadNumber) + " file bytestring found " + " " + buffer 
		c <- s
	//}

	// c <- "Not found"


	return
}

// calculate the byte block associated with a blocksize and modulus and modulus exponent with a sha1 and md5 hash
// this will run the modulus scan decode to find a byte block associated with a modulus floor and sinature
func (md *DecodeData) decode() (int, string) {

	start := time.Now()
	md.Printlog("Starting modScan byte decoder ", md.threadNumber)

	// create the byte buffer block size in bytes
	buf := make([]byte, md.blocksizeInt)

	// calculate the modular exponent floor and ceiling
	// the base in the exponent is 2 it can also be the modulus to some power less than the big block int
	// floor = 2 to the power
	// ceil = 2 to the power + 1 number less than greater than the block number
	floor := md.modExp
	ceil := floor + 1
	var modfloor, modexponent = big.NewInt(2), big.NewInt(floor)
	var modceiltwo, modceil = big.NewInt(2), big.NewInt(ceil)
	modfloor.Exp(modfloor, modexponent, nil)
	modceil.Exp(modceiltwo, modceil, nil)
	modulofloorstr := fmt.Sprint(modfloor)
	moduloceilstr := fmt.Sprint(modceil)

	md.Printlog("modulo floor ", modulofloorstr, " ceil ", moduloceilstr, " modceiltwo ", modceiltwo)
	md.Printlog("modulo floor string ", modfloor.String(), " ceil ", modceil.String(), " modceiltwo ", modceiltwo)
	md.Printlog("modulo floor bytes ", modfloor.Bytes(), " ceil ", modceil.Bytes(), " modceiltwo ", modceiltwo)

	md.Printlog("modulo floor ", md.modExp, " ", modulofloorstr, " ceil ", moduloceilstr)
	md.Printlog("modulo floor exponent ", md.modExp, " modulo floor ", modulofloorstr, " modulo ceil ", moduloceilstr, " mod ", md.modulusBigInt)
	md.Printlog("starting modulus byteblock before mod remainder added ", md.modulusStart.Bytes(), " bigint ", md.modulusStart, " ", md.blockBigInt)

	// calcluate the 2^exp mod floor
	// this converts the 2^exponent to modulus*n for the modfloor + the remainder
	modremainder := new(big.Int)
	modremainder = md.convertFloorBase2(modfloor, md.modulusBigInt)
	// test code
 	/// modremainder := new(big.Int)
	/// modremainder = md.modulusBigIntRemainder

	// add the modremainder the the modulusStart
	md.modulusStart = md.modulusStart.Add(md.modulusStart, modremainder)

	md.Printlog("starting modulus byteblock after remainder added ", md.modulusStart.Bytes(), " bigint ", md.modulusStart, " ", md.blockBigInt)
	md.Printlog("thread ", md.threadNumber, " ", md.threadCount, " modstart test result floor ", fmt.Sprint(md.modulusStart), " initial remainder ", fmt.Sprint(modremainder))

	// create the hash contexts
	md5 := md5.New()
	sha1 := sha1.New()

	md.Printlog("starting modulus byteblock ", md.modulusStart.Bytes())

	var lineCount uint64 = 1
	for {
		// copy the modulus bytes to the byte buffer
		// if the modulus start byte block is less than the buffer byte block copy it to the end of the buffer byte block
		var lbuf   = len(buf)
		var lenmd  = len(md.modulusStart.Bytes())
		// if the buf size equals the modulus start size just copy them
		if lbuf == lenmd {
			copy(buf[:], md.modulusStart.Bytes())
		// if the length of the modulusStart bytes is greater than the buffer stop
		} else if lenmd >  lbuf {
			md.Printlog("Not Found Block Greater ", md.modulusStart.Bytes())
			break
		// if the length of the buffer is greater than the modulusStart buffer copy it to the end
		// ie buffer size 3 modulus start byte length 2 [1,1]
		// buf = [0,1,1]
		} else {
			// fmt.Println("slice ", lbuf - lenmd, " buf ", lbuf, " mod bytes length ",  lenmd, " bytes ", md.modulusStart.Bytes())
			var start = lbuf - lenmd
			var end = lbuf
			copy(buf[start:end], md.modulusStart.Bytes())
		}
		// fmt.Println("bigint ", buf, " modulus start bytes ", md.modulusStart.Bytes())

		md5.Write([]byte(buf))

		// byte comparison is faster than string comparison
		/// if bytes.Equal(md5.Sum(nil), md.md5hash.Sum(nil)) {
		// if bytes.Equal(md5.Sum(nil), md.md5byteblock) {
		md5string := hex.EncodeToString(md5.Sum(nil))
		if md5string == md.md5hex {
			sha1.Write([]byte(buf))
			// sha1string := hex.EncodeToString(sha1.Sum(nil))
			// fmt.Println("md5 sha1 ", hex.EncodeToString(md5.Sum(nil)), " ", hex.EncodeToString(sha1.Sum(nil)))

			// compare the modulus sha hash bytes with the destination sha hash
			// if they are equal return the block as found
			///// if bytes.Equal(sha1.Sum(nil), md.sha1hash.Sum(nil)) {
			// if bytes.Equal(sha1.Sum(nil), md.sha1byteblock) {
			sha1string := hex.EncodeToString(sha1.Sum(nil))
			if sha1string == md.sha1hex {
				// fmt.Println("Found Block ", md.threadNumber, md5string, md.md5hex, sha1string, md.sha1hex, buf)
				md.Println("Found Block ", md5string, md.md5hex, sha1string, md.sha1hex, buf)
				// fmt.Printf("Found Block % x\n", buf)
				md.Byteblock = md.modulusStart.Bytes()
				md.matchFound = true
				break
			}
			sha1.Reset()
		}
		md5.Reset()

		// increment the modulus bigint by the modulus * thread count
		md.modulusStart = md.modulusStart.Add(md.modulusStart, md.modulusThreadCount)
		// check if the modulusStart is greater than modceil
		gt := md.modulusStart.Cmp(modceil)

		if gt > 0 {
			md.Printlog("Not Found Block ", buf, " thread ", md.threadNumber)
			break
		}

		if lineCount >= 1000000 {
			md.Printlog("blockbuffer value ", md.modulusStart)
			lineCount = 0
		}

		lineCount++

	}

	t := time.Now()
	elapsed := t.Sub(start)

	if md.matchFound == false {
		md.Printlog("Total time ", elapsed)
	} else {
		md.Println("Total time ", elapsed)
	}

	bufstring := fmt.Sprintf("%v", buf)

	return 0, bufstring
}

// calculate the modulus exponent
func (md *DecodeData) logN(fileblockint *big.Int, base *big.Int) int64 {

	var exponent int64 = 1
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

// converts 2 ^ exp to the modulus floor ^ exp
func (md *DecodeData) convertFloorBase2(modfloor *big.Int, modi *big.Int) *big.Int {

	mfloor := big.NewInt(0)
	mfloor = mfloor.Set(modfloor)
	zero := big.NewInt(0)
	modremainder := big.NewInt(0) //new(big.Int)
	modremainder = modfloor.Mod(modfloor, modi)
	gt := modremainder.Cmp(zero)

	md.Printlog("mfloor ", mfloor, " ", modfloor)

	// if the remainder is zero set modremainder to modfloor
	if gt == 0 {
		md.Printlog("modulo floor equals zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
		modremainder = modremainder.Set(mfloor)
		md.Printlog("modulo floor equals zero setting ", fmt.Sprint(modremainder), " ", mfloor)
		// otherwise subtract the modremainder from modfloor
	} else {
		md.Printlog("modulo floor greater than zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
		// modremainder = mfloor
		modremainder = modremainder.Sub(modfloor, modremainder)
		md.Printlog("modulo floor sub ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
	}
	remstring := modremainder.String()
	md.Printlog("modremainder ", modremainder, " ", remstring)

	return modremainder
}

// set the signature of the byte block
func (md *DecodeData) setSignature() {

	bytes := md.Byteblock

	// create an sha1 hash of the bytes
	h := sha1.New()
	h.Write([]byte(bytes))
	var shasum = hex.EncodeToString(h.Sum(nil))
	md.sha1hex = shasum
	// store context
	md.sha1hash = h

	// create an md5 hash of the bytes
	md5hash := md5.New()
	md5hash.Write([]byte(bytes))
	var md5sum = hex.EncodeToString(md5hash.Sum(nil))
	md.md5hex = md5sum
	// store context
	md.md5hash = md5hash

}

// set the signature of the byte block
func (md *DecodeData) setFileSignature() {

        // these are just strings
        md.md5hex  = fmt.Sprintf("%x", md.md5byteblock)
        md.sha1hex = fmt.Sprintf("%x", md.sha1byteblock)
}

// display the modScan data
func (md *DecodeData) modScanData() {

	md.Printlog("Starting Modulus Scan Random ", md.threadNumber)

	md.Printlog("blocksize ", md.blocksizeInt)
	// md.Printlog("byte block", md.byteblock)
	// md.Printlog("byte bigint", md.blockBigInt.String())
	md.Printlog("modulus bit size ", md.modsizeInt)
	md.Printlog("byte block modulus ", md.modulusBigIntString)
	md.Printlog("byte block modulus remainder ", md.modulusBigIntRemainder.String())
	md.Printlog("modulus exponent ", md.modExp)

	// md.Printlog("shasum ", md.sha1hex)
        // md.Printlog("md5sum ", md.md5hex)

	md.Printlog("md5sum ", fmt.Sprintf("%x", md.md5byteblock))
	md.Printlog("shasum ", fmt.Sprintf("%x", md.sha1byteblock))

	md.Printlog("threadnumber ", md.threadNumber)
	md.Printlog("thread count ", md.threadCount)
	md.Printlog("modulus * threadnumber ", md.modulusThreadNumber.String())
	md.Printlog("modulus * threadCount ", md.modulusThreadCount.String())
}

// get the modScan byteblock
func (md *DecodeData) GetBytes() []byte {
	return md.Byteblock
}

// get the modScan match found
func (md *DecodeData) MatchFound() bool {
	return md.matchFound
}

// get the modScan collision count
func (md *DecodeData) GetCollisionCount() int64 {
	return md.collisionCnt
}

// initialize the logfile
func (md *DecodeData) initLog() {

	var logfilename = "decodeRandom."
	s := fmt.Sprintf("%d", md.threadNumber)
	// logfilename = logfilename + s + "." + md.timeStarted + ".log"
	logfilename = logfilename + s + ".log"

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
func (md *DecodeData) Println(v ...interface{}) {
	fmt.Println(v...)
	md.Logfile.Println(v...)

}

// write to the log
func (md *DecodeData) Printlog(v ...interface{}) {
	md.Logfile.Println(v...)

}
