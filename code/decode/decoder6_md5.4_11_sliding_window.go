
package main

import ( 
	"fmt"
	"encoding/hex"
        _ "golang.org/x/crypto/md4"
        "crypto/md5"
        _ "crypto/sha1"
        _ "crypto/sha256"
        _ "crypto/sha512"
        "math/big"
	"strconv"
	"io"
	"os"
	"log"
	"time"
)


type fileData struct {
        commandArgs int
        fileName string
        filePath string
        fileSize uint64
        blockSize uint64
        blockCount uint64
        blockRemainder uint64
        modSize uint64
        modExponent uint64
        mdFormat int
        mdVersion string
        // argument hash list bits
        // fileHashListArray    []string
        // blockHashListArray    []string
        // argument signature hash list 
        // fileHashListNames []string
        // blockHashListNames []string
        // dictionary
        // dictionary map[string]string
        // hash list for files
        // hashList  map[string]hash.Hash
        // hash list for blocks
        // hashListBlocks  map[string]hash.Hash
        // log *log.log
	file *os.File 
	Logfile *log.Logger
	Multi *log.Logger
}

// var (
//     Log   *log.Logger
//     Multi   *log.Logger
//     Logger   *log.Logger
//    Trace   *log.Logger
//    Info    *log.Logger
//    Warning *log.Logger
//    Error   *log.Logger
//)
//

// this is a little different
// it has a 45 block byte size
// a 160-bit modulo 
// and just sha-1 for the hash
func main() {

	// initialize fileData struct
        // fdata := fileData{}
        // initialize the fileData struct information
        // fdta.modSize ata.fileName  = fileName
        // fdata.filePath  = path.Dir(fileName)
        //fdata.blockSize = uint64(blocksize)
        //fdata.modSize   = uint64(bitsize)
        //fdata.mdFormat  = format
	start()

	os.Exit(0)
}

