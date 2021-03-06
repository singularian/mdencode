package mdSignatureList

// mdencode project
//
// mdSignatureList.go
//
// mdencode mdunzip signature binary list converter
// This package converts a bitstring list to a name string list
// It also calculates a signature block size
//
// https://github.com/singularian/mdencode
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"fmt"
	"regexp"
	"strconv"
	"strings"
	"os"
	"sort"
)

// BlockList object stores the file block signature list
type BlockList struct {
	fileHashList string
	blockHashList string
	// hashNames
	hashNames      []string
	HashNamesSize  int
	// hash Block size map
	m map[string]int
	// block names
	hashList       []string
	hashListFile   []string
	hashListBlocks []string

	// blocksize
	bhashList string
	blockList string
	blockSize uint64
	blockSizeArr []int
}

// HashList Struct
type HashList struct {
	HashName    string
	Description string
	BlockSize int
	Key bool
}

// hash list struct variable
// this should be in sorted order
var hlist = []HashList{
		{ HashName: "add32",       Description: "Adler 32",		BlockSize: 4,	Key: false },
		{ HashName: "aes8",        Description: "AES Hash 8",		BlockSize: 8,	Key: true },
		{ HashName: "ax",          Description: "XXHash 128",		BlockSize: 16,	Key: true },
		{ HashName: "blake2",      Description: "Blake2",		BlockSize: 64,	Key: false },
		{ HashName: "blake2b",     Description: "Blake2b",		BlockSize: 32,	Key: false },
		{ HashName: "blake2s_128", Description: "Blake2s 128",		BlockSize: 16,	Key: true },
		{ HashName: "blake2s_256", Description: "Blake2s 256",		BlockSize: 32,	Key: true },
		{ HashName: "bmw",         Description: "BMW",			BlockSize: 64,	Key: false },

		{ HashName: "crc32",       Description: "CRC-32 IEEE",		BlockSize: 4,	Key: false },
		{ HashName: "crc32c",      Description: "CRC-32 Castagnoli",	BlockSize: 4,	Key: false },
		{ HashName: "crc32k",      Description: "CRC-32 Koopman",	BlockSize: 4,	Key: false },
		{ HashName: "crc64ecma",   Description: "CRC-64 ECMA",		BlockSize: 8,	Key: false },
		{ HashName: "crc64iso",    Description: "CRC-64 ISO",		BlockSize: 8,	Key: false },
		{ HashName: "ct64",        Description: "Cityhash 64",		BlockSize: 8,	Key: true },
		{ HashName: "cube",        Description: "Cubehash",		BlockSize: 64,	Key: false },
		{ HashName: "echo",        Description: "Echo",			BlockSize: 64,	Key: false },

		{ HashName: "fh32",        Description: "Farm Hash 32",		BlockSize: 4,	Key: true },
		{ HashName: "fh64",        Description: "Farm Hash 64",		BlockSize: 8,	Key: true },
		{ HashName: "fnv32",       Description: "FNV 32",		BlockSize: 4,	Key: false },
		{ HashName: "fnv32a",      Description: "FNV 32a",		BlockSize: 4,	Key: false },
		{ HashName: "fnv64",       Description: "FNV 64",		BlockSize: 8,	Key: false },
		{ HashName: "fnv64a",      Description: "FNV 64a",		BlockSize: 8,	Key: false },
		{ HashName: "fnv128",      Description: "FNV 128",		BlockSize: 16,	Key: false },
		{ HashName: "fnv128a",     Description: "FNV 128a",		BlockSize: 16,	Key: false },
		{ HashName: "gost256",     Description: "Gost 256",		BlockSize: 32,	Key: false },
		{ HashName: "gost512",     Description: "Gost 512",		BlockSize: 64,	Key: false },
		{ HashName: "groest",      Description: "Groest",		BlockSize: 64,	Key: false },

		{ HashName: "hmac256",     Description: "HMAC 256",		BlockSize: 32,	Key: true },
		{ HashName: "hmac512",     Description: "HMAC 512",		BlockSize: 64,	Key: true },
		{ HashName: "hw32",        Description: "Highway Hash 32",	BlockSize: 4,	Key: true },
		{ HashName: "hw64",        Description: "Highway Hash 64",	BlockSize: 8,	Key: true },
		{ HashName: "hw128",       Description: "Highway Hash 128",	BlockSize: 16,	Key: true },
		{ HashName: "hw256",       Description: "Highway Hash 256",	BlockSize: 32,	Key: true },
		{ HashName: "kekkak",      Description: "Kekkak",		BlockSize: 32,	Key: false },
		{ HashName: "luffa",       Description: "Luffa",		BlockSize: 64,	Key: false },

		{ HashName: "jn64",        Description: "Jenkins 64",		BlockSize: 8,	Key: true },
		{ HashName: "mar32",       Description: "Marvin 32",		BlockSize: 4,	Key: true },
		{ HashName: "me32",        Description: "Meow 32",		BlockSize: 4,	Key: true },
		{ HashName: "me64",        Description: "Meow 64",		BlockSize: 8,	Key: true },
		{ HashName: "me128",       Description: "Meow 128",		BlockSize: 16,	Key: true },
		{ HashName: "met64",       Description: "Metro Hash 64",	BlockSize: 8,	Key: true },

		{ HashName: "mm32",        Description: "Murmur3 32",		BlockSize: 4,	Key: true },
		{ HashName: "murmur3",     Description: "Murmur3",		BlockSize: 16,	Key: true },
		{ HashName: "md2",         Description: "MD2",			BlockSize: 16,	Key: false },
		{ HashName: "md4",         Description: "MD4",			BlockSize: 16,	Key: false },
		{ HashName: "md5",         Description: "MD5",			BlockSize: 16,	Key: false },
		{ HashName: "poly1305",    Description: "Poly1305",		BlockSize: 16,	Key: true },

		{ HashName: "ripe128",     Description: "Ripe 128",		BlockSize: 16,	Key: false },
		{ HashName: "ripe160",     Description: "Ripe 160",		BlockSize: 20,	Key: false },
		{ HashName: "ripe256",     Description: "Ripe 256",		BlockSize: 32,	Key: false },
		{ HashName: "ripe320",     Description: "Ripe 320",		BlockSize: 40,	Key: false },

		{ HashName: "sea",         Description: "Seahash",		BlockSize: 8,	Key: false },
		{ HashName: "sha1_64",     Description: "SHA1 64",		BlockSize: 8,	Key: false },
		{ HashName: "sha1_128",    Description: "SHA1 128",		BlockSize: 16,	Key: false },
		{ HashName: "sha1_1284",   Description: "SHA1 128 4-20 Bytes",	BlockSize: 16,	Key: false },
		{ HashName: "sha1",        Description: "SHA1",			BlockSize: 20,	Key: false },
		{ HashName: "sha224",      Description: "SHA224",		BlockSize: 28,	Key: false },
		{ HashName: "sha256",      Description: "SHA256",		BlockSize: 32,	Key: false },
		{ HashName: "sha512_224",  Description: "SHA512 224",		BlockSize: 28,	Key: false },
		{ HashName: "sha512_256",  Description: "SHA512 256",		BlockSize: 32,	Key: false },
		{ HashName: "sha512_384",  Description: "SHA512 384",		BlockSize: 48,	Key: false },
		{ HashName: "sha512",      Description: "SHA512",               BlockSize: 64,  Key: false },
		{ HashName: "sha3_224",    Description: "SHA3 224",		BlockSize: 28,	Key: false },
		{ HashName: "sha3_256",    Description: "SHA3 256",		BlockSize: 32,	Key: false },
		{ HashName: "sha3_384",    Description: "SHA3 384",		BlockSize: 48,	Key: false },
		{ HashName: "sha3_512",    Description: "SHA3 512",		BlockSize: 64,	Key: false },
		{ HashName: "shavite",     Description: "X11 SHAvite",		BlockSize: 64,	Key: false },

		{ HashName: "sip64",       Description: "Siphash 64",		BlockSize: 8,	Key: true },
		{ HashName: "sip128",      Description: "Siphash 128",		BlockSize: 16,	Key: true },
		{ HashName: "skein_160",   Description: "Skein 160",		BlockSize: 20,	Key: false },
		{ HashName: "skein_256",   Description: "Skein 256",		BlockSize: 32,	Key: false },
		{ HashName: "skein_384",   Description: "Skein 384",		BlockSize: 48,	Key: false },
		{ HashName: "skein_512",   Description: "Skein 512",		BlockSize: 64,	Key: false },
		{ HashName: "skein_1024",  Description: "Skein 1024",		BlockSize: 128,	Key: false },
		{ HashName: "spk32",       Description: "Spooky 32",		BlockSize: 4,	Key: true },
		{ HashName: "spk64",       Description: "Spooky 64",		BlockSize: 8,	Key: true },
		{ HashName: "tiger",       Description: "Tiger",		BlockSize: 24,	Key: false },
		{ HashName: "t1ha",        Description: "T1ha",			BlockSize: 8,	Key: true },
		{ HashName: "whirlpool",   Description: "Whirlpool",		BlockSize: 64,	Key: false },
		{ HashName: "xxh32",       Description: "XXHash 32",		BlockSize: 4,	Key: true },
		{ HashName: "xxhash",      Description: "XXHash 64",		BlockSize: 8,	Key: true },
		{ HashName: "wy",          Description: "WY Hash 64",		BlockSize: 8,	Key: true },
                }

