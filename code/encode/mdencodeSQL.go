package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"crypto/md5"
	"crypto/sha1"
	"crypto/sha256"
	"crypto/sha512"
	"golang.org/x/crypto/md4"
	"hash"
	"hash/fnv"
	"regexp"
	"crypto/hmac"
	"github.com/codahale/blake2"
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
	_ "github.com/spaolacci/murmur3"
	"github.com/steakknife/keccak"
	_ "github.com/twmb/murmur3"
	"golang.org/x/crypto/blake2s"
	"golang.org/x/crypto/ripemd160"
	"golang.org/x/crypto/sha3"
	"io"
	"math/big"
	"strconv"
	"log"
	"os"
	"path/filepath"
	"sort"
	"github.com/singularian/mdencode/code/encode/mdFormats/mdFormatSQL"
	"github.com/singularian/mdencode/code/encode/mdReports/mdReportsSQL"
)

// mdformat interface struct
type mdformat interface {
        Println()
        OpenFile()
        EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) 
        EncodeFileHash(encodingFormat int, hashName string, hashBytes string)
        EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string)
        EncodeEndFile(encodingFormat int)
}

// mdencode struct
type fileData struct {
	commandArgs    int
	fileName       string
	filePath       string
	fileSize       uint64
	blockSize      uint64
	blockCount     uint64
	blockRemainder uint64
	modSize        uint64
	modExponent    uint64
	mdFormat       int
	mdVersion      string
        // argument hash list bits
        fileHashListString string
        blockHashListString string
	// argument hash list bits
	fileHashListArray  []string
	blockHashListArray []string
	// argument signature hash list
	fileHashListNames  []string
	blockHashListNames []string
	// dictionary
	dictionary map[string]string
	// hash list for files
	hashList map[string]hash.Hash
	// hash list for blocks
	hashListBlocks map[string]hash.Hash
	// md formatter
	// mdx *mdFormatText.MdFormat
	md *mdFormatSQL.MdFormat
	mdfmt mdformat
	// log writer
	log *log.Logger
}

var byteblock = 0

func main() {

	var hashlist string = "01100000000"

	var argsNumber int = len(os.Args)
	var mdname = os.Args[0]

	// sqlite3 db file signature generator

	if mdname == "mdencodeSQL" || mdname == "./mdencodeSQL" {
		argsSQL(hashlist, argsNumber)
	} else if mdname == "mdencodereport" || mdname == "./mdencodereport" {
		argsReport(argsNumber)
	}

	os.Exit(0)

}

// sql handler
// might have different xml formats
func argsSQL(hashlist string, argsNumber int) int {

	var outfilename = ""
	var defaultFormat = 10
	var extension = ".db"

	// mdencode file
	if argsNumber == 2 {
		filename := os.Args[1]
		blocksize := "20"
		modsize := "16"
		outfilename = filename + extension
		mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist, filename, outfilename)
		// mdencode 60 24 file
	} else if argsNumber == 4 {
		blocksize := os.Args[1]
		modsize := os.Args[2]
		filename := os.Args[3]
		outfilename = filename + extension
		mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist, filename, outfilename)
		// mdencode 60 24 0110000001 0110000001 file
	} else if argsNumber == 6 {
		blocksize := os.Args[1]
		modsize := os.Args[2]
		hashsize1 := os.Args[3]
		hashsize2 := os.Args[4]
		filename := os.Args[5]
		outfilename = filename + extension
		mdencode(blocksize, modsize, defaultFormat, hashsize1, hashsize2, filename, outfilename)
		// mdencode 60 24 0110000001 0110000001 mdfile outputfile
	} else if argsNumber == 7 {
		blocksize := os.Args[1]
		modsize := os.Args[2]
		hashsize1 := os.Args[3]
		hashsize2 := os.Args[4]
		filename := os.Args[5]
		outfilename = os.Args[6]

		if filename == outfilename {
			outfilename = outfilename + extension
		}

		mdencode(blocksize, modsize, defaultFormat, hashsize1, hashsize2, filename, outfilename)
	} else {
		fmt.Println("\nUsage: mdencodeSQL [filename]")
		fmt.Println("Usage: mdencodeSQL [blocksize] [modulus bitsize] [filename]")
		fmt.Println("Usage: mdencodeSQL [blocksize] [modulus bitsize] [file signatures bitstring] [block signatures bitstring] [filename]\n")
		fmt.Println("Usage: mdencodeSQL [blocksize] [modulus bitsize] [01]+ [01]+ [filename]")
		fmt.Println("Usage: mdencodeSQL [blocksize] [modulus bitsize] [01]+ [01]+ [filename] [outputdbfile]")
		fmt.Println("\n")
		fmt.Println("Examples:")
		fmt.Println("mdencodeSQL file")
		fmt.Println("mdencodeSQL 60 24 file")
		fmt.Println("mdencodeSQL 60 24 1 0 file")
		fmt.Println("mdencodeSQL 60 24 0 1 file")
		fmt.Println("mdencodeSQL 60 24 0110000001 0110000001 file")
		fmt.Println("mdencodeSQL 60 24 0110000001 0110000001 file dbfile\n")
		fmt.Println("Version: 1.0 復甦 復活\n")
	}

	return 0

}

