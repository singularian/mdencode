package main

// mdencode
// copyright (C) Scott Ross 2017
// https://github.com/singularian/mdencode/blob/master/LICENSE


import (
        "regexp"
	"hash"
	"hash/fnv"
	"golang.org/x/crypto/md4"
	"crypto/md5"
	"crypto/sha1"
	"crypto/sha256"
	"crypto/sha512"
	"crypto/hmac"
	"github.com/codahale/blake2"
	// "golang.org/x/crypto/blake2b"
	"golang.org/x/crypto/blake2s"
	"golang.org/x/crypto/ripemd160"
	"golang.org/x/crypto/sha3"
	"github.com/jzelinskie/whirlpool"
	_ "github.com/mimoo/GoKangarooTwelve/K12"
	"github.com/steakknife/keccak"
	"github.com/dchest/siphash"
	_"github.com/aead/siphash"
	_ "github.com/spaolacci/murmur3"
	"github.com/asadmshah/murmur3"
	_"github.com/twmb/murmur3"
	_ "github.com/enceve/crypto/skein"
	"github.com/aead/skein"
	"github.com/cxmcc/tiger"
	"encoding/hex"
	"fmt"
	"strconv"
	"io"
	"math/big"
	"log"
	"os"
	"path/filepath"
	"sort"
	// "github.com/singularian/mdencode_private/code/encode/mdFormats/mdFormatText"
	"github.com/singularian/mdencode_private/code/encode/mdFormats/mdFormatBinary"
	// "github.com/singularian/mdencode_private/code/encode/mdFormats/mdFormatSQL"
)

// mdformat interface struct
type mdformat interface {
	PrintFormatType()
	OpenFile(appendfile bool)
	InitFile()
	EncodeFileHeader(encodingFormat int, fileName string, filePath string, fileSize int64, blockSize int64, filehashList []string, blockhashList []string, modulusSize int64) 
	EncodeFileHash(encodingFormat int, hashName string, hashBytes string)
	EncodeBlock(encodingFormat int, blockSize uint64, hashList []string, modExp int, mod string)
	EncodeEndFile(encodingFormat int)
}

// mdencode struct
type fileData struct {
	commandArgs int
	fileName string
	filePath string
	outputFileName string
	fileSize uint64
	blockSize uint64
	blockCount uint64
	blockRemainder uint64
	modSize uint64
	modExponent uint64
	mdFormat int
	mdVersion string
	// argument hash list bits
	fileHashListString string
	blockHashListString string
	// argument hash list arrays 
	fileHashListArray    []string
        blockHashListArray    []string
	// argument signature hash list 
	fileHashListNames []string
	blockHashListNames []string
	// dictionary
	dictionary map[string]string
	// hash list for files
	hashList  map[string]hash.Hash
	// hash list for blocks
	hashListBlocks  map[string]hash.Hash
	// formatter
	// md *mdFormatText.MdFormat
	// mdformat interface for modular type assignment
	mdfmt mdformat
	// log writer
	log *log.Logger
}

var byteblock = 0

func main() {

	var hashlist string = "01100000000"

	var argsNumber int = len(os.Args)

	// possibly have a fourth argument for the format type
	// minimal length
	// default length
	// xml
	// [] inform delineated
	// Ensure the file argument is passed
	// 
	// a simple default is chosen if only the fileName is specified
	//
	// mdencode file
	var mdname = os.Args[0]

	// fmt.Println(mdname)

	if mdname == "mdencodeBIN" || mdname == "./mdencodeBIN" {
		argsSimple(hashlist, argsNumber)
	} else if mdname == "mdencodebyteblock" || mdname == "./mdencodebyteblock" {
		byteblock = 1
		argsSimple(hashlist, argsNumber)

	} else if mdname == "mdencodeXML" || mdname == "./mdencodeXML" {
		argsXML(hashlist, argsNumber)
	}

	os.Exit(0)

}