// Init returns a new BlockList object  
func Init() (bl *BlockList) {
        blx := new(BlockList)
	blx.hashNames = blx.GetHashList()
	blx.HashNamesSize = len(blx.hashNames)

	blx.SetHashListBlockSize()
        return blx
}

// GetHashList
// returns the signature hashList Struct HashName ID list
func (bl *BlockList) GetHashList() ([]string) {

	var hashNames []string

	var length = len(hlist)
        for i := 0; i < length; i++ {
                var name  = hlist[i].HashName
		hashNames = append(hashNames, name)
	}

	return hashNames
}

// GetHashListObject
// returns the signature hash list Struct
func (bl *BlockList) GetHashListObject() ([]HashList) {

	return hlist
}

// CreateHashBlockList checks the hashlist parameter
// It calls CreateHashBlockListBinary or CreateHashBlockListCSV based on the hashlist format
// If it is Binary (010111001111)   call CreateHashBlockListBinary 
// If it is CSV    (1,2,4,11-15,15) call CreateHashBlockListCSV
func (bl *BlockList) CreateHashBlockList(hashlist string) ([]string) {

	// binarylist
	binre := regexp.MustCompile("^([01]+)$")
	matched := binre.MatchString(hashlist)

	// csv list
	var csvhashlist = hashlist
        csvhashlist += ","
        csvhashlist = strings.Replace(csvhashlist, ",,", ",", 1)

        // check if the csv hash list is valid
        // number 1, 2, ...
        // number range 1-13 or 4-6 ...
        var csv = regexp.MustCompile(`^(\d+(\-\d+)?[,])+$`)
        csvmatched := csv.MatchString(csvhashlist)

	if matched {
		return bl.CreateHashBlockListBinary(hashlist)
	} else if csvmatched {
		return bl.CreateHashBlockListCSV(hashlist)
	} else if !matched && !csvmatched {
		fmt.Println("Invalid Binary or CSV hashlist ", hashlist)
		os.Exit(1)
	}


	return bl.hashList

}

