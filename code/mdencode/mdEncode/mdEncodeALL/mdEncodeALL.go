package mdEncodeALL

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"crypto/hmac"
	"crypto/md5"
	"crypto/sha1"
	"crypto/sha256"
	"crypto/sha512"
	"github.com/codahale/blake2"
	"golang.org/x/crypto/md4"
	"hash"
	"hash/fnv"
	"regexp"
	// "golang.org/x/crypto/blake2b"
	"encoding/hex"
	"fmt"
	_ "github.com/aead/siphash"
	"github.com/aead/skein"
	"github.com/asadmshah/murmur3"
	"github.com/cxmcc/tiger"
	"github.com/dchest/siphash"
	_ "github.com/enceve/crypto/skein"
	"github.com/jzelinskie/whirlpool"
	_ "github.com/mimoo/GoKangarooTwelve/K12"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatBinary"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatCSV"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatInform"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatJson"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatMD"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatSQL"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatText"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatHtml"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatXML"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatXMLgo"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatLog"
	"github.com/singularian/mdencode/code/mdencode/mdFormats/mdFormatImg"
	_ "github.com/spaolacci/murmur3"
	"github.com/steakknife/keccak"
	_ "github.com/twmb/murmur3"
	"golang.org/x/crypto/blake2s"
	"golang.org/x/crypto/ripemd160"
	"golang.org/x/crypto/sha3"
	"io"
	"log"
	"math/big"
	"os"
	"path/filepath"
	"sort"
	"strconv"
	"strings"
)

// mdformat interface struct
type mdformat interface {
	PrintFormatType()
	OpenFile(appendfile bool)
	InitFile()
	// EncodeDirStart() // for the directory
	EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64)
	EncodeFileHash(encodingFormat int, hashName string, hashBytes string)
	EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string)
	EncodeEndFile(encodingFormat int)
	// EncodeDirEnd() // for the directory end - has a list of files
}

// mdencode struct
type FileData struct {
	commandArgs    int
	fileName       string
	filePath       string
	outputFileName string
	fileSize       uint64
	blockSize      uint64
	blockCount     uint64
	blockRemainder uint64
	modSize        uint64
	modExponent    uint64
	mdFormat       int
	mdVersion      string
	// append file
	appendfile bool
	// byte block
	byteblock bool
	// byte block bigint
	byteblockint bool
	// filehashline
	filehashline bool
	// key string
	key string
	// logfile
	logfile string
	// argument hash list bits
	fileHashListString  string
	blockHashListString string
	// argument hash list bit arrays
	fileHashListArray  []string
	blockHashListArray []string
	// argument signature hash list names
	fileHashListNames  []string
	blockHashListNames []string
	// dictionary
	dictionary map[string]string
	// hash list for files
	hashList map[string]hash.Hash
	// hash list for blocks
	hashListBlocks map[string]hash.Hash
	// byte buffer
	filebuffer []byte
	// modulus
	modExp                 int
	fileblockmodulusString string
	// modulus
	// N *big.Int // modulus
	// https://stackoverflow.com/questions/37502134/declaring-type-big-int-overflowing-constant-golang
	// formatter
	// md *mdFormatText.MdFormat
	// mdformat interface for modular type assignment
	mdfmt mdformat
	// log writer
	log *log.Logger
	islogging bool
}

// Init returns a new mdEncode object
func Init() (md *FileData) {

	mdata := new(FileData)
	mdata.appendfile   = false
	mdata.byteblock    = false
	mdata.byteblockint = false
	mdata.filehashline = false
	mdata.key          = "LomaLindaSanSerento9000"
	mdata.logfile      = ""
	mdata.islogging    = false

	// need to set the format here???
	// there are advantage to not have mdencode init everything
        // setup the file md formatter
        // fdata.setmdFormat(format)

	return mdata
}

