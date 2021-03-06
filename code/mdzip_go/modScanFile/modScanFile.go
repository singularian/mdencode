package modScanFile

// project mdencode
//
// modScanFile.go
//
// modScanFile is a package to run a parallel modulus scan on a file byte block
//
// modScanFile decrypts an mdzip file and finds a byte block associated with a modulus scan hash list and modular floor
// modScanFile runs a parallel modulus scan to find the original n-byte block with a matching signature
//
// There is a modScan for random bytes and modScan for files
//
// copyright (C) Scott Ross 2019

// https://github.com/singularian/mdencode
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"math/big"
	"log"
	"os"
	"time"
	"sync"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
	"github.com/singularian/mdencode/code/mdzip_go/mdUnzipMutex"
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
	// blockNumber
	blockNumber int64
	// threads
	threadNumber int64
	threadCount  int64
	// thread incrmenter
	modulusThreadNumber *big.Int
	modulusThreadCount  *big.Int
	modulusStart        *big.Int
	// bytes buffer
	byteblock []byte
	Result []byte
	// matchFound
	matchFound   bool
	collisionCnt int64
	// time
	timeStarted string
	// signatures results
	// blockHashNameList string
	// file mutex
	mux *mdUnzipMutex.FileMutex
	// hash context list
	hashContexList *mdHashContextList.HashContextList
	// log writer
	islogging bool
	stop bool
	Logfile   *log.Logger
}

// Init returns a new modScan object
func Init(blocksize int64, modsize int64, blockNumber int64, thread int64, threadCount int64, mux *mdUnzipMutex.FileMutex, hcl *mdHashContextList.HashContextList) (md *DecodeData) {
	mdata := new(DecodeData)

	mdata.blocksizeInt   = blocksize
	mdata.modsizeInt     = modsize
	mdata.modExp         = 0
	mdata.blockNumber    = blockNumber 
	mdata.threadNumber   = thread
	mdata.threadCount    = threadCount
	// mdata.byteblock   = bytes // initialized in the modulus scan
	mdata.matchFound     = false
	mdata.collisionCnt   = 0
	mdata.initLog()
	mdata.islogging      = false
	mdata.stop           = false
	mdata.mux            = mux
	mdata.hashContexList = hcl
	return mdata
}

// run a parallel modulus scan on a mdzip file block
func (md *DecodeData) ModulusScanFileBytes(modSize uint32, blocklist string, hashlist string, modRemainder string) {

        //runtime.LockOSThread()

        // process the modulus bitsize argument
        bitsize := md.modsizeInt
	// md.blocksizeInt = int64(blockSize)

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

        // log the starting modScan data
        md.modScanData()


        return

}

// run a parallel modulus scan on a mdzip file block
// there was a timing issue when I tried to split this into two functions
func (md *DecodeData) ModulusScanFileBytesOrig(modSize uint32, blocklist string, hashlist string, modRemainder string, blocksize int64, c *sync.WaitGroup) {

        // set the stop to false
        md.stop = false

        // set the current blocksize
        md.blocksizeInt = blocksize
        //runtime.LockOSThread()

        // process the modulus bitsize argument
        bitsize := md.modsizeInt
	// md.blocksizeInt = int64(blockSize)

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

        // log the starting modScan data
        md.modScanData()

        // result := md.decode()
        _ = md.decode()

	// if the byte block matches the modscan signature list add it to the Mutex and end the work group
	if md.matchFound == true {
		bufstring := fmt.Sprintf("%v", md.byteblock)
		md.Println("buffer ", bufstring)
		fmt.Printf("Found Thread %d Block % x\n", md.threadNumber, md.byteblock)
		md.mux.SetFileBuffer(int(md.threadNumber), md.byteblock)
		c.Done()
	} else if md.matchFound == false {
		//	fmt.Println("Not found ", md.threadNumber, buffer, md.matchFound)
	}
        return

}

