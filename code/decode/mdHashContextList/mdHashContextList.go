package mdHashContextList 

// mdencode project
//
// mdHashContextList.go
//
// This package creates a mdencode signature block hash context list
// The modulus scan calls the check routine with the block bytes and compares
// them against the hashcontext list and tests to see if a signature match is found 
//
// It also provides a wrapper to mdBinaryList.go for simplicity
//
// https://github.com/singularian/mdencode
// copyright (C) Scott Ross 2019
//
// License
// https://github.com/singularian/mdencode/blob/master/LICENSE

import (
	"os"
	"bytes"
	"fmt"
	"strings"
	"encoding/hex"
	"hash"
	"hash/fnv"
	"crypto/hmac"
	"golang.org/x/crypto/blake2s"
	"github.com/htruong/go-md2"
	"golang.org/x/crypto/md4"
        "crypto/md5"
        "github.com/singularian/mdhash/sha1_128"
        "crypto/sha1"
        "crypto/sha256"
        "crypto/sha512"
	"golang.org/x/crypto/sha3"
	"github.com/tildeleb/aeshash"
        "github.com/codahale/blake2"
	"github.com/minio/blake2b-simd"
	"github.com/skeeto/cubehash"
	"github.com/martinlindhe/gogost/gost34112012256"
	"github.com/martinlindhe/gogost/gost34112012512"
	"github.com/minio/highwayhash"
	"github.com/phoreproject/go-x11/bmw"
	"github.com/phoreproject/go-x11/luffa"
	"github.com/phoreproject/go-x11/groest"
	"github.com/phoreproject/go-x11/echo"
	"github.com/phoreproject/go-x11/shavite"
	"github.com/maoxs2/go-ripemd"
	"golang.org/x/crypto/ripemd160"
	"github.com/aead/skein"
	"github.com/asadmshah/murmur3"
	"blainsmith.com/go/seahash"
	"github.com/cxmcc/tiger"
	"github.com/dchest/siphash"
	"github.com/jzelinskie/whirlpool"
	"github.com/steakknife/keccak"
	"github.com/OneOfOne/xxhash"
	"github.com/singularian/mdhash/xxhash_128"
	"github.com/singularian/mdhash/poly1305"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
)

// HashContextList object stores the file signature hash context list
type HashContextList struct {
	fileHashList string
	blockHashList string
	MdBlockSize *mdBinaryList.BlockList
	// hash map
	// 
	// hash list for files
	HashList map[string]hash.Hash
        // hash context signature list for files or blocks
        HashListBlocks map[string]hash.Hash
	// hash context count
	HashListFileSize  int
	HashListBlockSize int
	// hashBlockBytes
	// this is the current mdzip file signature bytes
	// it contains the entire byte list
	hashFileBytes  []byte
	hashBlockBytes []byte
        // argument signature hash list names
        fileHashListNames  []string
        blockHashListNames []string
	// file signature size lists
	// should rename this
	hashFileBlockSize uint64
	hashFileBlockSizeList []int
	// block signature size list
	hashBlockSize uint64
	hashBlockSizeList []int
	// Optional hashContextList keys
	key string
	hwkey string
	keylist map[string]string
}

// Init returns a new HashContextList object  
func Init() (hc *HashContextList) {
        hcl := new(HashContextList)

	mdBlock := mdBinaryList.Init()
	hcl.MdBlockSize       = mdBlock
	hcl.HashListFileSize  = 0
	hcl.HashListBlockSize = 0

        return hcl
}