func start() {
        file, err := os.OpenFile("decoder6_11_4_window.log", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
        if err != nil {
                log.Fatal(err)
        }
        // defer to close when you're done with it, not because you think it's idiomatic!
        defer file.Close()
        // set output of logs to f
        // log.SetOutput(f)

	fdata := fileData{}

        fdata.Logfile = log.New(file, "", log.LstdFlags)
        // l.Logfile = flog

        // create the logger to log to the file and standard out
        multiwrite := io.MultiWriter(file, os.Stdout)
        fdata.Multi = log.New(multiwrite, "", log.LstdFlags)

	// fdata.decode("45", "160", "357", "630161686725705158675417117617609061936835949680", "md5", "26a62b1a8518f179dd5a32870be9a4b24994f490")

	// b][size]11[size][hashlist][35 32 84 104 105 115 32 105 115 32 104]:42465076464137276908314728:a94ff320f28604a5630808a94fcfee61[hashlist][modexp]85[modexp][mod]1769152616[mod][b]
	// 42465076464137276908314728 mod 16 is 8
	// window is 8
	// the block is 16 bytes for the md5 + a 5 byte modulus + a 1 or 2 byte collision + a 1 byte exponent byte
	// md5.4 + 4 byte modulus + 1 + 1  = 10 // 11 
	
	fdata.decode("11", "32", "85", "1769152616", "a94ff320f28604a5630808a94fcfee61", "sha1")

}

func (l *fileData) decode(blockSize string, modbitSize string, modexp string, remainder string, md5hex string, sha1hex string) int {


        var hashone string  = md5hex
        // var hashtwo string  = sha1hex

	start := time.Now()
	// l.Multi.Println("Starting decoder5")
	// l.Printlog("Starting decoder5")
	l.Println("Starting decoder6")

        blocksize, _ := strconv.ParseInt(blockSize, 10, 64)
        buf := make([]byte, blocksize)

        // process the modulus bitsize argument
        // raise 2 to the bitsize exponent
        bitsize, _ := strconv.ParseInt(modbitSize, 10, 64)
        var i, e = big.NewInt(2), big.NewInt(bitsize)
        i.Exp(i, e, nil)
        modulostr := fmt.Sprint(i)
        // fmt.Println("modulo bigint ", modulostr)
	l.Println("file md5 ", md5hex)
	l.Println("file blocksize ", blocksize)
	l.Println("modulo bitsize ", bitsize)
	l.Println("modulo bigint", modulostr)

        // module exponent floor and ceiling
        // 2 to the power 1 number less than greater than the block number
	floor, _ := strconv.ParseInt(modexp, 10, 64)
	ceil := floor + 1
        var modfloor, modexponent = big.NewInt(2), big.NewInt(floor)
        var modceiltwo, modceil  = big.NewInt(2),  big.NewInt(ceil)
        modfloor = modfloor.Exp(modfloor, modexponent, nil)
        modceil = modceil.Exp(modceiltwo, modceil, nil)
        modulofloorstr :=  fmt.Sprint(modfloor)
        moduloceilstr :=  fmt.Sprint(modceil)

        // fmt.Println("modulo floor ", modulofloorstr)
        // fmt.Println("modulo ceil ", moduloceilstr)
	l.Println("modulo floor ", modexp, " ", modulofloorstr, " ceil ", moduloceilstr)

	// calcluate the 2^exp mod floor
	mfloor := big.NewInt(1)
	// need to use the Set function to copy the values between bigints
	// the five version doesn't work
	mfloor = mfloor.Set(modfloor)
	zero := big.NewInt(0)
	modremainder := new(big.Int)
        modremainder = modfloor.Mod(modfloor, i);
	gt := modremainder.Cmp(zero)
	// if the remainder is zero set modremainder to modfloor
	if gt == 0 {
		// l.Println("modulo floor sub ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
		// modremainder = modremainder.Sub(modfloor, modremainder)
		//l.Println("modulo floor sub ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
		l.Println("modulo floor equals zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
		modremainder = modremainder.Set(mfloor)
		l.Println("modulo floor equals zero setting ", fmt.Sprint(modremainder), " ", mfloor)
	// otherwise subtract the modremainder from modfloor
	} else {
		l.Println("modulo floor greater than zero ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
		// modremainder = mfloor
		modremainder = modremainder.Sub(modfloor, modremainder)
		l.Println("modulo floor sub ", fmt.Sprint(modremainder), " ", fmt.Sprint(modfloor))
	}
	

        l.Println("modulo floor ", fmt.Sprint(modfloor))
        l.Println("modulo i ", fmt.Sprint(i))
        l.Println("modulo remainder ", fmt.Sprint(modremainder))


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

	l.Println("modulo remainder ", modstart)

	// add the modular floor two to the exponent mod modulus
	modstart = modstart.Add(modstart, modremainder)

	l.Println("modulo modstart ", modstart)

        // sha1 := sha1.New()
	md5  := md5.New()


	var lineCount uint64 = 1
	// window
	window := big.NewInt(12)
 	var windowStart uint64 = 0
	var md5string string
	var md5stringFour string
	var hashoneFour = hashone[8:16]

	// currently not calculated
	l.Println("hashmd5 window ", hashoneFour, " 8 16 ")

        for {
		bigbytes := modstart.Bytes()

                copy(buf[:], bigbytes) 
		// fmt.Println("bigint ", buf, " ", bigbytes)

                md5.Write([]byte(buf))
		md5string = hex.EncodeToString(md5.Sum(nil))
		// sha1.Write([]byte(buf))
		// sha1string := hex.EncodeToString(sha1.Sum(nil))

		modremainder = modremainder.Set(modstart)
		modremainder = modremainder.Mod(modremainder, window);
		windowStart = modremainder.Uint64()

		// md5stringFour := md5string[0:8]
		// hashoneFour   := hashone[0:8]
		md5stringFour = md5string[windowStart:windowStart+8]
		// hashoneFour   := hashone[windowStart:windowStart+8]
		if md5stringFour == hashoneFour {
		// if sha1string == hashtwo {
			// fmt.Println("md5 sha1 ", hex.EncodeToString(md5.Sum(nil)), " ", hex.EncodeToString(sha1.Sum(nil)))
			// fmt.Println("Found Block ", buf, " ", bigbytes)
			l.Println("Found Block ", buf, " ", bigbytes, " ", md5stringFour, " ", hashoneFour)
		//	break
		}
		// sha1.Reset()
		md5.Reset()

                modstart := modstart.Add(modstart, i)
                gt := modstart.Cmp(modceiltwo)

                if gt < 0 {
                       break
                }

		// about 700000 hashes a second
		// if lineCount >= 10000000 {
		if lineCount >= 100000000 {
			l.Printlog("blockbuffer value ", modstart, " window ", windowStart, " ", windowStart+8, " ", md5stringFour, "window ", hashoneFour )
			lineCount = 0
		}

		lineCount++

        }

	t := time.Now()
        elapsed := t.Sub(start)
	// fmt.Println("total time ", elapsed)
	l.Println("Total time ", elapsed)

	return 0
}


func (l *fileData) Initlog() {
/*
        //create log file with desired read/write permissions
	// var err error
        file, err := os.OpenFile("decoder5.log", os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
        if err != nil {
                log.Fatal(err)
        }
        // defer to close when you're done with it, not because you think it's idiomatic!
        defer file.Close()
        // set output of logs to f
        // log.SetOutput(f)

	l.Logfile = log.New(file, "", log.LstdFlags)
	// l.Logfile = flog

	// create the logger to log to the file and standard out
	multiwrite := io.MultiWriter(file, os.Stdout)
	l.Multi = log.New(multiwrite, "", log.LstdFlags)

        // logger := log.New(f, "INFO: ", log.Lshortfile)
        // l.Logfile = log.New(f, "", log.LstdFlags)

        l.Printlog("test logged only")
        l.Println2("test2 log and stand out")
	l.Logfile.Println("test3 logged only")
	l.Multi.Println("test4 log and stand out")
*/
}

// writes to the logger only
func (l *fileData) Printlog (v ...interface{}) {
	////l.Logfile.Println(v...)
	// fmt.Println("test fdata println ", v)
	// fmt.Println(v...)
	// h := l.Logfile
	// l.Logfile.Println(v...)
	// l.Multi.Println(v...)
	l.Logfile.Println(v...)

}

// writes to the logger and standard out
// line  ...interface{}
func (l *fileData) Println (v ...interface{}) {
	// fmt.Println("test fdata println2 ", v)
        // l.Multi.Println(v...)
        l.Multi.Println(v...)
        // l.Multi.Println("konishei")

}