// CreateHashBlockListBinary
// CreateHashBlockListBinary takes a hashlist boolean and converts it to a name list
// ie converts the boolean string to a hash name string
// example binary hash list  110011 is converted to hashlist [add32 aes8 blake2b blake2s_128]
func (bl *BlockList) CreateHashBlockListBinary(hashlistBoolean string) ([]string) {

	bl.ClearHashLists()

	re := regexp.MustCompile("[01]")

	arr := re.FindAllString(hashlistBoolean, -1)

	// set the hashNames array list
	// hashNames := bl.hashNames
	// var length = len(hashNames)
	var length = len(hlist)

	// fmt.Println("hash Name", hashNames,  len(hashNames))

        for i := 0; i < len(arr); i++ {
                // fmt.Println("hashlist ", hashName[i])
		var vbool, _ = strconv.Atoi(arr[i])

		if i >= length {
			break
		}

                switch vbool {
                        // case 1: bl.AddHashList(hashNames[i], i) 
			case 0: // do nothing
                        case 1: bl.AddHashList(hlist[i].HashName, i)
			default:
				fmt.Println("Unrecognized hashlist boolean")
				os.Exit(1)
                }

        }

	bl.blockHashList = strings.Join(bl.hashList, ":")

	// fmt.Println("hashlist ", bl.blockHashList)

	return bl.hashList
}