// createHashListMap
// create the hash context hash list map
// need to incorporate the thread number
// has to be either a thread list for each or a key_threadnumber
// create 16 hashlist context objects for each thread once
func (hc *HashContextList) CreateHashListMap(hashList string, mdtype int, threadNumber int) {


	hb := make(map[string]hash.Hash)

	hashlistArr := strings.Split(hashList, ":")

	// key          := "LomaLindaSanSerento9000"
	var defaultkey = []byte("LomaLindaSanSerento9000")
	var key        = []byte("LomaLindaSanSerento9000")

	// set the siphash key
	var siphashkey = []byte(hc.key)
	sipkeysize  := len(siphashkey)
	key = siphashkey
	if (sipkeysize < 15 ) {
		// fmt.Println("setting siphash default ", siphashkey, " ", sipkeysize)
		// fmt.Println("setting siphash default ", defaultkey, " length ", len(key))
		key = defaultkey
	} 

	// set the blake2s key
	// this has a length limit I think of greater than 16
	var blakekey = []byte(hc.key)
	blackkeysize := len(blakekey)
	if ((blackkeysize > 16) || (blackkeysize < 1)) {
		blakekey = defaultkey
	}


	// highway hash key
	// key, err := hex.DecodeString("000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f")
	// hwkey, _ := hex.DecodeString("000102030405060708090a0b0cff0e0f101112131415161718191a1b1c1d1e1f")
	var defhwkey = "000102030405060708090a0b0cff0e0f101112131415161718191a1b1c1d1e1f"
	if hc.hwkey == "" {
		hc.hwkey = defhwkey
	}
	hwkey, err := hex.DecodeString(hc.hwkey)
	if err != nil {
		 fmt.Println("Highway Key error: %v", err, blakekey)
		 os.Exit(1)
	}
	// fmt.Println("hwkey ", hc.hwkey)

	hashlistsize := len(hashlistArr)

	// these hash contexts should be in sorted order
	// for thread := 0; thread < threadNumber; thread++ {
		for hashnum := 0; hashnum < hashlistsize; hashnum++ {
                // fmt.Println("hashlist ", st[i])          
		switch hashlistArr[hashnum] {
			case "aes8":
				// seed is a uint64
				hb["aes8"] = aeshash.NewAES(99123312)
			case "ax":
				hb["ax"] = xxhash_128.New()
                        case "blake2":
				hb["blake2"] = blake2.New(nil)
			// blake2b is similar to blake2 I think it is broken
                        case "blake2b":
				// this NewBlake2b was producing the same hash as blake2.New()
				// hb["blake2b"] = blake2.NewBlake2B() 
				hb["blake2b"] = blake2b.New256()
                        case "blake2s_128":
				b, err := blake2s.New128(blakekey)
				if err != nil {
                                        fmt.Println("#Blake2s_128 error from New128: %v", err, blakekey)
					os.Exit(1)
                                }
				hb["blake2s_128"] = b
                        case "blake2s_256":
				b, err := blake2s.New256(blakekey)
				if err != nil {
					fmt.Println("#Blake2s_256 set error from New256: %v", err, blakekey)
					os.Exit(1)
				}
                                hb["blake2s_256"] = b
			case "bmw":
				hb["bmw"] = bmw.New()
			case "cube":
				hb["cube"] = cubehash.New()
			case "echo":
				hb["echo"] = echo.New()
			case "fnv":
				hb["fnv"] = fnv.New64()
			case "fnva":
				hb["fnva"] = fnv.New64a()
			case "fnv128":
				hb["fnv128"] = fnv.New128()
			case "fnv128a":
				hb["fnv128a"] = fnv.New128a()
			case "gost256":
				hb["gost256"] = gost34112012256.New()
			case "gost512":
				hb["gost512"] = gost34112012512.New()
			case "groest":
				hb["groest"] = groest.New()
			case "hmac256":
				hb["hmac256"] = hmac.New(sha256.New, key)
			case "hmac512":
				hb["hmac512"] = hmac.New(sha512.New, key)
			case "hw64":
				hb["hw64"], _  = highwayhash.New64(hwkey[:])
			case "hw128":
				hb["hw128"], _ = highwayhash.New128(hwkey[:])
			case "hw256":
				hb["hw256"], _ = highwayhash.New(hwkey[:])
			case "kekkak":
				hb["kekkak"] = keccak.New256()
			case "luffa":
				 hb["luffa"] = luffa.New()
                        case "murmur3":
				var seed uint64 = 1120322
				hb["murmur3"] = murmur3.New128(seed)
			case "md2":
				hb["md2"] = md2.New()
                        case "md4":
				hb["md4"] = md4.New()
                        case "md5":
				hb["md5"] = md5.New()
			// Returns a MAC Object
			// poly1305 is a one time hash not usable
			case "poly1305":
				polykey := [32]byte{0xda, 0x84, 0xbc, 0xab, 0x02, 0x67, 0x6c, 0x38, 0xcd, 0xb0, 0x15, 0x60, 0x42, 0x74, 0xc2, 0xaa}
				hb["poly1305"] = poly1305.New(polykey)
			case "ripe128":
				hb["ripe128"] = ripemd.New128()
                        case "ripe160":
				hb["ripe160"] = ripemd160.New()
			case "ripe256":
				 hb["ripe256"] = ripemd.New256()
			case "ripe320":
				hb["ripe320"] = ripemd.New320()
			case "sea":
				hb["sea"] = seahash.New()
                        case "sha1":
				hb["sha1"] = sha1.New()
			case "sha1_128":
				hb["sha1_128"] = sha1_128.New()
			case "sha1_1284":
				hb["sha1_1284"] = sha1_128.New(4, 20)
                        case "sha224":
				hb["sha224"] = sha256.New224()
                        case "sha256":
				hb["sha256"] = sha256.New()
                        case "sha512":
				hb["sha512"] = sha512.New()
                        case "sha512_224":
				hb["sha512_224"] = sha512.New512_224()
                        case "sha512_256":
				hb["sha512_256"] = sha512.New512_256()
                        // case "sha512_384":
                        case "sha3_224":
				hb["sha3_224"] = sha3.New224()
                        case "sha3_256":
				hb["sha3_256"] = sha3.New256()
			case "sha3_384":
				hb["sha3_384"] = sha3.New384()
			case "sha3_512":
				hb["sha3_512"] = sha3.New512()
			case "shavite":
				hb["shavite"] = shavite.New()
			case "siphash":
				hb["siphash"] = siphash.New128(key)
                        case "skein_160":
				hb["skein_160"] = skein.New(20, nil)
                        // case "skein_224":
                        case "skein_256":
				hb["skein_256"] = skein.New256(key)
                        case "skein_384":
				hb["skein_384"] = skein.New(48, nil)
                        case "skein_512":
				hb["skein_512"] = skein.New512(key)
                        case "skein_1024":
				hb["skein_1024"] = skein.New(128, nil)
                        case "tiger":
				hb["tiger"] = tiger.New()
                        case "whirlpool":
				hb["whirlpool"] = whirlpool.New()
			case "xxhash":
				// seed uint64
				hb["xxhash"] =  xxhash.New64()
                	}
		}
	// }

	// 0 is file
	// 1 is block
	// need to add block groups

	if mdtype == 0 {
		hc.fileHashListNames  = hashlistArr
		hc.fileHashList       = hashList 
		hc.HashList           = hb
		hc.HashListFileSize   = hashlistsize
	} else {
		hc.blockHashListNames = hashlistArr
		hc.blockHashList      = hashList
		hc.HashListBlocks     = hb
		hc.HashListBlockSize  = hashlistsize
	}
}