/* func argsReport(argsNumber int) {

	// var outfilename = ""
	var defaultFormat = 9 

	// if just the file is given display the file mdencode list
	// make 0 display everything
	if argsNumber == 2 {
		filename := os.Args[1]
		if _, err := os.Stat(filename); os.IsNotExist(err) {
			log.Fatal("The mdfile db does not exist")
			os.Exit(3)
		}
		fdata := fileData{}
		fdata.md = mdFormatSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		fdata.md.PrintFileList(1, 0)

	// mdencodereport filename fileid
	} else if argsNumber == 3 {
		filename := os.Args[1]
		fileid := os.Args[2]
		fileInt, _ := strconv.ParseUint(fileid, 10, 64)

		if _, err := os.Stat(filename); os.IsNotExist(err) {
			log.Fatal("The mdfile db does not exist")
			os.Exit(3)
		}

		fdata := fileData{}
		fdata.md = mdFormatSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
		if fileid != "0" {
			fdata.md.PrintReport(defaultFormat, "filename", fileInt)
			// display the entire file list
		} else {
			fdata.md.PrintEntireFileList(defaultFormat)
		}
	// mdencode filename fileid format
	} else if argsNumber == 4 {
		filename := os.Args[1]
		fileid := os.Args[2]
		format := os.Args[3]
		fileInt, _ := strconv.ParseUint(fileid, 10, 64)

		fdata := fileData{}
		formatInt, _ := strconv.Atoi(format)
		fdata.md = mdFormatSQL.Init(formatInt, filename, "", 0, 0, 0, "0", "0", filename)
		fdata.md.PrintReport(formatInt, "filename", fileInt)
		// fdata.md.EncodeEndFile(formatInt)

	} else {
		// display the file list
		fmt.Println("\nUsage: mdencodereport [filename]")
		// dump everything from the db in the default format
		fmt.Println("Usage: mdencodereport [filename] 0")
		// display the mdfile with a db mdfileid number
		fmt.Println("Usage: mdencodereport [filename] [fileid]")
		fmt.Println("Usage: mdencodereport [filename] [fileid] [format]")
	}

}
*/

func argsReport(argsNumber int) {

        // var outfilename = ""
        var defaultFormat = 9 

        // if just the file is given display the file mdencode list
        // make 0 display everything
        if argsNumber == 2 {
                filename := os.Args[1]
                if _, err := os.Stat(filename); os.IsNotExist(err) {
                        log.Fatal("The mdfile db does not exist")
                        os.Exit(3)
                }
                // fdata := fileData{}
                //fdata.md = mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
                md := mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
                md.PrintFileList(1, 0)

        // mdencodereport filename fileid
        } else if argsNumber == 3 {
                filename := os.Args[1]
                fileid := os.Args[2]
                fileInt, _ := strconv.ParseUint(fileid, 10, 64)

                if _, err := os.Stat(filename); os.IsNotExist(err) {
                        log.Fatal("The mdfile db does not exist")
                        os.Exit(3)
                }

                // fdata := fileData{}
                // fdata.md = mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
                md := mdReportSQL.Init(0, filename, "", 0, 0, 0, "0", "0", filename)
                if fileid != "0" {
                        // fdata.md.PrintReport(defaultFormat, "filename", fileInt)
                        md.PrintReport(defaultFormat, "filename", fileInt)
                        // display the entire file list
                } else {
                        // fdata.md.PrintEntireFileList(defaultFormat)
                        md.PrintEntireFileList(defaultFormat)
        }       
        // mdencode filename fileid format
        } else if argsNumber == 4 {
                filename := os.Args[1]
                fileid := os.Args[2]
                format := os.Args[3]
                fileInt, _ := strconv.ParseUint(fileid, 10, 64)

                // fdata := fileData{}
                formatInt, _ := strconv.Atoi(format)
                /// fdata.md = mdReportSQL.Init(formatInt, filename, "", 0, 0, 0, "0", "0", filename)
                ////fdata.md.PrintReport(formatInt, "filename", fileInt)
                md := mdReportSQL.Init(formatInt, filename, "", 0, 0, 0, "0", "0", filename)
                md.PrintReport(formatInt, "filename", fileInt)


                // fdata.md.EncodeEndFile(formatInt)

        } else {
                // display the file list
                fmt.Println("\nUsage: mdencodereport [filename]")
                // dump everything from the db in the default format
                fmt.Println("Usage: mdencodereport [filename] 0")
                // display the mdfile with a db mdfileid number
                fmt.Println("Usage: mdencodereport [filename] [fileid]")
                fmt.Println("Usage: mdencodereport [filename] [fileid] [format]")
        }

}

