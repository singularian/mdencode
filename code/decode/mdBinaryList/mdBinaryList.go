package mdBinaryList

// mdencode project
//
// mdBinaryList.go
//
// mdencode mdunzip signature binary list converter
// this package converts a bitstring list to a name string list
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
)

// BlockList object stores the file block signature list
type BlockList struct {
	fileHashList string
	blockHashList string
	// block names
	hashList       []string
	hashListFile   []string
	hashListBlocks []string
}

// Init returns a new BlockList object  
func Init() (bl *BlockList) {
        blx := new(BlockList)
        return blx
}

// returns the signature hash list
func (bl *BlockList) GetHashList() ([]string) {


	hashNames := []string{"blake2", "blake2b", "blake2s_128", "blake2s_256", "fnv", "hmac256", "hmac512", "kekkak", "murmur3", "md4", "md5", "ripe128",
		"ripe160", "ripe256", "sha1_128", "sha1_1284", "sha1", "sha224", "sha256", "sha512", "sha512_224", "sha512_256", "sha3_224", "sha3_256", "sha3_384", "sha3_512", "siphash",
		"skein_160", "skein_256", "skein_384", "skein_512", "skein_1024", "tiger", "whirlpool" }

	return hashNames
}

// CreateHashBlockList takes a hashlist integer list and converts it to a name list
// ie converts the boolean string to a hash name string
func (bl *BlockList) CreateHashBlockList(hashlistBoolean string) ([]string) {

	bl.ClearHashLists()

	re := regexp.MustCompile("[01]")

	arr := re.FindAllString(hashlistBoolean, -1)

	hashNames := []string{"blake2", "blake2b", "blake2s_128", "blake2s_256", "fnv", "hmac256", "hmac512", "kekkak", "murmur3", "md4", "md5", "ripe128", 
				"ripe160", "ripe256", "sha1_128", "sha1_1284", "sha1", "sha224", "sha256", "sha512", "sha512_224", "sha512_256", "sha3_224", "sha3_256", "sha3_384", "sha3_512", "siphash",
				"skein_160", "skein_256", "skein_384", "skein_512", "skein_1024", "tiger", "whirlpool" }


	var length = len(hashNames)

	fmt.Println("hash Name", hashNames,  len(hashNames))

        for i := 0; i < len(arr); i++ {
                // fmt.Println("hashlist ", hashName[i])
		var vbool, _ = strconv.Atoi(arr[i])

		if i >= length {
			break
		}

                switch vbool {
                        case 1: bl.AddHashList(hashNames[i], i) 
                }

        }

	bl.blockHashList = strings.Join(bl.hashList, ":")

	fmt.Println("hashlist ", bl.blockHashList)

	return bl.hashList
}

// add the hash block the blockSize Array and increment the blockSize count
func (bl *BlockList) AddHashList (hashName string, number int) {

	fmt.Println("hash ", number, hashName)

	bl.hashList = append(bl.hashList, hashName)
}

// clear the blocksize counts
func (bl *BlockList) ClearHashLists () {

	bl.hashList       = nil
	bl.hashListFile   = nil
	bl.hashListBlocks = nil

}