// sets the current mdzip file hash signature block bytes
// this is the list of signature bytes
func (hc *HashContextList) SetFileHashBlock (byteblock []byte) {

	hc.hashFileBytes = byteblock

}

// sets the current mdzip file block hash signature block bytes
// this is the list of signature bytes
func (hc *HashContextList) SetBlockHash (byteblock []byte) {

        hc.hashBlockBytes = byteblock
}

// Get the current Block Hash List Names
// more of a test function
func (hc *HashContextList) GetBlockHash () ([]string) {

        // return hc.hashBlockBytes
	return hc.blockHashListNames

}

// GetFileHashBlockSigBytes creates a list with the signature name and hash block signature bytes 
// Example: [aes8 80696185b5b00db8 blake2s_128 d26d0d2d40f394e279ab14f6f1bb4dde]
func (hc *HashContextList) GetFileHashBlockSigBytes (byteblock []byte) ([]string) {
	var strArray []string	
	//fmt.Println("hashlist ", hc.blockHashListNames)

	var start    int = 0
	var end      int = 0
	// for position, bs := range hc.hashFileBlockSizeList {
	for position, bs := range hc.hashBlockSizeList {
		// end = start + hc.hashBlockSizeList[position]
		end = start + bs

		hex := fmt.Sprintf("%s %x",  hc.blockHashListNames[position], hc.hashBlockBytes[start:end])

		// fmt.Println("GetFileHashBlockSigBytes ", position, start, end, " size ", bs, " hex ", hex)

		strArray = append(strArray, hex)
		start += hc.hashBlockSizeList[position]

	}

	// fmt.Println("block bytes ", strArray)
	return strArray
}