// MdencodeDirectory
// mdencode the directory non recursivelly with a array list
func (fdata *FileData) MdencodeDirectory(blockSize string, modSize string, format int, fileHashList string, blockHashList string, directoryName string, outputfileName string) int {

	// need to clean up the output file check a little bit
        // find the output filename file path
        //////////////////////////////outputpath, _ := filepath.Abs(fd.outputfilename)
        // find the fileData file directory
        // dir, _ := filepath.Split(outputpath)
        // fmt.Println("output file = ", outputpath, " ", dir)

        fileList := make([]string, 0)
        e := filepath.Walk(directoryName, func(path string, f os.FileInfo, err error) error {
                if stat, err := os.Stat(path); err == nil && !stat.IsDir() {
                        fileList = append(fileList, path)
                }
                return err
        })

        if e != nil {
                panic(e)
        }

        for _, fileName := range fileList {
                ///// fmt.Println(fileName)
                // skip the output file if it is specified
                // if ((fileName != outputpath) && (fd.outputfilename != "")) {
                // might be bug here???
                        // fdata.Mdencode(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fileName, fd.outputfilename)
                        fdata.MdencodeFile(blockSize, modSize, format, fileHashList, blockHashList, fileName, outputfileName)
                //}
        }

	return 0

}

// test directory hash code recursivelly
/* func (fd *FileData) hashDirectoryRecursive(directory string) {
        files, _ := ioutil.ReadDir(directory)
        for _, file := range files {
                fileName := directory + string(os.PathSeparator) + file.Name()
                // todo: need to skip the output file so it doesn't process it in the directory
                if file.IsDir() {
                        fd.hashDirectoryRecursive(fileName)
                } else {
                        fd.md.MdencodeFile(fd.blocksize, fd.modsize, fd.defaultFormat, fd.fhashlist, fd.bhashlist, fileName, fd.outputfilename)
                }
        }

} */


// Mdencode
// mdencode the file
//
// mdencode creates an optional top file level signature list
// mdencode creates an optional file block signature with a group of file blocks with a modular floor
//
// in one alternative format it can have a single signature with a x-bit collision number to differentiate between collisions
// ie sha-256 and a 128-bit collision number to differentiate the correct collision or block with an equal signature and a modular floor
// in another case there could be a signature pair md5 or sha1 and a modular floor and a 1 to 4 byte collision integer to differentiate the correct collision
// the correct collision is just the nth block with the same signature
func (fdata *FileData) MdencodeFile(blockSize string, modSize string, format int, fileHashList string, blockHashList string, fileName string, outputfileName string) int {

	// check if the input file is a directory
	// and throw an error
	// mdencode doesn't currently handle directories
	// probably could change this to handle directories
	if info, err := os.Stat(fileName); err == nil && info.IsDir() {
		log.Fatal("The mdfile is a directory")
		os.Exit(3)
	}

	// process the blocksize argument
	blocksize, _ := strconv.ParseInt(blockSize, 10, 64)

	// process the modulus bitsize argument
	bitsize, _ := strconv.ParseInt(modSize, 10, 64)

	// set the filename
	// remove the directory path if it is prefixed before the directory
	fileBaseName := filepath.Base(fileName)
	// set the filename
	fdata.fileName = fileName
	// find the fileData file path
	path, _ := filepath.Abs(fileName)
	// find the fileData file directory
	dir, _ := filepath.Split(path)
	fdata.filePath = dir
	// set the output filename
	fdata.outputFileName = outputfileName
	// initialize the blocksize and modsize
	fdata.blockSize = uint64(blocksize)
	fdata.modSize = uint64(bitsize)

	// get the file size
	fi, e := os.Stat(fileName)
	if e != nil {
		fmt.Println(e)
		os.Exit(3)
	}

	// log file
	// should close the log at the end???
	fdata.Printlog("mdencode file ", fileName, " blocksize ", fdata.blockSize, " modsize ", fdata.modSize)

	// set the file size
	size := fi.Size()
	fdata.fileSize = uint64(size)

	// encode the File Header
	fdata.mdfmt.EncodeFileHeader(format, fileBaseName, fdata.filePath, size, blocksize, fdata.fileHashListNames, fdata.blockHashListNames, bitsize)

	// create a function to hash the entire file as one signature or multiple digital hash signatures
	// then collisions between the blocks can be resolved because the correct ones will fit together and be verified by the top level signature or signature group
	// can be a combination of 2 to 30 or more file signatures
	//
	// All combinations of blocks found by the block modulus scan signatures can be tested with the file signatures to find the correct blocks and resolve any collisions in the signatures
	//
	// The cost of the file block can be ammortized over the entire file
	//
	// could also have a mod2 encoding file / block mod 2 with exponent floor
	if fdata.filehashline {
		fdata.mdencodeFileHashLine(fileName)
	} else {
		fdata.mdencodeFile(fileName)
	}

	// encode the file blocks
	if blocksize > 0 {
	        // set the number of blocks in the file and last block size
		blocks, remainder := fdata.calculateFileBlocks(uint64(size), uint64(blocksize))
		fdata.blockCount = uint64(blocks)
		fdata.blockRemainder = uint64(remainder)

		// mdEncode the File Block
		fdata.mdencodeFileBlock(blockSize, modSize, format, fileName)
	}

	// endcode the end of file formatter
	fdata.mdfmt.EncodeEndFile(format)

	return 0

}