// this is a formatted mdencode argument processor
// it adds a numeric formatter
// there are currently the following formats for the md
// text output
// inform output
// xml output
// binary output -- todo
// sql db output
func argsSimple(hashlist string, argsNumber int) int {

	var outfilename = ""
	var defaultFormat = 4 

	// mdencode file
	if argsNumber == 2 {
		filename   := os.Args[1]
		blocksize  :=  "20"
		modsize    :=  "16"
		mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist, filename, outfilename)
	// mdencode 60 24 file
	} else if argsNumber == 4 {
		blocksize  := os.Args[1]
                modsize    := os.Args[2]
		filename   := os.Args[3]
		mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist, filename, outfilename)
	// mdencode 60 24 4 file
	} else if argsNumber == 5 {
		blocksize  := os.Args[1]
		modsize    := os.Args[2]
		formatType, _ := strconv.Atoi(os.Args[3])
		filename   := os.Args[4]
		mdencode(blocksize, modsize, formatType, hashlist, hashlist, filename, outfilename)
	// mdencode 60 24 4 0110000001 0110000001 file
	} else if argsNumber == 7 {
		blocksize  := os.Args[1]
                modsize    := os.Args[2]
		formatType, _ := strconv.Atoi(os.Args[3])
		hashsize1  := os.Args[4]
		hashsize2  := os.Args[5]
		filename   := os.Args[6]
		mdencode(blocksize, modsize, formatType, hashsize1, hashsize2, filename, outfilename)
	// mdencode 60 24 4 0110000001 0110000001 mdfile outputfile
	} else if argsNumber == 8 {
                blocksize   := os.Args[1]
                modsize     := os.Args[2]
                formatType, _ := strconv.Atoi(os.Args[3])
                hashsize1   := os.Args[4]
                hashsize2   := os.Args[5]
                filename    := os.Args[6]
                outfilename = os.Args[7]
                mdencode(blocksize, modsize, formatType, hashsize1, hashsize2, filename, outfilename)
	} else {
		fmt.Println("\nUsage: mdencode [filename]")
		fmt.Println("Usage: mdencode [blocksize] [modulus bitsize] [filename]")
		fmt.Println("Usage: mdencode [blocksize] [modulus bitsize] [format] [filename]")
		fmt.Println("Usage: mdencode [blocksize] [modulus bitsize] [format] [file signatures bitstring] [block signatures bitstring] [filename]\n")
		fmt.Println("Usage: mdencode [blocksize] [modulus bitsize] [format] [01]+ [01]+ [filename]")
		fmt.Println("Usage: mdencode [blocksize] [modulus bitsize] [format] [01]+ [01]+ [filename] [outputfile]")
		fmt.Println("\n")
		fmt.Println("Examples:")
		fmt.Println("mdencode file")
		fmt.Println("mdencode 60 24 file")
		fmt.Println("mdencode 60 24 1 file")
		fmt.Println("mdencode 60 24 4 file")
		fmt.Println("mdencode 60 24 4 1 0 file")
		fmt.Println("mdencode 60 24 4 0 1 file")
		fmt.Println("mdencode 60 24 4 0110000001 0110000001 file")
		fmt.Println("mdencode 60 24 4 0110000001 0110000001 file outputfile\n")
		fmt.Println("Formats 1.0:")
                fmt.Println("0: Minimal")
                fmt.Println("1: Expanded")
                fmt.Println("2: Simple/Separated")
                fmt.Println("3: Complex")
                fmt.Println("4: Verbose")
                fmt.Println("5: Verbose")
                fmt.Println("9: Inform")
		fmt.Println("100: XML")
		fmt.Println("\n")
		fmt.Println("Version: 1.0 復甦 復活\n")
	}
	return 0

}

