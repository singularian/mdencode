package mdZipFile

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"hash"
	"regexp"
	"encoding/hex"
	"fmt"
	"github.com/singularian/mdencode/code/decode/mdFormatBinary"
	"github.com/singularian/mdencode/code/decode/mdHashContextList"
	"io"
	"log"
	"math/big"
	"os"
	"path/filepath"
	_ "sort"
	"strconv"
	"strings"
)

// mdformat interface struct
type mdformat interface {
	PrintFormatType()
	OpenFile(appendfile bool)
	InitFile()
	// EncodeDirStart() // for the directory
	EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, keylist string, modulusSize int64)
	EncodeFileHash(encodingFormat int, hashName string, hashBytes string)
	EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod *big.Int)
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
	// byte block
	byteblock bool
	// key string
	key string
	hwkey string
	keylist string
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
	hashListFile map[string]hash.Hash
	// hash list for blocks
	hashListBlocks map[string]hash.Hash
	// byte buffer
	filebuffer []byte
	// modulus
	modExp                 int
	fileblockmodulusString string
	fileBlockModulus *big.Int
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

// Init returns a new mdEncode mdZipFile object
func Init() (md *FileData) {

	mdata := new(FileData)
	mdata.byteblock    = false
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
	fdata.mdfmt.EncodeFileHeader(format, fileBaseName, fdata.filePath, size, blocksize, fdata.fileHashListNames, fdata.blockHashListNames, fdata.keylist, bitsize)

	// create a function to hash the entire file as one signature or multiple digital hash signatures
	// then collisions between the blocks can be resolved because the correct ones will fit together and be verified by the top level signature or signature group
	// can be a combination of 2 to 30 or more file signatures
	//
	// All combinations of blocks found by the block modulus scan signatures can be tested with the file signatures to find the correct blocks and resolve any collisions in the signatures
	//
	// The cost of the file block can be ammortized over the entire file
	//
	// could also have a mod2 encoding file / block mod 2 with exponent floor
	fdata.mdencodeFile(fileName)

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
		h := l.hashListFile[hashvalue]
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
		h := l.hashListFile[hashvalue]
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
// this creates a file block hash signature with a group of file byte blocks with a modular floor
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

		// if the modulus bitsize is greater than zero calculate the byte block modulus
		// otherwise skip generating the byte block modulus
		if bitsize > 0 {
			// create the biginteger representation of the bytes
			blockBigInt := new(big.Int)
			blockBigInt.SetBytes(l.filebuffer)

			l.calculateFileBlockModulus(blockBigInt, modulusBigInt)
		} else {
			l.modExp = 0
			l.fileblockmodulusString = "0"
			// blockBigInt := new(big.Int)
			//blockBigInt = l.fileBlockModulus
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
		// changing fileblockmodulusString to bigint
		/////////l.mdfmt.EncodeBlock(format, bytesRead, hlistarray, l.modExp, l.fileblockmodulusString)
		l.mdfmt.EncodeBlock(format, bytesRead, hlistarray, l.modExp, l.fileBlockModulus)
		fmt.Println("block number ", blocksRead, bytesRead, hlistarray, l.modExp, l.fileblockmodulusString, modSize)
		blocksRead++
		hlistarray = hlistarray[0:0]
	}

	return 0

}