// CreateHashBlockListCSV
// This converts a csv hash number list to a hashlist with the corresponding hash ids 
// csv string 1,3,4,5,7-100
//
// The current format is
// SIGNUM,SIGNUM2,SIGNUMX,SIGNUMY-SIGNUMZ, ...
//
// TODO: It could add an optional Type 
// TYPE:SIGNUM,TYPE:SIGNUMSTART-SIDEND, ...
//
// 0 file 1 block 2 both
// block or file list: 0:1,1:5,2:7,2:8-12
// block list 1,2,3,7-12,15-20
// file list 5,6,7,8,10-20
//
// Current Examples:
// hash list  1,2,3,4,5-6 [add32 aes8 ax blake2 blake2b blake2s_128] length 6
// 1,11,23,44 [add32 crc32k fnv128 md2] length 4
func (bl *BlockList) CreateHashBlockListCSV(hashlistCSV string) ([]string) {

	bl.ClearHashLists()

	hashlistCSV += ","
	hashlistCSV = strings.Replace(hashlistCSV, ",,", ",", 1)

	// check if the csv hash list is valid
	// number 1, 2, ...
	// number range 1-13 or 4-6 ...
	var csv = regexp.MustCompile(`^(\d+(\-\d+)?[,])+$`)
	matched := csv.MatchString(hashlistCSV)

	if !matched {
		fmt.Println("Invalid CSV hashlist ", hashlistCSV)
		os.Exit(1)
	}

	arr := strings.Split(hashlistCSV, ",")

	var s []int
	// fmt.Println("processing hashlistCSV ", hashlistCSV)

	// process the csv hash list and add the hash numbers to the slice
	// for index, hash := range split {
	for _, hash := range arr {

		var hashNum   = regexp.MustCompile(`^[[:digit:]]+$`)
		var hashRange = regexp.MustCompile(`[[:digit:]]+\-[[:digit:]]+`)
		if (hashNum.MatchString(hash)) {
		//	fmt.Println("hash number ", hash)
			var index, _ = strconv.Atoi(hash)
			if index > 0 {
				index = index - 1
			}
			s = append(s, index)
		}
		if (hashRange.MatchString(hash)) {
		//	fmt.Println("hash range number ", hash)
			hr := strings.Split(hash, "-")
			var start, _ = strconv.Atoi(hr[0])
			var end,   _ = strconv.Atoi(hr[1])
		//	fmt.Println("hash range number ", hash, start, end)
			if start > 0 {
			start = start - 1
			end   = end   - 1
			}
			if start <= end {
				for index := start; index <= end; index++ {
					if index < bl.HashNamesSize {
						s = append(s, index )
					}
				}

			}
		}

		// fmt.Println("hash ", hash)

	}

	// sort the hash list slice
	sort.Ints(s)

	// unique the hash list slice with the in place deduper
	s = bl.Uniques(s)

	// add the hash list names to the hash list
	for _, index := range s {
		if index < bl.HashNamesSize {
			bl.AddHashList(hlist[index].HashName, index)
		}
	}


	return bl.hashList

}

func (bl *BlockList) Uniques(s []int) []int {
	if len(s) == 0 {
		return s
	}
	seen := make([]int, 0, len(s))
slice:
	for i, n := range s {
		if i == 0 {
			s = s[:0]
		}
		for _, t := range seen {
			if n == t {
				continue slice
			}
		}
		seen = append(seen, n)
		s = append(s, n)
	}
	return s
}

// AddHashList
// add the hash name to the blockList hashList Array 
func (bl *BlockList) AddHashList (hashName string, number int) {

	// fmt.Println("hash ", number, hashName)

	bl.hashList = append(bl.hashList, hashName)
}

// clear the block lists
func (bl *BlockList) ClearHashLists () {

	bl.hashList       = nil
	bl.hashListFile   = nil
	bl.hashListBlocks = nil

}

/************************************** 
Block Calculator Size Methods
***************************************/


// SetHashListBlockSize
// set the block size map
// This function returns a hash map of signatures strings with their hash block size
// This allows a hash signature to lookup it's block size by name 
func (bl *BlockList) SetHashListBlockSize() {

	blockSizeMap := make(map[string]int)
	var length = len(hlist)
	for i := 0; i < length; i++ {
		var name  = hlist[i].HashName
		var bsize = hlist[i].BlockSize
		blockSizeMap[name] = bsize

	}
	bl.m = blockSizeMap
}

// CalcHashBlockSize
// calculate the block size for a hashlist string
// This allows mdprint or mdunzip to decode each signature block and calculate their size
//
// It returns the blocksize sum and the array of signature block sizes
//
// Example block string: ax:md4:md5:sha1:sea:hw64
// Example block size  84 block array size [16 16 16 20 8 8]
func (bl *BlockList) CalcHashBlockSize (hashlist string) (uint64, []int) {
        hashName := strings.Split(hashlist, ":")

	// clear this since the file block is also using it to calculate the file block size
	// it needs this for the whole file signature size for mdunzip and the file block signature size
	bl.ClearBlockSig()

	// set the block hash list
        bl.bhashList = hashlist

        for i := 0; i < len(hashName); i++ {
                // fmt.Println("hashlist ", hashName[i])

		var hname = hashName[i]
		if bsize, ok := bl.m[hname]; ok {
			bl.AddBlockSig(bsize)
		}
	}


	return bl.blockSize, bl.blockSizeArr
}


// AddBlockSig
// add the hash block the blockSize Array and increment the blockSize count
func (bl *BlockList) AddBlockSig (hashSize int) {

        bl.blockSizeArr = append(bl.blockSizeArr, hashSize)
        bl.blockSize += uint64(hashSize)
}

// clear the blocksize counts
func (bl *BlockList) ClearBlockSig () {

        bl.blockSizeArr = nil
        bl.blockSize = 0

}