// CheckFileHashBlock compares the current modulus scan byte block with the hash byte block at position start to end
// the hash byte block is the entire signature block 
// [0:20]sha1[21:40]ripe160
func (hc *HashContextList) CheckFileHashBlock (byteblock []byte) (bool) {

	var start    int = 0
	var end      int = 0

	end = hc.hashBlockSizeList[0]

	for position, hashvalue := range hc.blockHashListNames {
		h := hc.HashListBlocks[hashvalue]
		h.Reset()
		h.Write([]byte(byteblock))

		end = start + hc.hashBlockSizeList[position]

		if !bytes.Equal(h.Sum(nil), hc.hashBlockBytes[start:end])  {
			return false
		}
		start += hc.hashBlockSizeList[position] 
	}

	result := hc.GetFileHashBlockSigBytes(byteblock)

	fmt.Printf("Found Block %s result = %x ", result, hc.hashBlockBytes)
	return true
}

// CalcHashBlockSize takes a hashlist colon separated string of hash names
//
// It calculates the total blocksize and a array of the hash signature block sizes
// this allows mdprint or mdunzip to decode each signature block and calculate their size
// returns blocksize and the blocksize array for each block
func (hc *HashContextList) CalcHashBlockSize (hashlist string, mdtype int) (uint64, []int) {

	blocksize, blocklistarr := hc.MdBlockSize.CalcHashBlockSize(hashlist)

	if mdtype == 0 {
		hc.hashFileBlockSize     = blocksize
		hc.hashFileBlockSizeList = blocklistarr
	} else {
		hc.hashBlockSize     = blocksize
		hc.hashBlockSizeList = blocklistarr
	}


	return blocksize, blocklistarr
}


// SetKeyFile
// from MDEncodeALL
// set the md key
// this defaults to a default key if the key is less than 16 bytes
// one of the hash libs faults if the key is to small
// some of the signatures use a key
func (hc *HashContextList) SetKeyFile(key string) {
	hc.key = key
}

// SetHighwayKey
// set the Highway Hash key
// It assigns the 256 bit key
func (hc *HashContextList) SetHighwayKey(key string) {

	var defkey = "000102030405060708090a0b0cff0e0f101112131415161718191a1b1c1d1e1f"
	// hwkey, _ := hex.DecodeString("000102030405060708090a0b0cff0e0f101112131415161718191a1b1c1d1e1f")
	if key != "" {
		hc.hwkey = key
	} else {
		hc.hwkey = defkey
	}

	// fmt.Println("hwkey set ", hc.hwkey, key)
}

// Set the Hash List Key
func (hc *HashContextList) SetHashListKey(keylist string) {

	// var m map[string]string
	hc.keylist = make(map[string]string)

	s := strings.Split(keylist, ",")

	// for index, val := range s {
	for _, val := range s {
		res := strings.Split(val, ":")
		if len(res) == 2 {
			var sig    = res[0]
			var sigkey = res[1]
			switch sig {
                                case "ax1":
                                        hc.keylist[sig] = sigkey
                                case "ax2":
                                        hc.keylist[sig] = sigkey
                                case "hw64":
                                        hc.keylist[sig] = sigkey
                                case "hw128":
                                        hc.keylist[sig] = sigkey
                                case "hw256":
                                        hc.keylist[sig] = sigkey
			}
		}
	}

	fmt.Println("map:", hc.keylist)
}
