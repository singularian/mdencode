package mdHashContextList 

import (
	"strings"
)

// BlockSize object stores the file block signature size
type HashContextList struct {
	fileHashList string
	blockHashList string
	// hash map
	// 
	// hash list for files
	hashList map[string]hash.Hash
        // hash list for blocks
        hashListBlocks map[string]hash.Hash
}

// Init returns a new BlockSize object  
func Init() (hc *HashContextList) {
        hcx := new(HashContextList)
        return hcx
}

// createHashListMap
// create the hash context hash list map
func (hc *HashContextList) createHashListMap(fileBlockflag int) {


	hb := make(map[string]hash.Hash)

}

// CalcHashBlockSize takes a hashlist colon separated string of hash names
//
// it calculates the total blocksize and a array of the hash signature block sizes
// this allows mdprint or mdunzip to decode each signature block and calculate their size
func (hc *HashContextList) CalcHashBlockSize (hashlist string) (uint64, []int) {
        st := strings.Split(hashlist, ":")

        var blocksize uint64 = 0
        var s []int

        for i := 0; i < len(st); i++ {
                // fmt.Println("hashlist ", st[i])

                switch st[i] {
                        case "blake2":
                        s = append(s, 64)
                        case "blake2b":
                        s = append(s, 64)
                        case "blake2s_128":
                        s = append(s, 16)
                        case "blake2_256":
                        s = append(s, 32)
                        case "murmur3":
                        s = append(s, 16)
                        case "md4":
                        s = append(s, 16)
                        case "md5":
                        s = append(s, 16)
                        case "ripe160":
                        s = append(s, 20)
                        case "sha1":
                        s = append(s, 20)
                        case "sha224":
                        s = append(s, 28)
                        case "sha256":
                        s = append(s, 32)
                        case "sha512":
                        s = append(s, 64)
                        case "sha512_224":
                        s = append(s, 28)
                        case "sha512_256":
                        s = append(s, 32)
                        case "sha512_384":
                        s = append(s, 48)
                        case "sha3_224":
                        s = append(s, 28)
                        case "sha3_256":
                        s = append(s, 32)
                        case "skein_160":
                        s = append(s, 20)
                        case "skein_224":
                        s = append(s, 28)
                        case "skein_256":
                        s = append(s, 32)
                        case "skein_384":
                        s = append(s, 48)
                        case "skein_512":
                        s = append(s, 64)
                        case "skein_1024":
                        s = append(s, 128)
                        case "tiger":
                        s = append(s, 48)
                        case "whirlpool":
                        s = append(s, 64)
                }

        }

        for i := range s {
                blocksize += uint64(s[i])
        }

         return blocksize, s
}
