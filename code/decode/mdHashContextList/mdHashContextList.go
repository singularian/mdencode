package mdHashContextList 

import (
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
)

// BlockSize object stores the file block signature size
type HashContextList struct {
	fileHashList string
	blockHashList string
	// hash map
	// 
	// hash list for files
	HashList map[string]hash.Hash
        // hash list for blocks
        HashListBlocks map[string]hash.Hash
}

// Init returns a new BlockSize object  
func Init() (hc *HashContextList) {
        hcx := new(HashContextList)
        return hcx
}

// createHashListMap
// create the hash context hash list map
func (hc *HashContextList) CreateHashListMap(hashList string, mdtype int) {


	hb := make(map[string]hash.Hash)

	hashlist := strings.Split(hashList, ":")

	// key          := "LomaLindaSanSerento9000"
	// var defaultkey = []byte("LomaLindaSanSerento9000")
	var key = []byte("LomaLindaSanSerento9000")
	// key = defaultkey

        for i := 0; i < len(hashlist); i++ {
                // fmt.Println("hashlist ", st[i])          
                switch hashlist[i] {
                        case "blake2":
				hb["blake2"] = blake2.New(nil)
                        case "blake2b":
				hb["blake2b"] = blake2.NewBlake2B()
                        case "blake2s_128":
				hb["blake2s_128"], _ = blake2s.New128(key)
                        case "blake2_256":
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
				hb["skein160"] = skein.New(20, nil)
                        // case "skein_224":
                        case "skein_256":
				hb["skein256"] = skein.New256(key)
                        case "skein_384":
				hb["skein384"] = skein.New(48, nil)
                        case "skein_512":
				hb["skein512"] = skein.New512(key)
                        case "skein_1024":
				hb["skein1024"] = skein.New(128, nil)
                        case "tiger":
				hb["tiger"] = tiger.New()
                        case "whirlpool":
				hb["whirlpool"] = whirlpool.New()
                }
	}

	if mdtype == 0 {
		hc.HashList = hb
	} else {
		hc.HashListBlocks = hb
	}
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
			case "sha3_384":
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
