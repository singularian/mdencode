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
	"hash/adler32"
	"hash/fnv"
	"hash/crc32"
	"hash/crc64"
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
	"github.com/martinlindhe/gogost/gost34112012256"
	"github.com/martinlindhe/gogost/gost34112012512"
	"github.com/minio/highwayhash"
	"github.com/phoreproject/go-x11/bmw"
	"github.com/phoreproject/go-x11/luffa"
	"github.com/phoreproject/go-x11/groest"
	"github.com/phoreproject/go-x11/echo"
	"github.com/phoreproject/go-x11/shavite"
	"github.com/maoxs2/go-ripemd"
	"github.com/mmcloughlin/meow"
	"github.com/asadmshah/murmur3"
	"golang.org/x/crypto/ripemd160"
	"github.com/aead/skein"
	"blainsmith.com/go/seahash"
	"github.com/cxmcc/tiger"
	sip "github.com/aead/siphash"
	"github.com/dchest/siphash"
	"github.com/jzelinskie/whirlpool"
	"github.com/steakknife/keccak"
	"github.com/OneOfOne/xxhash"
	"github.com/singularian/mdhash/cityHash64"
	"github.com/singularian/mdhash/cubehash"
	_ "github.com/singularian/mdhash/farmHash32"
	"github.com/singularian/mdhash/farmHash64"
	"github.com/singularian/mdhash/hw32"
	"github.com/singularian/mdhash/jenkins64"
	"github.com/singularian/mdhash/murmur332"
	"github.com/singularian/mdhash/poly1305"
	"github.com/singularian/mdhash/spooky64"
	"github.com/singularian/mdhash/xxhash_128"
	"github.com/orisano/wyhash/v2"
	"github.com/singularian/mdencode/code/decode/mdBinaryList"
	"github.com/singularian/mdencode/code/decode/sigRand"
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
	keylist map[string]string
	// error 
	err error
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

	// initialize the hash list context map
	hb := make(map[string]hash.Hash)

	// create the hashlist names array
	hashlistArr := strings.Split(hashList, ":")
	hashlistsize := len(hashlistArr)

	// set the default key
	var defkey     = hc.keylist["default"]
	var defaultkey = []byte(defkey)

	// these hash contexts should be in sorted order
	// for thread := 0; thread < threadNumber; thread++ {
		for _, hashname := range hashlistArr {

		switch hashname {
			case "add32":
				hb["add32"] = adler32.New()
			case "aes8":
				// seed is a uint64
				var key = hc.keylist[hashname]
				hb["aes8"] = aeshash.NewAES(sigRand.ConvertString2Int(key))
			case "ax":
				var key  = hc.keylist["ax1"]
				var key2 = hc.keylist["ax2"]
				hb["ax"] = xxhash_128.New(sigRand.ConvertString2Int(key), sigRand.ConvertString2Int(key2))
                        case "blake2":
				hb["blake2"] = blake2.New(nil)
			// blake2b is similar to blake2 I think it is broken
                        case "blake2b":
				// this NewBlake2b was producing the same hash as blake2.New()
				// hb["blake2b"] = blake2.NewBlake2B() 
				hb["blake2b"] = blake2b.New256()
                        case "blake2s_128":
				var key = hc.keylist[hashname]
				var blakekey = []byte(key)

				b, err := blake2s.New128(blakekey)
				hc.CheckKeyError(hashname, string(blakekey), err)
				hb["blake2s_128"] = b
                        case "blake2s_256":
				// if ((blakeKeysize > 16) || (blakeKeysize < 1)) {
				var key = hc.keylist[hashname]
				var blakekey = []byte(key)

				b, err := blake2s.New256(blakekey)
				hc.CheckKeyError(hashname, string(blakekey), err)
                                hb["blake2s_256"] = b
			case "bmw":
				hb["bmw"] = bmw.New()
			case "crc32":
				hb["crc32"] = crc32.New(crc32.MakeTable(crc32.IEEE))
			case "crc32c":
				hb["crc32c"] = crc32.New(crc32.MakeTable(crc32.Castagnoli))
			case "crc32k":
				hb["crc32k"] = crc32.New(crc32.MakeTable(crc32.Koopman))
			case "crc64ecma":
				hb["crc64ecma"] = crc64.New(crc64.MakeTable(crc64.ECMA)) 
			case "crc64iso":
				 hb["crc64iso"] = crc64.New(crc64.MakeTable(crc64.ISO))
			case "ct64":
				var key = hc.keylist[hashname]
				hb["ct64"] = cityHash64.New(0, 8, sigRand.ConvertString2Int(key))
			case "cube":
				hb["cube"] = cubehash.New()
			case "echo":
				hb["echo"] = echo.New()
			case "fh32":
				// hb["fh32"] = farmHash32.New(0, 4)
				var key = hc.keylist[hashname]
				hb["fh32"] = farmHash64.New(0, 4, sigRand.ConvertString2Int(key))
			case "fh64":
				// seed is a uint64
				var key = hc.keylist[hashname]
				hb["fh64"] = farmHash64.New(0, 8, sigRand.ConvertString2Int(key))
			case "fnv32":
				hb["fnv32"] = fnv.New32()
			case "fnv32a":
				hb["fnv32a"] = fnv.New32a()
			case "fnv64":
				hb["fnv64"] = fnv.New64()
			case "fnv64a":
				hb["fnv64a"] = fnv.New64a()
			case "fnv128":
				hb["fnv128"] = fnv.New128()
			case "fnv128a":
				hb["fnv128a"] = fnv.New128a()
			case "gost256":
				// I think this hash is slow
				hb["gost256"] = gost34112012256.New()
			case "gost512":
				hb["gost512"] = gost34112012512.New()
			case "groest":
				hb["groest"] = groest.New()
			// these two have a key
			case "hmac256":
				hb["hmac256"] = hmac.New(sha256.New, defaultkey)
			case "hmac512":
				hb["hmac512"] = hmac.New(sha512.New, defaultkey)
			case "hw32":
				var key = hc.keylist[hashname]
                                hwkey, err := hex.DecodeString(key)
				hc.CheckKeyError(hashname, key, err)

				hb["hw32"] = hw32.New(0, 4, hwkey[:])
				hc.CheckKeyError(hashname, key, hc.err)
			case "hw64":
				var key = hc.keylist[hashname]
				hwkey, err := hex.DecodeString(key)
				// test
				// key = "11ZZZ"
				hc.CheckKeyError(hashname, key, err)

				hb["hw64"], hc.err  = highwayhash.New64(hwkey[:])
				hc.CheckKeyError(hashname, key, hc.err)
			case "hw128":
				var key = hc.keylist[hashname]
				hwkey, err := hex.DecodeString(key)
				hc.CheckKeyError(hashname, key, err)

				hb["hw128"], hc.err = highwayhash.New128(hwkey[:])
				hc.CheckKeyError(hashname, key, hc.err)
			case "hw256":
				var key = hc.keylist[hashname]
				hwkey, err := hex.DecodeString(key)
				hc.CheckKeyError(hashname, key, err)

				hb["hw256"], hc.err = highwayhash.New(hwkey[:])
				hc.CheckKeyError(hashname, key, hc.err)
			case "jn64":
				var key = hc.keylist[hashname]
				hb["jn64"] = jenkins64.New(0, 8, sigRand.ConvertString2Int(key))
			case "kekkak":
				hb["kekkak"] = keccak.New256()
			case "luffa":
				 hb["luffa"] = luffa.New()
			case "me32":
				var key = hc.keylist[hashname]
				// hb["me32"] = meow.New32(sigRand.ConvertString2Int(key), 4)
				hb["me32"] = meow.New32(sigRand.ConvertString2Int(key))
			case "me64":
				var key = hc.keylist[hashname]
				// hb["me64"] = meow.New64(sigRand.ConvertString2Int(key), 8)
				hb["me64"] = meow.New64(sigRand.ConvertString2Int(key))
			case "me128":
				var key = hc.keylist[hashname]
				hb["me128"] = meow.New(sigRand.ConvertString2Int(key))
			case "mm32":
				var key = hc.keylist[hashname]
				hb["mm32"] = murmur332.New(0, 4, sigRand.ConvertString2Int32(key))
                        case "murmur3":
				// var seed uint64 = 1120322
				var key = hc.keylist[hashname]
				hb["murmur3"] = murmur3.New128(sigRand.ConvertString2Int(key))
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
			case "sha1_64":
				hb["sha1_64"] = sha1_128.New(0, 8)
			case "sha1_128":
				hb["sha1_128"] = sha1_128.New()
			case "sha1_1284":
				hb["sha1_1284"] = sha1_128.New(4, 20)
                        case "sha224":
				hb["sha224"] = sha256.New224()
                        case "sha256":
				hb["sha256"] = sha256.New()
                        case "sha512_224":
				hb["sha512_224"] = sha512.New512_224()
                        case "sha512_256":
				hb["sha512_256"] = sha512.New512_256()
                        case "sha512_384":
				hb["sha512_384"] = sha512.New384()
			case "sha512":
				hb["sha512"] = sha512.New()
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
			case "sip64":
				// 16 byte key
				var key = hc.keylist[hashname]
				sipkey, err := hex.DecodeString(key)
				hc.CheckKeyError(hashname, key, err)

				hb[hashname], hc.err = sip.New64(sipkey)
				hc.CheckKeyError(hashname, key, hc.err)
			case "sip128":
				// 16 byte key
				var key = hc.keylist[hashname]
				sipkey128, err := hex.DecodeString(key)
				hc.CheckKeyError(hashname, key, err)

				hb[hashname] = siphash.New128(sipkey128)
                        case "skein_160":
				hb["skein_160"] = skein.New(20, nil)
                        // case "skein_224":
                        case "skein_256":
				hb["skein_256"] = skein.New256(defaultkey)
                        case "skein_384":
				hb["skein_384"] = skein.New(48, nil)
                        case "skein_512":
				hb["skein_512"] = skein.New512(defaultkey)
                        case "skein_1024":
				hb["skein_1024"] = skein.New(128, nil)
			case "spk32":
				var key = hc.keylist[hashname]
				hb["spk32"] = spooky64.New(0, 4, sigRand.ConvertString2Int(key))
			case "spk64":
				var key = hc.keylist[hashname]
				hb["spk64"] = spooky64.New(0, 8, sigRand.ConvertString2Int(key))
                        case "tiger":
				hb["tiger"] = tiger.New()
                        case "whirlpool":
				hb["whirlpool"] = whirlpool.New()
			case "xxh32":
				var key = hc.keylist[hashname]
				hb["xxh32"] = xxhash.NewS32(sigRand.ConvertString2Int32(key))
			case "xxhash":
				// seed uint64
				// xxhash.NewS64(seed2) // seed key version
				// xxhash.New64()       // non seed key version
				var key = hc.keylist[hashname]
                                hb["xxhash"] = xxhash.NewS64(sigRand.ConvertString2Int(key))
			case "wy":
				var key = hc.keylist[hashname]
				hb["wy"]  = wyhash.New(sigRand.ConvertString2Int(key))
			default:
				fmt.Println("Unknown Hash Context List Signature ", hashname)
				os.Exit(2)
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

// GetBlockHashBytes
// return the current mdzip block hash signature block bytes
func (hc *HashContextList) GetBlockHashBytes () ([]byte) {

	return hc.hashBlockBytes

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

// CalcHashBlockSizeFileList takes a hashlist colon separated string of hash names
//
// It calculates the total blocksize and a array of the hash signature block sizes
// this allows mdprint or mdunzip to decode each signature block and calculate their size
// returns blocksize and the blocksize array for each block
func (hc *HashContextList) CalcHashBlockSizeFileList (hashlist string) (uint64, []int) {

	blocksize, blocklistarr := hc.MdBlockSize.CalcHashBlockSize(hashlist)

	hc.hashFileBlockSize     = blocksize
	hc.hashFileBlockSizeList = blocklistarr


	return blocksize, blocklistarr
}


// CalcHashBlockSizeBlockList takes a hashlist colon separated string of hash names
//
// It calculates the total blocksize and a array of the hash signature block sizes
// this allows mdprint or mdunzip to decode each signature block and calculate their size
// returns blocksize and the blocksize array for each block
func (hc *HashContextList) CalcHashBlockSizeBlockList (hashlist string) (uint64, []int) {

        blocksize, blocklistarr := hc.MdBlockSize.CalcHashBlockSize(hashlist)

	hc.hashBlockSize     = blocksize
	hc.hashBlockSizeList = blocklistarr


        return blocksize, blocklistarr
}



// Set the Hash List Key
// should return a string not sure if I need this
// it's so that if a default is used it will specify it in the mdunzip key list
func (hc *HashContextList) SetHashListKey(keylist string) (string) {

	// fmt.Println("Setting Hashlistkey ", keylist)

	hc.keylist = make(map[string]string)

	keylist = keylist + ","
	keys   := strings.Split(keylist, ",")

	// default hash list keys
	var defaultkey   = "LomaLindaSanSerento9000"
	var defaulthwkey = "000102030405060708090a0b0cff0e0f101112131415161718191a1b1c1d1e1f"

	hc.keylist["default"]     = defaultkey
	hc.keylist["aes8"]        = "99123312"
	hc.keylist["ax1"]         = "991209123091283"
	hc.keylist["ax2"]         = "9881223091283"
	// hc.keylist["blake"]    = defaultkey
	hc.keylist["blake2s_128"] = defaultkey
	hc.keylist["blake2s_256"] = defaultkey
	hc.keylist["ct64"]        = "111111222"
	hc.keylist["fh32"]        = "909921232221" 
	hc.keylist["fh64"]        = "909921232221" 
	hc.keylist["hw32"]        = defaulthwkey 
	hc.keylist["hw64"]        = defaulthwkey 
	hc.keylist["hw128"]       = defaulthwkey 
	hc.keylist["hw256"]       = defaulthwkey
	hc.keylist["jn64"]        = "132200231"
	hc.keylist["me32"]        = "11203221"
	hc.keylist["me64"]        = "6660322"
	hc.keylist["me128"]       = "9920322"
	hc.keylist["mm32"]        = "1120322"
	hc.keylist["murmur3"]     = "1120322"
	hc.keylist["sip64"]       = "000102030405060708090a0b0c0d0e0f"
	hc.keylist["sip128"]      = "000102030405060708090a0b0c0d0e0f"
	hc.keylist["spk32"]       = "1238821"
	hc.keylist["spk64"]       = "7072112122"
	hc.keylist["xxh32"]       = "99123312"
	hc.keylist["xxhash"]      = "99123312"

	var result string

	// iterate over the keylist
	for _, val := range keys {
		res := strings.Split(val, ":")
		if len(res) == 2 {
			var sig        = res[0]
			var sigkey     = res[1]
			var sigkeysize = len(res[1])
			switch sig {
				case "aes8":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
                                case "ax1":
                                        hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
                                case "ax2":
                                        hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				//case "blake2":
				//	hc.keylist[sig] = sigkey
				case "blake2s_128":
					// fmt.Println("blake2s_128 ", sig, sigkey)
					// if ((blakeKeysize > 16) || (blakeKeysize < 1)) {
					if (sigkeysize >= 16) {
						hc.keylist[sig] = sigkey
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
				case "blake2s_256":
					// fmt.Println("blake2s_256 ", sig, sigkey)
					if (sigkeysize >= 16) {
						hc.keylist[sig] = sigkey
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
				case "ct64":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "fh32":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "fh64":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "hw32":
					if sigkeysize == 64 {
						hc.keylist[sig] = sigkey
					} else if sigkeysize > 1 && sigkeysize < 64 {
						hc.keylist[sig] = fmt.Sprintf("%064s", sigkey)
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
                                case "hw64":
					if sigkeysize == 64 {
						hc.keylist[sig] = sigkey
					} else if sigkeysize > 1 && sigkeysize < 64 {
						hc.keylist[sig] = fmt.Sprintf("%064s", sigkey) 
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
                                case "hw128":
					if sigkeysize == 64 {
						hc.keylist[sig] = sigkey
					} else if sigkeysize > 1 && sigkeysize < 64 {
						hc.keylist[sig] = fmt.Sprintf("%064s", sigkey)
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
                                case "hw256":
					if sigkeysize == 64 {
                                        hc.keylist[sig] = sigkey
					} else if sigkeysize > 1 && sigkeysize < 64 {
						hc.keylist[sig] = fmt.Sprintf("%064s", sigkey)
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
				case "jn64":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "me32":
                                        hc.keylist[sig] = sigkey
                                        result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "me64":
                                        hc.keylist[sig] = sigkey
                                        result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "me128":
                                        hc.keylist[sig] = sigkey
                                        result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "mm32":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "murmur3":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "sip64":
					// siphash uses a 16 byte key
					if sigkeysize == 32 {
						hc.keylist[sig] = sigkey
					} else if sigkeysize > 1 && sigkeysize < 32 {
						hc.keylist[sig] = fmt.Sprintf("%032s", sigkey)
					}
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
				case "sip128":
					// siphash uses a 16 byte key 
					if sigkeysize == 32 {
						hc.keylist[sig] = sigkey
					} else if sigkeysize > 1 && sigkeysize < 32 {
						hc.keylist[sig] = fmt.Sprintf("%032s", sigkey)
					} 
					result += fmt.Sprintf("%s:%s,", sig, hc.keylist[sig])
				case "spk32":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "spk64":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "xxh32":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "xxhash":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				case "wy":
					hc.keylist[sig] = sigkey
					result += fmt.Sprintf("%s:%s,", sig, sigkey)
				default:
					fmt.Println("Unknown key", sig)
			}
		}
	}

	// for k, v := range hc.keylist {
	// 	result += fmt.Sprintf("%s:%s,", k, v)
	// }

	// fmt.Println("keylist map:", hc.keylist)
	// fmt.Println("keylist result map:", result)

	return result
}

// Check if a Signature Key Returns an error
func (hc *HashContextList) CheckKeyError(key string, keyval string, err error) { 

	if err != nil {
	// fmt.Println("#Blake2s_128 error from New128: %v", err, blakekey)
		fmt.Printf("Context Signature Key error %s keyvalue %s error %v\n", key, keyval, err)
		os.Exit(1)
	}

}
