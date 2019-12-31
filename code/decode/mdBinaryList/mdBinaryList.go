package mdBinaryList

// mdencode project
//
// mdBinaryList.go
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
//	"fmt"
	"regexp"
	"strconv"
	"strings"
)

// BlockList object stores the file block signature list
type BlockList struct {
	fileHashList string
	blockHashList string
	// hashNames
	hashNames      []string
	hashNamesSize  int
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
}

// hash list struct variable
// this should be in sorted order
var hlist = []HashList{
		{ HashName: "aes8",        Description: "AES Hash 8",		BlockSize: 8 },
		{ HashName: "ax",          Description: "XXHash 128",		BlockSize: 16 },
		{ HashName: "blake2",      Description: "Blake2",		BlockSize: 64 },
		{ HashName: "blake2b",     Description: "Blake2b",		BlockSize: 32},
		{ HashName: "blake2s_128", Description: "Blake2s 128",		BlockSize: 16 },
		{ HashName: "blake2s_256", Description: "Blake2s 256",		BlockSize: 32 },
		{ HashName: "cube",        Description: "Cubehash",		BlockSize: 64 },
		{ HashName: "fnv",         Description: "FNV",			BlockSize: 8 },
		{ HashName: "fnv128",      Description: "FNV 128",		BlockSize: 16 },
		{ HashName: "fnv128a",     Description: "FNV 128a",		BlockSize: 16 },
		{ HashName: "gost256",     Description: "Gost 256",		BlockSize: 32 },
		{ HashName: "gost512",     Description: "Gost 512",		BlockSize: 64 },
		{ HashName: "hmac256",     Description: "HMAC 256",		BlockSize: 32 },
		{ HashName: "hmac512",     Description: "HMAC 512",		BlockSize: 64 },
		{ HashName: "hw64",        Description: "Highway Hash 64",	BlockSize: 8 },
		{ HashName: "hw128",       Description: "Highway Hash 128",	BlockSize: 16 },
		{ HashName: "hw256",       Description: "Highway Hash 256",	BlockSize: 32 },
		{ HashName: "kekkak",      Description: "Kekkak",		BlockSize: 32 },
		{ HashName: "murmur3",     Description: "Murmur3",		BlockSize: 16 },
		{ HashName: "md4",         Description: "MD4",			BlockSize: 16 },
		{ HashName: "md5",         Description: "MD5",			BlockSize: 16 },
		{ HashName: "ripe128",     Description: "Ripe 128",		BlockSize: 16 },
		{ HashName: "ripe160",     Description: "Ripe 160",		BlockSize: 20 },
		{ HashName: "ripe256",     Description: "Ripe 256",		BlockSize: 32 },
		{ HashName: "sea",         Description: "Seahash",		BlockSize: 8 },
		{ HashName: "sha1_128",    Description: "SHA1 128",		BlockSize: 16 },
		{ HashName: "sha1_1284",   Description: "SHA1 1284",		BlockSize: 16 },
		{ HashName: "sha1",        Description: "SHA1",			BlockSize: 20 },
		{ HashName: "sha224",      Description: "SHA224",		BlockSize: 28 },
		{ HashName: "sha256",      Description: "SHA256",		BlockSize: 32 },
		{ HashName: "sha512",      Description: "SHA512",		BlockSize: 64 },
		{ HashName: "sha512_224",  Description: "SHA512 224",		BlockSize: 28 },
		{ HashName: "sha512_256",  Description: "SHA512 256",		BlockSize: 32 },
		{ HashName: "sha3_224",    Description: "SHA3 224",		BlockSize: 28 },
		{ HashName: "sha3_256",    Description: "SHA3 256",		BlockSize: 32 },
		{ HashName: "sha3_384",    Description: "SHA3 384",		BlockSize: 48 },
		{ HashName: "sha3_512",    Description: "SHA3 512",		BlockSize: 64 },
		{ HashName: "siphash",     Description: "Siphash",		BlockSize: 16 },
		{ HashName: "skein_160",   Description: "Skein 160",		BlockSize: 20 },
		{ HashName: "skein_256",   Description: "Skein 256",		BlockSize: 32 },
		{ HashName: "skein_384",   Description: "Skein 384",		BlockSize: 48 },
		{ HashName: "skein_512",   Description: "Skein 512",		BlockSize: 64 },
		{ HashName: "skein_1024",  Description: "Skein 1024",		BlockSize: 128 },
		{ HashName: "tiger",       Description: "Tiger",		BlockSize: 24 },
		{ HashName: "whirlpool",   Description: "Whirlpool",		BlockSize: 64 },
		{ HashName: "xxhash",      Description: "XXHash",		BlockSize: 8 },
                }

// Init returns a new BlockList object  
func Init() (bl *BlockList) {
        blx := new(BlockList)
	blx.hashNames = blx.GetHashList()
	blx.hashNamesSize = len(blx.hashNames)

	blx.SetHashListBlockSize()
        return blx
}

// returns the signature hash list
func (bl *BlockList) GetHashList() ([]string) {


	hashNames := []string{"aes8", "ax", "blake2", "blake2b", "blake2s_128", "blake2s_256", "cube", "fnv", "fnv128", "fnv128a", "gost256", "gost512", "hmac256", "hmac512", "hw64", "hw128", "hw256", "kekkak", "murmur3", "md4", "md5", "ripe128",
		"ripe160", "ripe256", "sea", "sha1_128", "sha1_1284", "sha1", "sha224", "sha256", "sha512", "sha512_224", "sha512_256", "sha3_224", "sha3_256", "sha3_384", "sha3_512", "siphash",
		"skein_160", "skein_256", "skein_384", "skein_512", "skein_1024", "tiger", "whirlpool", "xxhash" }

	return hashNames
}

// returns the signature hash list
func (bl *BlockList) GetHashListObject() ([]HashList) {

	return hlist
}

// CreateHashBlockList takes a hashlist integer list and converts it to a name list
// ie converts the boolean string to a hash name string
func (bl *BlockList) CreateHashBlockList(hashlistBoolean string) ([]string) {

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
                        case 1: bl.AddHashList(hlist[i].HashName, i) 
                }

        }

	bl.blockHashList = strings.Join(bl.hashList, ":")

	// fmt.Println("hashlist ", bl.blockHashList)

	return bl.hashList
}

// add the hash block the blockSize Array and increment the blockSize count
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


// set the block size map
func (bl *BlockList) SetHashListBlockSize () {

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
// this allows mdprint or mdunzip to decode each signature block and calculate their size
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