// mdencode the file
// this can generate a file level top signature
// this creates a file block signature with a group of file blocks with a modular floor
//
// in one alternative format it can have a single signature with a x-bit collision number to differentiate between collisions
// ie sha-256 and a 128-bit collision number to differentiate the correct collision or block with an equal signature and a modular floor
// in another case there could be a signature pair md5 or sha1 and a modular floor and a 1 to 4 byte collision integer to differentiate the correct collision
// the correct collision is just the nth block with the same signature
func mdencode(blockSize string, modSize string, format int, fileHashList string, blockHashList string, fileName string, outputfileName string) int {

	// check if the input file is a directory
	// and throw an error
	// mdencode doesn't currently handle directorys
	if info, err := os.Stat(fileName); err == nil && info.IsDir() {
		log.Fatal("The mdfile is a directory")
		os.Exit(3)
	}

	// process the blocksize argument
	blocksize, _ := strconv.ParseInt(blockSize, 10, 64)

	// process the modulus bitsize argument
	// exponent
	bitsize, _ := strconv.ParseInt(modSize, 10, 64)
	// var i, e = big.NewInt(2), big.NewInt(bitsize)
	// i.Exp(i, e, nil)

	// initialize fileData struct
	fdata := fileData{}
	// initialize the fileData struct information
	fdata.fileName = fileName
	// find the fileData file path
	path, _ := filepath.Abs(fileName)
	dir, _ := filepath.Split(path)
	fdata.filePath = dir
	// initialize the blocksize and modsize
	fdata.blockSize = uint64(blocksize)
	fdata.modSize = uint64(bitsize)
	// initalize the format
	fdata.mdFormat = format

	// process the hash list arguments
	fdata.fileHashListString  = fileHashList
        fdata.blockHashListString = blockHashList 
        // hash list regex
	re := regexp.MustCompile("[01]")

	fdata.fileHashListArray = re.FindAllString(fileHashList, -1)
	fdata.blockHashListArray = re.FindAllString(blockHashList, -1)

	// need to check the length of the arrays
	// fmt.Println(fdata.fileHashListArray, fdata.blockHashListArray)

	// initialize the map
	fdata.dictionary = make(map[string]string)
	// initialize the hash list maps
	fdata.hashList = make(map[string]hash.Hash) // this is for the entire file
	fdata.hashListBlocks = make(map[string]hash.Hash)

	// create the Hash List Map
	fdata.createHashListMap(0) // files
	fdata.createHashListMap(1) // blocks

	// get the file size
	fi, e := os.Stat(fileName)
	if e != nil {
		fmt.Println(e)
		os.Exit(3)
	}

	// log file
	logfile, err := os.OpenFile("mdencode.log", os.O_CREATE|os.O_WRONLY|os.O_APPEND, 0666)
	if err != nil {
		log.Fatalln("Failed to open log file:", err)
	}
	defer logfile.Close()
	fdata.log = log.New(logfile, "", log.Ldate|log.Ltime|log.Lshortfile)
	fdata.log.Println("mdencode file ", fileName, " blocksize ", fdata.blockSize, " modsize ", fdata.modSize)

	// set the file size
	size := fi.Size()
	fdata.fileSize = uint64(size)
	// set the number of blocks in the file and last block size
	blocks, remainder := calculateFileBlocks(uint64(size), uint64(blocksize))
	fdata.blockCount = uint64(blocks)
	fdata.blockRemainder = uint64(remainder)

	// setup the formatter
	// mdx := mdFormatText.Init()
	// mdx.SetFileData(fdata)
	fdata.md = mdFormatSQL.Init(format, fileName, fdata.filePath, fdata.fileSize, fdata.blockSize, fdata.modSize, fdata.fileHashListString, fdata.blockHashListString, outputfileName)
	fdata.md.SqlCreateTable()

	// test method to pass in the fdata struct type into the mdFormatText package
	// fdata.md.SetFileData(fdata)

	// fmt.Println("file hashlist names ", fdata.fileHashListNames)
	// fmt.Println("hashlist names ", fdata.blockHashListNames)

	// add a method to display the header block
	// It has an optional format to not display the block size
	// It would auto calculate it by adding up the block totals
	// and calculating what the last block size would be
	fdata.md.EncodeFileHeader(format, fileName, fdata.filePath, size, blocksize, fdata.fileHashListNames, fdata.blockHashListNames, bitsize)

	// mdXML := mdFormatXML.Init()
	// mdXML.EncodeFileHeader(format, fileName, fdata.filePath, size, blocksize, fdata.blockHashListNames, bitsize)

	// create a function to hash the entire file as one signature or multiple digital hash signatures
	// then collisions between the blocks can be resolved because the correct ones will fit together and be verified by the top level signature or signature group
	// can be a combination of 2 to 30 or more file signatures
	//
	// All combinations of blocks found by the block modulus scan signatures can be tested with the file signatures to find the correct blocks and resolve any collisions in the signatures
	//
	// The cost of the file block can be ammortized over the entire file
	//
	// could also have a mod2 encoding file / block mod 2 with exponent floor
	fdata.encodeFile(fileName)

	// encode the blocks
	fdata.mdencodeBlock(blockSize, modSize, format, fileName)

	// endcode the end of file xml
	fdata.md.EncodeEndFile(format)

	return 0

}