// mdencodeFile
//
// add a top level file signature or group of signatures
// this will resolve any collisions in the block signatures
//
// it allows a modulus scan to find all the blocks matching a signature group and modulus
// then the top level file signature can validate against all the combinations of the blocks matched
// and find the correct one
//
// it can be a combination of 1 to 10+ signatures and a file modulus
// like ripe160, sha1, sha512, md5, md6, md4 etc
// this makes it very collision resistant
func (l *FileData) mdencodeFile(fileName string) {
	f, err := os.Open(fileName)
	if err != nil {
		// log.Fatal(err)
		fmt.Println(err)
	}
	defer f.Close()

	// this is not in sorted order
	// hash list
	for _, hashvalue := range l.fileHashListNames {
		h := l.hashList[hashvalue]
		if _, err := io.Copy(h, f); err != nil {
			fmt.Println(err)
		}
		l.mdfmt.EncodeFileHash(l.mdFormat, hashvalue, hex.EncodeToString(h.Sum(nil)))
	}

}

// mdencodeFileHashLine
// encode the file hash list as one line
func (l *FileData) mdencodeFileHashLine(fileName string) {
	f, err := os.Open(fileName)
	if err != nil {
		fmt.Println(err)
	}
	defer f.Close()

	// this is not in sorted order
	// hash list
	var hlistarray []string
	for _, hashvalue := range l.fileHashListNames {
		h := l.hashList[hashvalue]
		if _, err := io.Copy(h, f); err != nil {
			fmt.Println(err)
		}
		hlistarray = append(hlistarray, hex.EncodeToString(h.Sum(nil)))
	}
	var hashListString = strings.Join(l.fileHashListNames, ":")
	var hashListHex = strings.Join(hlistarray, ":")
	l.mdfmt.EncodeFileHash(l.mdFormat, hashListString, hashListHex)

}

// mdencodeFileBlock
// mdencode the file blocks
// this creates a file block hash signature with a group of file byte blocks with a modular floora
// they are n-byte sized file blocks with a modulus and a modular floor
func (l *FileData) mdencodeFileBlock(blockSize string, modSize string, format int, fileName string) int {

	// process the blocksize argument
	blocksize, err := strconv.ParseInt(blockSize, 10, 64)

	// process the modulus bitsize argument
	bitsize, err := strconv.ParseInt(modSize, 10, 64)

	// create the modulus bigint 2 to the bitsize exponent
	var modulusBigInt, e = big.NewInt(2), big.NewInt(bitsize)
	modulusBigInt = modulusBigInt.Exp(modulusBigInt, e, nil)

	// Open the file for reading
	file, err := os.Open(fileName)
	if err != nil {
		fmt.Println("Cannot find file: ", fileName)
		return 3
	}

	defer file.Close()

	// intialize the buffer with the default blocksize
	l.filebuffer = make([]byte, blocksize)

	// setup the file block count and last block remainder
	blocks := l.blockCount
	remainder := l.blockRemainder

	var blocksRead uint64 = 1
	var bytesRead uint64 = uint64(blocksize)

	var hashsize = len(l.blockHashListNames)
	if hashsize < 1 {
		return 0
	}

	var hlistarray []string

	// iterate through the file blocks and generate the hash signature list
	// append them to the hlistarray
	for {

		// break on end of file
		if _, err := file.Read(l.filebuffer); err == io.EOF {
			break
		}

		if err != nil {
			fmt.Println("readblock error: ", err)
			return 3
		}

		if blocksRead == blocks {
			l.filebuffer = l.filebuffer[0:remainder]
			bytesRead = remainder
		}

		// if the byteblock is specified add it to the hash list
		if l.byteblock {
			var bufstring = fmt.Sprint(l.filebuffer)
			hlistarray = append(hlistarray, bufstring)
		}

		// if the modulus bitsize is greater than zero calculate the byte block modulus
		// otherwise skip generating the byte block modulus
		if bitsize > 0 {
			// create the biginteger representation of the bytes
			blockBigInt := new(big.Int)
			blockBigInt.SetBytes(l.filebuffer)

			// if the byteblock bigint option is specified add it to the hash list
			if l.byteblockint {
				blockbytesBigInt := blockBigInt.String()
				// hlistarray = append(hlistarray, ":")
				hlistarray = append(hlistarray, blockbytesBigInt)
			}

			l.calculateFileBlockModulus(blockBigInt, modulusBigInt)
		} else {
			l.modExp = 0
			l.fileblockmodulusString = "0"
		}

		// generate the file block signatures from the hash list
		for _, hashvalue := range l.blockHashListNames {
			// if the hashname is not block treat it as a hash context
			if hashvalue != "block" {
				h := l.hashListBlocks[hashvalue]
				h.Write([]byte(l.filebuffer))
				hlistarray = append(hlistarray, hex.EncodeToString(h.Sum(nil)))
				h.Reset()
			}
		}
		// generate the block message digest signature with the formatter
		l.mdfmt.EncodeBlock(format, bytesRead, hlistarray, l.modExp, l.fileblockmodulusString)
		blocksRead++
		hlistarray = hlistarray[0:0]
	}

	return 0

}