// createHashListMap
// this method converts a binary signature list into a hash context list
func (l *FileData) createHashListMap(fileBlockflag int) {

	// file signature variables
        var hlistarray []string
        if fileBlockflag == 0 {
                hlistarray = l.fileHashListArray

        } else if fileBlockflag == 1 {
                hlistarray = l.blockHashListArray
        }
        var length = len(hlistarray)
        var last = 34
        if length < last {
                last = length
        }

	// initialize the Hash Context List
	// this also creates the mdBinaryList object
	mdc := mdHashContextList.Init()

	// ==================================================
	// set the key
	// this is for blake2s and siphash
	// mdc.SetKeyFile(l.key)
	mdc.SetHighwayKey(l.hwkey)
	mdc.SetHashListKey(l.keylist)
	// ==================================================

 	x := strings.Join(hlistarray, "")
	// list  := mdc.MdBlockSize.CreateHashBlockList(hlistarray)

	list  := mdc.MdBlockSize.CreateHashBlockList(x)
	result := strings.Join(list, ":")

//	fmt.Println("new list ", list)
//	fmt.Println("new list ", result)

	// func (hc *HashContextList) CreateHashListMap(hashList string, mdtype int, threadNumber int) {
	//// mdc.CreateHashListMap(list, fileBlockflag, 1)
	// 0 is file 
	// 1 is block

	if fileBlockflag == 0 {
		mdc.CreateHashListMap(result, 0, 1)
	} else {
		mdc.CreateHashListMap(result, 1, 1)
	}

	// mdc.CreateHashListMap(result, fileBlockflag, 1)

/*	for k, _ := range  mdc.HashListBlocks {
                hashname := k
		// list = append(list, hashname)
		fmt.Println("hash list contexts ", hashname)
	}
*/

	// mm  := mdc.GetBlockHash()

	// sort.Strings(list)

	// hc.HashListFile
	// hc.HashListBlocks
	// 0 is file
	// 1 is block
	if fileBlockflag == 0 {
		l.fileHashListNames = list
		l.hashListFile = mdc.HashList
	} else {
		l.blockHashListNames = list
		l.hashListBlocks = mdc.HashListBlocks
	}

}

// setmdFormat
// sets the correct md format object
func (l *FileData) setmdFormat(format int) {

	l.mdFormat = format

	// binary Formatter
	// if format == 1000 {
	if l.outputFileName == "" {
		l.outputFileName = "default.mdbin"
	}
	l.mdfmt = mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)
	l.mdfmt.OpenFile(false)
	l.mdfmt.InitFile()
	return
	// }

	l.mdfmt.InitFile()

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

// SetKeyFile
// set the md key
// this defaults to a default key if the key is less than 16 bytes
// one of the hash libs faults if the key is to small
// some of the signatures use a key
func (l *FileData) SetKeyFile(key string) {
	l.key = key
}

// SetHWKeyFile
// set the high way hash key
// need to make these a hash map
func (l *FileData) SetHWKeyFile(key string) {
	l.hwkey = key
}

// SetKeyList
// Set the Signature List
// keylist=aeshash:083948304830948302332,ax1:0284923402934,ax2:03809903
// ax:blake2s_128-aes8:ax
// ax:blake2s_128-aes8:ax-key
// ax:blake2s_128-aes8:ax-key-bg
func (l *FileData) SetKeyList(keylist string) {


	// re1 := regexp.MustCompile("^([A-Za-z0-9]+[:][[:xdigit:]]+)+$")
	// re2 := regexp.MustCompile("^([A-Za-z0-9]+[:][[:xdigit:]]+[,])+$")
	re := regexp.MustCompile("^([A-Za-z0-9]+[:][[:xdigit:]]+)?([A-Za-z0-9]+[:][[:xdigit:]]+[,])*$")
        matched := re.MatchString(keylist)
        // matched2 := re2.MatchString(keylist)

	if matched {
                l.keylist = keylist
	//	return
	// } else if matched2 {
	//	l.keylist = keylist
	//	return
        } else if !matched {
		if keylist != "" {
			fmt.Println("Invalid Keylist ", keylist)
			os.Exit(1)
		}

	}

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

// SetProcessDirectory
func (l *FileData) SetProcessDir(dir bool) {
//	l.procDir = dir

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
        l.hashListFile = make(map[string]hash.Hash) // this is for the entire file
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
	l.fileBlockModulus = new(big.Int)
	l.fileBlockModulus = fileblockmodulus
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
	fmt.Println("mdZip")

}