// add a top level file signature or group of signatures
// this will resolve any collisions in the block signatures
// it allows a modulus scan to find all the blocks matching a signature group and modulus
// then the top level file signature can validate against all the combinations of the blocks matched
// and find the correct one
//
// it can be a combination of 1 to 10+ signatures and a file modulus
// like ripe160, sha1, sha512, md5, md6, md4 etc
// this makes it very collision resistant
//
// this can encode the block as minimal simple, inform, xml
func (l *fileData) encodeFile(fileName string) {
	f, err := os.Open(fileName)
	if err != nil {
		// log.Fatal(err)
		fmt.Println(err)
	}
	defer f.Close()

	// this is not in sorted order
	// hash list
	// for k, h := range l.hashList {
	// for hashname, hashvalue := range fileHashListArrayNames {
	for _, hashvalue := range l.fileHashListNames {
		h := l.hashList[hashvalue]
		if _, err := io.Copy(h, f); err != nil {
			fmt.Println(err)
		}
		// md :=  hashvalue
		// h.Reset()
		// fmt.Println("z: ", md, " ", hex.EncodeToString(h.Sum(nil)))
		l.md.EncodeFileHash(l.mdFormat, hashvalue, hex.EncodeToString(h.Sum(nil)))
	}

}

// mdencode the file blocks
// this can generate a file level top signature
// this creates a file block signature with a group of file blocks with a modular floor
func (l *fileData) mdencodeBlock(blockSize string, modSize string, format int, fileName string) int {

	// process the blocksize argument
	blocksize, err := strconv.ParseInt(blockSize, 10, 64)

	// process the modulus bitsize argument
	// exponent
	bitsize, err := strconv.ParseInt(modSize, 10, 64)
	var i, e = big.NewInt(2), big.NewInt(bitsize)
	i.Exp(i, e, nil)

	// Open the file for reading
	file, err := os.Open(fileName)
	if err != nil {
		fmt.Println("Cannot find file: ", fileName)
		return 3
	}

	defer file.Close()

	// Start block hash
	// hashMD5 := md5.New()
	// hashSHA := sha1.New()

	// reader := bufio.NewReader(file)

	buf := make([]byte, blocksize)

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
	for {

		// bytesRead := blocksize
		// I think this is not correct and should use a block reader
		// bytesRead, err := reader.Read(buf) //ReadString and ReadLine() also applicable or alternative
		if _, err := file.Read(buf); err == io.EOF {
			break
		}

		if err != nil {
			fmt.Println("readblock error: ", err)
			return 3
		}

		// if int64(bytesRead) < blocksize {
		//        buf = buf[0:bytesRead]
		// }
		// if bytesRead == 0 {
		//        break
		// }
		if blocksRead == blocks {
			buf = buf[0:remainder]
			bytesRead = remainder
		}

		//in case it is a string file, you could check its content here...
		// fmt.Print(string(buf))
		/// io.WriteString(hashMD5, string(buf))
		/// io.WriteString(hashSHA, string(buf))
		// io.WriteString(hashMD5, string(buf))
		// io.WriteString(hashSHA, string(buf))
		// =========================
		// might need to be just buf
		// io.WriteString(hashMD5, buf)
		// io.WriteString(hashSHA, buf)
		// io.Copy(hashMD5, buf)
		// io.Copy(hashSHA, buf)
		// hashMD5.Write([]byte(buf))
		//// hashSHA.Write([]byte(buf))

		// create the biginteger representation of the bytes
		z := new(big.Int)
		z.SetBytes(buf)

		if byteblock == 1 {
			blockbytesBigInt := z.String()
			var bufstring = fmt.Sprint(buf)
			hlistarray = append(hlistarray, bufstring)
			// retrieve the block bigint
			// hlistarray = append(hlistarray, ":")
			hlistarray = append(hlistarray, blockbytesBigInt)
		}

		// critical test code for byte comparison
		// fmt.Println("buffer bytes ", z)
		// bigstr2 := fmt.Sprint(z)
		// this allows you to compare the bytes
		// should make this a hash or argument to display the byte buffer
		/////////////////////fmt.Println("buffer bytes ", z.Bytes(), " ", buf, " ", z)

		// this could be 2^exp then put the modulus somewhere within it so it has a larger floor
		// ie 2^151 and < 2^152
		//
		two := big.NewInt(2)
		modExp := logN(z, two)
		// alternate exponent floor the modulus to the power 1 less than the file block power greater than the file block
		// ie mod^151 and mod^152
		// generate the modulus exponent less than the file block size bigint
		// fmt.Print(":", logN(z, i))
		/// modExp := logN(z, i)

		// generate the file block modulus remainder
		mod := new(big.Int)
		mod = z.Mod(z, i)

		// generate the file block signatures from the hash list
		for _, hashvalue := range l.blockHashListNames {
			// if the hashname is not block treat it as a hash context
			if hashvalue != "block" {
				h := l.hashListBlocks[hashvalue]
				h.Write([]byte(buf))
				hlistarray = append(hlistarray, hex.EncodeToString(h.Sum(nil)))
				h.Reset()
				// append the block bytes
				/* } else {
				// hlistarray = append(hlistarray, string(buf))
				var bufstring = fmt.Sprint(buf)
				hlistarray = append(hlistarray, bufstring)
				// retrieve the block bigint
				hlistarray = append(hlistarray, ":")
				/// blockbytesBigInt := z.String()  // should be before the modulus operation
				hlistarray = append(hlistarray, blockbytesBigInt)
				*/
			}
		}
		/*
		                   // create the biginteger representation of the bytes
		                   z := new(big.Int)
		                   z.SetBytes(buf)

		   		// critical test code for byte comparison
		   		// fmt.Println("buffer bytes ", z)
		   		// bigstr2 := fmt.Sprint(z)
		   		// this allows you to compare the bytes
		   		// should make this a hash or argument to display the byte buffer
		   		/////////////////////fmt.Println("buffer bytes ", z.Bytes(), " ", buf, " ", z)

		   		// this could be 2^exp then put the modulus somewhere within it so it has a larger floor
		   		// ie 2^151 and < 2^152
		   		//
		   		two := big.NewInt(2)
		   		modExp := logN(z, two)
		   		// alternate exponent floor the modulus to the power 1 less than the file block power greater than the file block
		   		// ie mod^151 and mod^152
		   		// generate the modulus exponent less than the file block size bigint
		                   // fmt.Print(":", logN(z, i))
		   		/// modExp := logN(z, i)

		   		// generate the file block modulus remainder
		                   mod := new(big.Int)
		                   mod = z.Mod(z, i);
		*/

		// fmt.Print(":", mod);

		// display the block big integer number
		// fmt.Print(":", z);

		// fmt.Print("\n");

		// generate the file signature block string
		// l.encodeBlock(format, bytesRead, hex.EncodeToString(hashMD5.Sum(nil)), hex.EncodeToString(hashSHA.Sum(nil)), modExp, mod.String());
		// message formatter version
		l.md.EncodeBlock(format, bytesRead, hlistarray, modExp, mod.String())
		blocksRead++
		hlistarray = hlistarray[0:0]
	}

	return 0

}

