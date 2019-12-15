package mdBlockSize 

import (
	"strings"
	"fmt"
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

        var blocksize uint64 = 0
        var s []int

	// clear this since the file block is also using it to calculate the file block size
	// it needs this for the whole file signature size for mdunzip and the file block signature size
	bl.ClearBlockSig()

	// set the block hash list
	bl.hashList = hashlist

        for i := 0; i < len(hashName); i++ {
                // fmt.Println("hashlist ", st[i])

                switch hashName[i] {
                        case "blake2":
				s = append(s, 64)
				bl.AddBlockSig(64)
                        case "blake2b":
				s = append(s, 64)
				bl.AddBlockSig(64)
                        case "blake2s_128":
				s = append(s, 16)
				bl.AddBlockSig(16)
                        case "blake2s_256":
				s = append(s, 32)
				bl.AddBlockSig(32)
			case "fnv":
				s = append(s, 8)
				bl.AddBlockSig(8)
			case "hmac256":
				s = append(s, 32)
				bl.AddBlockSig(32)
			case "hmac512":
				s = append(s, 64)
				bl.AddBlockSig(64)
			case "kekkak":
				s = append(s, 32)
				bl.AddBlockSig(32)
                        case "murmur3":
				s = append(s, 16)
				bl.AddBlockSig(16)
                        case "md4":
				s = append(s, 16)
				bl.AddBlockSig(16)
                        case "md5":
				s = append(s, 16)
				bl.AddBlockSig(16)
                        case "ripe160":
				s = append(s, 20)
				bl.AddBlockSig(20)
			case "sha1_128":
				s = append(s, 16)
				bl.AddBlockSig(16)
			case "sha1_1284":
				s = append(s, 16)
				bl.AddBlockSig(16)
			case "sha1":
				s = append(s, 20)
				bl.AddBlockSig(20)
                        case "sha224":
				s = append(s, 28)
				bl.AddBlockSig(28)
                        case "sha256":
				s = append(s, 32)
				bl.AddBlockSig(32)
                        case "sha512":
				s = append(s, 64)
				bl.AddBlockSig(64)
                        case "sha512_224":
				s = append(s, 28)
				bl.AddBlockSig(28)
                        case "sha512_256":
				s = append(s, 32)
				bl.AddBlockSig(32)
                        case "sha3_224":
				s = append(s, 28)
				bl.AddBlockSig(28)
                        case "sha3_256":
				s = append(s, 32)
				bl.AddBlockSig(32)
			case "sha3_384":
				s = append(s, 48)
				bl.AddBlockSig(48)
			case "sha3_512":
				s = append(s, 64)
				bl.AddBlockSig(64)
			case "siphash":
				s = append(s, 16)
				bl.AddBlockSig(16)
                        case "skein_160":
				s = append(s, 20)
				bl.AddBlockSig(20)
                        // case "skein_224":
			// 	s = append(s, 28)
                        case "skein_256":
				s = append(s, 32)
				bl.AddBlockSig(32)
                        case "skein_384":
				s = append(s, 48)
				bl.AddBlockSig(48)
                        case "skein_512":
				s = append(s, 64)
				bl.AddBlockSig(64)
                        case "skein_1024":
				s = append(s, 128)
				bl.AddBlockSig(128)
                        case "tiger":
				s = append(s, 24)
				bl.AddBlockSig(24)
                        case "whirlpool":
				s = append(s, 64)
				bl.AddBlockSig(64)
                }

        }

        for i := range s {
                blocksize += uint64(s[i])
        }

	// fmt.Println("size ", blocksize, s)
	// fmt.Println("size 2222 ",  bl.blockSize, bl.blockSizeArr)

        // return blocksize, s
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