// createHashListMap
// create the hash context hash list map
func (l *FileData) createHashListMap(fileBlockflag int) {

	var defaultkey = []byte("LomaLindaSanSerento9000")
	var keystring = l.key
	key := []byte(keystring)
	if len(keystring) < 15 {
		key = defaultkey
	}

	// file signature variables
	var hlistarray []string
	if fileBlockflag == 0 {
		hlistarray = l.fileHashListArray

	} else if fileBlockflag == 1 {
		hlistarray = l.blockHashListArray
	}
	var length = len(hlistarray)
	var last = 30
	if length < last {
		last = length
	}

	// file block sig variables
	// l.blockHashListArray
	// l.hashListBlocks
	// l.blockHashListNames
	hb := make(map[string]hash.Hash)

	var list []string
	for i := 0; i < last; i++ {
		var vbool, _ = strconv.Atoi(hlistarray[i])
		var v = (i * 10) + vbool

		switch v {
		// md4 file
		case 1:
			hb["md4"] = md4.New()
		// md5 file
		case 11:
			hb["md5"] = md5.New()
		// sha1 file
		case 21:
			hb["sha1"] = sha1.New()
		// sha256
		case 31:
			hb["sha224"] = sha256.New224()
		case 41:
			hb["sha256"] = sha256.New()
		// sha512_224
		case 51:
			hb["sha512_224"] = sha512.New512_224()
		case 61:
			hb["sha512_256"] = sha512.New512_256()
		// sha512
		case 71:
			hb["sha512"] = sha512.New()
		// sha3_224
		case 81:
			hb["sha3_224"] = sha3.New224()
		// sha3_256
		case 91:
			hb["sha3_256"] = sha3.New256()
		// sha_384
		case 101:
			hb["sha3_384"] = sha3.New384()
		// sha3_512
		case 111:
			hb["sha3_512"] = sha3.New512()
		// blake2b.New256
		case 121:
			hb["blake2b"] = blake2.NewBlake2B()
		case 131:
			hb["fnv"] = fnv.New64a()
		case 141:
			hb["ripe160"] = ripemd160.New()
		case 151:
			var seed uint64 = 1120322
			hb["murmur3"] = murmur3.New128(seed)
		case 161:
			hb["whirlpool"] = whirlpool.New()
		case 171:
			hb["hmac256"] = hmac.New(sha256.New, key)
		case 181:
			hb["hmac512"] = hmac.New(sha512.New, key)
		case 191:
			hb["kekkak"] = keccak.New256()
		case 201:
			hb["skein160"] = skein.New(20, nil)
		case 211:
			hb["skein256"] = skein.New256(key)
		case 221:
			hb["skein384"] = skein.New(48, nil)
		case 231:
			hb["skein512"] = skein.New512(key)
		case 241:
			hb["skein1024"] = skein.New(128, nil)
		case 251:
			hb["tiger"] = tiger.New()
		// siphash has to have a key 8 or 16 bytes
		case 261:
			hb["siphash"] = siphash.New128(key)
		// blake2s needs two values since it has a multiple return
		case 271:
			hb["blake2s_128"], _ = blake2s.New128(key)
		case 281:
			hb["blake2s_256"], _ = blake2s.New256(key)
		case 291:
			hb["blake2"] = blake2.New(nil)
		// case 301:
		// hb["block"] = fnv.New64a()
		// hb["blake2s"], _ =  blake2s.New128(nil)

		}
	}

	for k, _ := range hb {
		hashname := k
		list = append(list, hashname)
	}

	sort.Strings(list)

	if fileBlockflag == 1 {
		l.blockHashListNames = list
		l.hashListBlocks = hb
	} else {
		l.fileHashListNames = list
		l.hashList = hb
	}
}

