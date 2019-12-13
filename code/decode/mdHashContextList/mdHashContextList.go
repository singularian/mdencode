package mdHashContextList 

import (
	"bytes"
	"fmt"
	"strings"
	"hash"
	"hash/fnv"
        "crypto/hmac"
	"golang.org/x/crypto/md4"
        "crypto/md5"
        "github.com/singularian/mdencode/code/hash/sha1_128"
        "crypto/sha1"
        "crypto/sha256"
        "crypto/sha512"
        "github.com/codahale/blake2"
	"golang.org/x/crypto/blake2s"
	"golang.org/x/crypto/ripemd160"
	"golang.org/x/crypto/sha3"
	"github.com/aead/skein"
	"github.com/asadmshah/murmur3"
	"github.com/cxmcc/tiger"
	"github.com/dchest/siphash"
	"github.com/jzelinskie/whirlpool"
	"github.com/steakknife/keccak"
	"github.com/singularian/mdencode/code/decode/mdBlockSize"
)

// BlockSize object stores the file block signature size
type HashContextList struct {
	fileHashList string
	blockHashList string
	mdBlockSize *mdBlockSize.BlockSize
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
	// file and block signature size
	hashFileBlockSize uint64
	hashFileBlockSizeList []int
	hashBlockSize uint64
	hashBlockSizeList []int

}

// Init returns a new BlockSize object  
func Init() (hc *HashContextList) {
        hcl := new(HashContextList)

	mdBlock := mdBlockSize.Init()
	hcl.mdBlockSize       = mdBlock
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
	// var defaultkey = []byte("LomaLindaSanSerento9000")
	var key = []byte("LomaLindaSanSerento9000")
	// key = defaultkey

	hashlistsize := len(hashlistArr)

	// for thread := 0; thread < threadNumber; thread++ {
		for hashnum := 0; hashnum < hashlistsize; hashnum++ {
                // fmt.Println("hashlist ", st[i])          
		switch hashlistArr[hashnum] {
                        case "blake2":
				hb["blake2"] = blake2.New(nil)
                        case "blake2b":
				hb["blake2b"] = blake2.NewBlake2B()
                        case "blake2s_128":
				hb["blake2s_128"], _ = blake2s.New128(key)
                        case "blake2s_256":
				hb["blake2s_256"], _ = blake2s.New256(key)
			case "fnv":
				hb["fnv"] = fnv.New64a()
			case "hmac256":
				hb["hmac256"] = hmac.New(sha256.New, key)
			case "hmac512":
				hb["hmac512"] = hmac.New(sha512.New, key)
			case "kekkak":
				hb["kekkak"] = keccak.New256()
                        case "murmur3":
				var seed uint64 = 1120322
				hb["murmur3"] = murmur3.New128(seed)
                        case "md4":
				hb["md4"] = md4.New()
                        case "md5":
				hb["md5"] = md5.New()
                        case "ripe160":
				hb["ripe160"] = ripemd160.New()
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
                	}
		}
	// }

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

func (hc *HashContextList) GetBlockHash () ([]string) {

        // return hc.hashBlockBytes
	return hc.blockHashListNames

}


// this compares the current modulus scan byte block with the hash byte block at position start to end
// the hash byte block is the entire signature block 
// [0:20]sha1[21:40]ripe160
func (hc *HashContextList) CheckFileHashBlock (byteblock []byte) (bool) {

	var position int = 0
	var start    int = 0
	var end      int = 0
	// hc.hashBlockBytes[start:end]

	// fmt.Println("******************* hashlist check ", hc.hashBlockSizeList, byteblock)
	end = hc.hashBlockSizeList[0]
	// for hashkey, hashvalue := range hc.HashListBlocks {
	// for _, hashvalue := range l.blockHashListNames {
	for _, hashvalue := range hc.blockHashListNames {
		// fmt.Println("hash ", hashkey)
		//////////////// fmt.Println("hash ", hashvalue)
		h := hc.HashListBlocks[hashvalue]
		h.Reset()
		// hashvalue.Reset()
		//  bytes[start:blockXsize]
		// hashvalue.Write(byteblock[])
		//////// hashvalue.Write([]byte(byteblock))
		// h.Reset()
		h.Write([]byte(byteblock))
		end = start + hc.hashBlockSizeList[position]
		// if position > 0 {
		//	start = hc.hashBlockSizeList[position]
		//}
		// end = start + hc.hashBlockSizeList[position]
		///////////////////////////////////////////////////// fmt.Println("start end ", start, end)
		// fmt.Println("start end ", start, end, hc.hashBlockBytes[start:end])
		// if bytes.Equal(md5.Sum(nil), md.md5byteblock) {
		/* if bytes.Equal(hashvalue.Sum(nil), hc.hashBlockBytes[start:end])  {
			fmt.Printf("test bytes true %x %x\n", hashvalue.Sum(nil), hc.hashBlockBytes[start:end])
		} else {
			fmt.Printf("test bytes false %x %x\n", hashvalue.Sum(nil), hc.hashBlockBytes[start:end])
			// return false
		} */
		if !bytes.Equal(h.Sum(nil), hc.hashBlockBytes[start:end])  {
	//		fmt.Printf("FALSE -- test bytes false %s start %d end %d %x %x\n", hashvalue, start, end, h.Sum(nil), hc.hashBlockBytes[start:end])
			// h.Reset()
			return false
		} else {
	//		fmt.Printf("TRUE ---- test bytes true %s %d %d %x %x\n", hashvalue, start, end, h.Sum(nil), hc.hashBlockBytes[start:end])
		}
		// fmt.Printf("hash %s bytes hex % x\n", hashkey, hashvalue.Sum(nil))
		// h.Reset()
		start += hc.hashBlockSizeList[position] 
		// end = start
		// fmt.Println("next ", start, end)
		position++
	}

	var result = ""
	for _, hashvalues := range hc.blockHashListNames {
		h := hc.HashListBlocks[hashvalues]
		result += fmt.Sprintf("%s %x ", hashvalues, h.Sum(nil))
	}
	

	fmt.Printf("Found Block %s result = %x ", result, hc.hashBlockBytes)
	return true
	// return false

}

// CalcHashBlockSize takes a hashlist colon separated string of hash names
//
// it calculates the total blocksize and a array of the hash signature block sizes
// this allows mdprint or mdunzip to decode each signature block and calculate their size
func (hc *HashContextList) CalcHashBlockSize (hashlist string, mdtype int) (uint64, []int) {

	blocksize, blocklistarr := hc.mdBlockSize.CalcHashBlockSize(hashlist)

	if mdtype == 0 {
		hc.hashFileBlockSize     = blocksize
		hc.hashFileBlockSizeList = blocklistarr
	} else {
		hc.hashBlockSize     = blocksize
		hc.hashBlockSizeList = blocklistarr
	}


	return blocksize, blocklistarr
}