// create the hash list map
func (l *fileData) createHashListMap(fileBlockflag int) {

	var key = []byte("H32d87sdfjh112221")

	// file sig variables
	// l.fileHashListArray[i]
	// l.hashList
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
		//		fmt.Println("create length ", length, " ", last, " ", fileBlockflag)
	}

	// file block sig variables
	// l.blockHashListArray
	// l.hashListBlocks
	// l.blockHashListNames
	// var hlistarray []string
	// hb := l.hashListBlocks
	hb := make(map[string]hash.Hash)

	var list []string
	for i := 0; i < last; i++ {
		// var vbool, _ = strconv.Atoi(l.blockHashListArray[i])
		var vbool, _ = strconv.Atoi(hlistarray[i])
		var v = (i * 10) + vbool
		// fmt.Println("z: ", v)
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
			// hb["sip128"] = siphash.New128([]byte("H32d87sdfjh"))
			var seed uint64 = 1120322
			hb["murmur3"] = murmur3.New128(seed)
			// hasher := murmur3.New128()
			// hasher.Write
		case 161:
			hb["whirlpool"] = whirlpool.New()
		case 171:
			hb["hmac256"] = hmac.New(sha256.New, key)
		case 181:
			hb["hmac512"] = hmac.New(sha512.New, key)
		case 191:
			hb["kekkak"] = keccak.New256()
		case 201:
			// hb["skein160"] = skein.New160([]byte("H32d87sdfjh"))
			hb["skein160"] = skein.New(20, nil)
		case 211:
			//hb["skein256"] = skein.New256([]byte("H32d87sdfjh"))
			hb["skein256"] = skein.New256(key)
		case 221:
			// hb["skein384"] = skein.New384([]byte("H32d87sdfjh"))
			hb["skein384"] = skein.New(48, nil)
		case 231:
			hb["skein512"] = skein.New512(key)
		case 241:
			hb["skein1024"] = skein.New(128, nil)
		case 251:
			hb["tiger"] = tiger.New()
		case 261:
			// has to have a key 8 or 16 bytes
			hb["siphash"] = siphash.New128(key)
		case 271:
			// this one needs two values since it has a multiple return
			hb["blake2s_128"], _ = blake2s.New128(key)
		case 281:
			hb["blake2s_256"], _ = blake2s.New256(key)
		case 291:
			hb["blake2"] = blake2.New(nil)
			// case 301:
			// hb["block"] = fnv.New64a()
			//hb["blake2s"], _ =  blake2s.New128(nil)
			// case 231:
			//var customString = []byte("128Ulsurl90Sx789sdreReee12")
			// hb["kangaroo12"] =  K12.NewK12(customString)

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

// this sets the md format
// it can be simple
// inform
// xml
// binary output
// sql lite
// it returns the correct md format object
/*func (l *fileData) setmdFormat (format int ) {
fdata.md = mdFormatText.Init(format, fileName, fdata.filePath, fdata.fileSize, fdata.blockSize, fdata.modSize, outputfileName)


}
*/

/*
int logN(int num,int base)
{
      if(num<base)
           return 0;
     return 1 + logN(num/base,base);
}
*/

// calculate the bigint log
// the bitsize exponent less than the blocksize big integer
func logNold(x *big.Int, base *big.Int) int {

	gt := x.Cmp(base)

	if gt < 0 {
		return 0
	}
	z := new(big.Int).Quo(x, base)
	return 1 + logN(z, base)

}

// calculate the bigint log
// the bitsize exponent less than the blocksize big integer
func logN(x *big.Int, base *big.Int) int {

	gt := x.Cmp(base)

	if gt < 0 {
		return 0
	}
	z := new(big.Int).Quo(x, base)
	return 1 + logN(z, base)

}

// calculate the number of file blocks
func calculateFileBlocks(fileSize uint64, blockSize uint64) (uint64, uint64) {

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