// xml handler
// might have different xml formats
func argsXML(hashlist string, argsNumber int) int {

        var outfilename = ""
        var defaultFormat = 100

	// mdencode file
        if argsNumber == 2 {
                filename   := os.Args[1]
                blocksize  :=  "20"
                modsize    :=  "16"
                mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist, filename, outfilename)
        // mdencode 60 24 file
        } else if argsNumber == 4 {
                blocksize  := os.Args[1]
                modsize    := os.Args[2]
                filename   := os.Args[3]
                mdencode(blocksize, modsize, defaultFormat, hashlist, hashlist, filename, outfilename)
	// mdencode 60 24 0110000001 0110000001 file
        } else if argsNumber == 6 {
                blocksize  := os.Args[1]
                modsize    := os.Args[2]
                hashsize1  := os.Args[3]
                hashsize2  := os.Args[4]
                filename   := os.Args[5]
                mdencode(blocksize, modsize, defaultFormat, hashsize1, hashsize2, filename, outfilename)
        // mdencode 60 24 0110000001 0110000001 mdfile outputfile
	} else if argsNumber == 7 {
                blocksize   := os.Args[1]
                modsize     := os.Args[2]
                hashsize1   := os.Args[3]
                hashsize2   := os.Args[4]
                filename    := os.Args[5]
                outfilename = os.Args[6]
                mdencode(blocksize, modsize, defaultFormat, hashsize1, hashsize2, filename, outfilename)
        } else {
                fmt.Println("\nUsage: mdencodeXML [filename]")
                fmt.Println("Usage: mdencodeXML [blocksize] [modulus bitsize] [filename]")
                fmt.Println("Usage: mdencodeXML [blocksize] [modulus bitsize] [filename]")
                fmt.Println("Usage: mdencodeXML [blocksize] [modulus bitsize] [file signatures bitstring] [block signatures bitstring] [filename]\n")
                fmt.Println("Usage: mdencodeXML [blocksize] [modulus bitsize] [01]+ [01]+ [filename]")
                fmt.Println("Usage: mdencodeXML [blocksize] [modulus bitsize] [01]+ [01]+ [filename] [outputfile]")
                fmt.Println("\n")
                fmt.Println("Examples:")
                fmt.Println("mdencodeXML file")
                fmt.Println("mdencodeXML 60 24 file")
                fmt.Println("mdencodeXML 60 24 1 0 file")
                fmt.Println("mdencodeXML 60 24 0 1 file")
                fmt.Println("mdencodeXML 60 24 0110000001 0110000001 file\n")
                fmt.Println("Version: 1.0 復甦 復活\n")
        }

	return 0

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
	// set the filename
        fdata.fileName  = fileName
	// find the fileData file path
	path, _ := filepath.Abs(fileName)
	// dir, file := filepath.Split(path)
	dir, _ := filepath.Split(path)
	fdata.filePath  = dir
	// set the output filename
	fdata.outputFileName = outputfileName
	// initialize the blocksize and modsize
        fdata.blockSize = uint64(blocksize)
        fdata.modSize   = uint64(bitsize)
	// initalize the format
        fdata.mdFormat  = format

	// process the hash list arguments
	fdata.fileHashListString  = fileHashList
	fdata.blockHashListString = blockHashList 
	// hash list regex
        re := regexp.MustCompile("[01]")

        fdata.fileHashListArray  = re.FindAllString(fileHashList, -1)
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
	fi, e := os.Stat(fileName);
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
	fdata.log =  log.New(logfile, "", log.Ldate|log.Ltime|log.Lshortfile)
	fdata.log.Println("mdencode file ", fileName, " blocksize ", fdata.blockSize, " modsize ", fdata.modSize )

	// set the file size
	size := fi.Size()
	fdata.fileSize = uint64(size)
	// set the number of blocks in the file and last block size
	blocks, remainder := calculateFileBlocks(uint64(size), uint64(blocksize))
	fdata.blockCount     = uint64(blocks)
	fdata.blockRemainder = uint64(remainder)

	// setup the formatter
	// fdata.md = mdFormatText.Init(format, fileName, fdata.filePath, fdata.fileSize, fdata.blockSize, fdata.modSize, outputfileName)
	// fdata.mdfmt = mdFormatText.Init(format, fileName, fdata.filePath, fdata.fileSize, fdata.blockSize, fdata.modSize, outputfileName)
	fdata.setmdFormat(format)

	// fdata.mdfmt.Println()

	// test method to pass in the fdata struct type into the mdFormatText package
	// fdata.md.SetFileData(fdata)

	// fmt.Println("file hashlist names ", fdata.fileHashListNames)
	// fmt.Println("hashlist names ", fdata.blockHashListNames)

	// add a method to display the header block
        // It has an optional format to not display the block size
        // It would auto calculate it by adding up the block totals
        // and calculating what the last block size would be
	fdata.mdfmt.EncodeFileHeader(format, fileName, fdata.filePath, size, blocksize, fdata.fileHashListNames, fdata.blockHashListNames, bitsize)

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
	fdata.mdfmt.EncodeEndFile(format)

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
		l.mdfmt.EncodeFileHash(l.mdFormat, hashvalue, hex.EncodeToString(h.Sum(nil)))
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
	var bytesRead  uint64 = uint64(blocksize)

	var hashsize = len(l.blockHashListNames)
	if hashsize < 1 {
		return 0
	}

	var hlistarray []string
        for{

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
                mod = z.Mod(z, i);

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
		l.mdfmt.EncodeBlock(format, bytesRead, hlistarray, modExp, mod.String());
		blocksRead++
		hlistarray = hlistarray[0:0] 
        }


	return 0

}


// create the hash list map
func (l *fileData) createHashListMap (fileBlockflag int ) {

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
	var last  = 30
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
                var v  = (i * 10) + vbool
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
			hb["sha224"]  = sha256.New224()
			case 41:
			hb["sha256"]  = sha256.New()
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
			hb["siphash"] =  siphash.New128(key)
			case 271:
			// this one needs two values since it has a multiple return
			hb["blake2s_128"], _ =  blake2s.New128(key)
			case 281:
			hb["blake2s_256"], _ =  blake2s.New256(key)
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

// it sets the correct md format object 
func (l *fileData) setmdFormat (format int ) {

	// if format <= 100 {
	// l.mdfmt = mdFormatText.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.outputFileName)
	//} else {
	//	l.mdfmt = mdFormatSQL.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.outputFileName)
	//}
	l.mdfmt = mdFormatBinary.Init(format, l.fileName, l.filePath, l.fileSize, l.blockSize, l.modSize, l.fileHashListString, l.blockHashListString, l.outputFileName)

	l.mdfmt.InitFile()
	l.mdfmt.OpenFile(false)

}

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