// setmdFormat
// sets the correct md format object
func (l *FileData) setmdFormat(format int) {

	l.mdFormat = format

	// CSV formatter
	if format == 101 || format == 102 {
		l.mdfmt = mdFormatCSV.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		l.mdfmt.OpenFile(l.appendfile)
	// binary Formatter
	} else if format == 1000 {
		// l.mdfmt = mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		if l.outputFileName == "" {
			l.outputFileName = "default.mdbin"
		}
		l.mdfmt = mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		l.mdfmt.OpenFile(false)
		l.mdfmt.InitFile()
		return
	// SQL Formatter
	} else if format == 2000 {
		// set a default filename if none is provided
		if l.outputFileName == "" {
			l.outputFileName = "default"
		}
		l.outputFileName = l.outputFileName + ".db"
		l.mdfmt = mdFormatSQL.Init(101, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
	// Inform Formatter
	} else if format == 3000 {
		l.mdfmt = mdFormatInform.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		l.mdfmt.OpenFile(l.appendfile)
	// JSON Formatter
	} else if format == 4000 {
		l.mdfmt = mdFormatJson.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		l.mdfmt.OpenFile(l.appendfile)
	// MD Formatter
	} else if format == 4300 {
		l.mdfmt = mdFormatMD.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
                l.mdfmt.OpenFile(l.appendfile)
	// HTML Formatter
	} else if format == 4500 {
               l.mdfmt = mdFormatHtml.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
               l.mdfmt.OpenFile(l.appendfile)
	// GO XML Formatter
	} else if format == 5000 {
		l.mdfmt = mdFormatXMLgo.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		l.mdfmt.OpenFile(l.appendfile)
	// non GO XML Formatter
	} else if format == 5001 {
                l.mdfmt = mdFormatXML.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
                l.mdfmt.OpenFile(l.appendfile)
	// log Formatter
	} else if format == 6000 {
		l.mdfmt = mdFormatLog.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
	// Image Formatter
	} else if format == 8000 || format == 8001 {
		l.mdfmt = mdFormatImg.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
	// Text Formatter
	} else {
		l.mdfmt = mdFormatText.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
		l.mdfmt.OpenFile(l.appendfile)
	}

	l.mdfmt.InitFile()

}

// create an empty sqlite3 signature db
func (l *FileData) InitDB(fileName string) {

	if fileName == "" {
		return
	} 

	if l.outputFileName == fileName {
		fileName = fileName + ".2"
	}

	md := mdFormatSQL.Init(0, fileName, "", 0, 0, 0, "", "", fileName)
	md.InitFile()
	md.EncodeEndFile(0)

}

// initialize the logfile
func (l *FileData) initLog() {

	if l.logfile != "" {
		var logfilename = l.logfile
		// var logfile = md.logfile

		logfile, err := os.OpenFile(logfilename, os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0666)
		if err != nil {
			fmt.Println("Failed to open log file:", err)
		}
		// using defer close causes it to close the logfile
		// and not write
		// defer logfile.Close()
		// l.Logfile = log.New(logfile, "", log.Ldate|log.Ltime)
		l.log = log.New(logfile, "", log.Ldate|log.Ltime)
		l.islogging = true
	}

}