func (md *DecodeData) ModulusScanFileBytesRun(blockNumber int64, blocksize int64, c *sync.WaitGroup) {

	// set the stop to false
	md.stop = false

	// set the current blocksize
	md.blocksizeInt = blocksize

	// run the modulus scan decode
        _ = md.decode()

        // if the byte block matches the modscan signature list add it to the Mutex and end the work group
        if md.matchFound == true  {
                bufstring := fmt.Sprintf("%v", md.byteblock)
                md.Println("buffer ", bufstring)
                fmt.Printf("Found Thread %d Block % x blocksize %d\n", md.threadNumber, md.byteblock, blocksize)
                // fmt.Printf("Found Thread block buf % x number %d\n", md.byteblock, blockNumber)
                md.mux.SetFileBuffer(int(md.threadNumber), md.byteblock)
                c.Done()
        } else if md.matchFound == false {
                //      fmt.Println("Not found ", md.threadNumber, buffer, md.matchFound)
        }
        return

}


// calculate the byte block associated with a blocksize and modulus and modulus exponent with a sha1 and md5 hash
// this will run the modulus scan decode to find a byte block associated with a modulus floor and sinature
func (md *DecodeData) decode() (int) {

	// md.matchFound = false
	start := time.Now()
	md.Printlog("Starting modScan byte decoder ", md.threadNumber)

	// create the byte buffer block size in bytes
	buf := make([]byte, md.blocksizeInt)
	var lbuf = md.blocksizeInt

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

	md.Printlog("starting modulus byteblock ", md.modulusStart.Bytes())

	var lineCount uint64 = 1
	for {
		// copy the modulus bytes to the byte buffer
		// if the modulus start byte block is less than the buffer byte block copy it to the end of the buffer byte block
		// var lbuf   = len(buf)
		var lenmd  = int64(len(md.modulusStart.Bytes()))
		// if the buf size equals the modulus start size just copy them
		if lbuf == lenmd {
			copy(buf[:], md.modulusStart.Bytes())
		// if the length of the modulusStart bytes is greater than the buffer stop
		} else if lenmd >  lbuf {
			md.Printlog("Not Found Block Greater ", md.modulusStart.Bytes())
			break
		// if the length of the buffer is greater than the modulusStart buffer copy it to the end
		// ie buffer size 3 modulus start byte length 2 
		// pad the byte
		// bytebuf =   [1,1]
		// buf     = [0,1,1] 
		// if the modulusStart buffer bigint is less than the buffer block size pad it with zeros
		// it needs to copy this result to the buffer so it will create the output uncompressed file properly
		//     ce e6 2a a2 81 67 3f 95 60 2e 
		//  00 ce e6 2a a2 81 67 3f 95 60 2e
		} else {
			/// fmt.Println("slice ", lbuf - lenmd, " buf ", lbuf, " mod bytes length ",  lenmd, " bytes ", md.modulusStart.Bytes())
			var start = lbuf - lenmd
			var end = lbuf
			copy(buf[start:end], md.modulusStart.Bytes())
			/// fmt.Println("slice ", lbuf - lenmd, " buf after fix copy ", buf, " ", md.modulusStart.Bytes())
		}
		// fmt.Println("bigint ", buf, " modulus start bytes ", md.modulusStart.Bytes())

		if md.hashContexList.CheckFileHashBlock(buf)  {
			md.Println("Found Block buffer ", buf, " thread ", md.threadNumber)
			md.byteblock = buf
			md.matchFound = true
			break

		} 


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

		if md.stop {
			break
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

	return 0
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

	// log the block signature list and associated hash list bytes
	if md.threadNumber == 0 {
		md.Printlog(md.hashContexList.GetFileHashBlockSigBytes(md.hashContexList.GetBlockHashBytes()))
	}

	md.Printlog("threadnumber ", md.threadNumber)
	md.Printlog("thread count ", md.threadCount)
	md.Printlog("modulus * threadnumber ", md.modulusThreadNumber.String())
	md.Printlog("modulus * threadCount ", md.modulusThreadCount.String())
}

// get the modScan byteblock
func (md *DecodeData) GetBytes() []byte {
	return md.byteblock
}

// get the modScan match found
func (md *DecodeData) MatchFound() bool {
	return md.matchFound
}

// reset the modScan match found
func (md *DecodeData) ResetMatchFound()  {
        md.matchFound = false
}

// stop the modScan
func (md *DecodeData) StopScan()  {
	md.stop = true
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
