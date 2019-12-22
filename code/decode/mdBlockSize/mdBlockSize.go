package mdBlockSize 

// mdencode project
//
// mdBlockSize.go
//
// mdencode mdunzip signature block size calculator
// this package calculates a signature block size so it can be decrypted by mdunzip
// it creates a blockSize sum and a list of the signature block sizes
//
// https://github.com/singularian/mdencode
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"strings"
//	"fmt"
)

// BlockSize object stores the file block signature size
type BlockSize struct {
	hashList string
	blockList string
	blockSize uint64
	blockSizeArr []int
}

// Init returns a new BlockSize object  
func Init() (bl *BlockSize) {
        blx := new(BlockSize)
	blx.blockSize  = 0
        return blx
}

// CalcHashBlockSize takes a hashlist colon separated string of hash names
//
// it calculates the total blocksize and a array of the hash signature block sizes
// this allows mdprint or mdunzip to decode each signature block and calculate their size
func (bl *BlockSize) CalcHashBlockSize (hashlist string) (uint64, []int) {
        hashName := strings.Split(hashlist, ":")

	// clear this since the file block is also using it to calculate the file block size
	// it needs this for the whole file signature size for mdunzip and the file block signature size
	bl.ClearBlockSig()

	// set the block hash list
	bl.hashList = hashlist

        for i := 0; i < len(hashName); i++ {
                // fmt.Println("hashlist ", hashName[i])

                switch hashName[i] {
                        case "blake2":
				bl.AddBlockSig(64)
                        case "blake2b":
				bl.AddBlockSig(64)
                        case "blake2s_128":
				bl.AddBlockSig(16)
                        case "blake2s_256":
				bl.AddBlockSig(32)
			case "cube":
				bl.AddBlockSig(64)
			case "fnv":
				bl.AddBlockSig(8)
			case "fnv128":
				bl.AddBlockSig(16)
			case "fnv128a":
				bl.AddBlockSig(16)
			case "gost256":
				bl.AddBlockSig(32)
			case "gost512":
				bl.AddBlockSig(64)
			case "hmac256":
				bl.AddBlockSig(32)
			case "hmac512":
				bl.AddBlockSig(64)
			case "hw128":
				bl.AddBlockSig(16)
			case "hw256":
				bl.AddBlockSig(32)
			case "kekkak":
				bl.AddBlockSig(32)
                        case "murmur3":
				bl.AddBlockSig(16)
                        case "md4":
				bl.AddBlockSig(16)
                        case "md5":
				bl.AddBlockSig(16)
			case "ripe128":
				bl.AddBlockSig(16)
                        case "ripe160":
				bl.AddBlockSig(20)
			case "ripe256":
				bl.AddBlockSig(32)
			case "sha1_128":
				bl.AddBlockSig(16)
			case "sha1_1284":
				bl.AddBlockSig(16)
			case "sha1":
				bl.AddBlockSig(20)
                        case "sha224":
				bl.AddBlockSig(28)
                        case "sha256":
				bl.AddBlockSig(32)
                        case "sha512":
				bl.AddBlockSig(64)
                        case "sha512_224":
				bl.AddBlockSig(28)
                        case "sha512_256":
				bl.AddBlockSig(32)
                        case "sha3_224":
				bl.AddBlockSig(28)
                        case "sha3_256":
				bl.AddBlockSig(32)
			case "sha3_384":
				bl.AddBlockSig(48)
			case "sha3_512":
				bl.AddBlockSig(64)
			case "siphash":
				bl.AddBlockSig(16)
                        case "skein_160":
				bl.AddBlockSig(20)
                        // case "skein_224":
			//	bl.AddBlockSig(28)
                        case "skein_256":
				bl.AddBlockSig(32)
                        case "skein_384":
				bl.AddBlockSig(48)
                        case "skein_512":
				bl.AddBlockSig(64)
                        case "skein_1024":
				bl.AddBlockSig(128)
                        case "tiger":
				bl.AddBlockSig(24)
                        case "whirlpool":
				bl.AddBlockSig(64)
			case "xxhash":
				bl.AddBlockSig(8)
                }

        }

	// fmt.Println("size ",  bl.blockSize, bl.blockSizeArr)

	return bl.blockSize, bl.blockSizeArr
}

// add the hash block the blockSize Array and increment the blockSize count
func (bl *BlockSize) AddBlockSig (hashSize int) {

	bl.blockSizeArr = append(bl.blockSizeArr, hashSize)
	bl.blockSize += uint64(hashSize)
}

// clear the blocksize counts
func (bl *BlockSize) ClearBlockSig () {

	bl.blockSizeArr = nil
	bl.blockSize = 0

}