// SetByteBlock
// set the byte block mode
func (l *FileData) SetByteBlock(byteblock bool) {
	l.byteblock = byteblock
}

// SetByteBlockBigInt
// set the byte block mode
func (l *FileData) SetByteBlockBigInt(byteblockint bool) {
	l.byteblockint = byteblockint
}

// SetAppendFile
// set the append file mode
func (l *FileData) SetAppendFile(appendfile bool) {
	l.appendfile = appendfile
}

// SetFileHashLine
// set the file hash format
// if true it will write or display the file hash list as one line
func (l *FileData) SetFileHashLine(filehashline bool) {
	l.filehashline = filehashline
}

// SetKeyFile
// set the md key
// this defaults to a default key if the key is less than 16 bytes
// one of the hash libs faults if the key is to small
// some of the signatures use a key
func (l *FileData) SetKeyFile(key string) {
	l.key = key
}

// SetLogFile
// set the optional logfile
func (l *FileData) SetLogFile(logfile string) {
	l.logfile = logfile
	l.initLog()

        // setup the file md formatter
	// possibly add the logfile if it is set
	// need to set this for the mdFormat 
        l.setmdFormat(l.mdFormat)
}

// SetOutputFile
func (l *FileData) SetOutputFile(outputfile string) {

	l.outputFileName = outputfile

}

// SetMdFormat
// set the md output format
func (l *FileData) SetMdFormat(format int) {

        l.setmdFormat(format)
}

// setHashLists
// this sets the block hash list and file hash list maps
// it also set the block hash list contexts and file hash list context maps
func (l *FileData) SetHashLists(fileHashList string, blockHashList string) {

	// process the hash list arguments
        l.fileHashListString = fileHashList
        l.blockHashListString = blockHashList
        // hash list regex
        re := regexp.MustCompile("[01]")

        l.fileHashListArray = re.FindAllString(fileHashList, -1)
        l.blockHashListArray = re.FindAllString(blockHashList, -1)

        // initialize the map
        l.dictionary = make(map[string]string)
        // initialize the hash list maps
        l.hashList = make(map[string]hash.Hash) // this is for the entire file
        l.hashListBlocks = make(map[string]hash.Hash)

        // create the Hash List Map
        l.createHashListMap(0) // files
        l.createHashListMap(1) // blocks
}

// logNrecursive
// calculate the bigint log exponent recursivelly
// the bitsize exponent less than the blocksize big integer
func (l *FileData) logNrecursive(x *big.Int, base *big.Int) int {

	gt := x.Cmp(base)

	if gt < 0 {
		return 0
	}
	z := new(big.Int).Quo(x, base)
	return 1 + l.logNrecursive(z, base)

}

// logN
// calculate the bigint log exponent
// the bitsize exponent less than the blocksize big integer
// faster than the recursive version
func (l *FileData) logN(fileblockint *big.Int, base *big.Int) int {

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

// calculateFileBlockModulus
// calculate the file byte block bigint modulus exponent with base 2 currently
// ie mod^151 and less than mod^152
// it could use the block modulus as the base but two is a larger base
//
// calculate the file byte block bigint modulus remainder
func (l *FileData) calculateFileBlockModulus(blockBigInt *big.Int, modulusBigInt *big.Int) {

	// calclute the modulus exponent
	two := big.NewInt(2)
	l.modExp = l.logN(blockBigInt, two)

	// calculate the modulus remainder
	fileblockmodulus := new(big.Int)
	fileblockmodulus = fileblockmodulus.Mod(blockBigInt, modulusBigInt)
	l.fileblockmodulusString = fileblockmodulus.String()

}

// calculate the number of file blocks
func (l *FileData) calculateFileBlocks(fileSize uint64, blockSize uint64) (uint64, uint64) {

	remainder := fileSize % blockSize
	var blocksCount uint64 = 0
	if remainder == 0 {
		blocksCount = fileSize / blockSize
		remainder = blockSize
	} else {

		blocksCount = (fileSize / blockSize) + 1
	}

	return blocksCount, remainder
}

// write to the log
func (l *FileData) Printlog (v ...interface{}) {

	if l.islogging {
		l.log.Println(v...)
	}

}

// display the object type
func (l *FileData) PrintType() {
	fmt.Println("mdencodeALL")

}
