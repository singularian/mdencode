// SHA-1 128 is a 128-bit or 16 byte hash of the SHA-1
// This is a MD5 equivalent SHA1 function and tests wrapping the hashes
package sha1_128

import (
_	"crypto"
//	"encoding/binary"
//	"errors"
	"hash"
	// "github.com/singularian/mdencode/code/hash/sha1_128_block"
	"crypto/sha1"
)

// The size of a SHA-1 checksum in bytes.
const Size = 16 

// The blocksize of SHA-1 in bytes.
const BlockSize = 64

const SHA1_128 = 128

// digest represents the partial evaluation of a checksum.
type digest struct {
	sha1hash hash.Hash
}

func init() {
	// crypto.RegisterHash(crypto.SHA1_128, New)
	// crypto.RegisterHash(SHA1_128, New)
}

func (d *digest) Reset() {
	d.sha1hash.Reset()
}

func (d *digest) Size() int { return Size }
func (d *digest) BlockSize() int { return BlockSize }

func (d *digest) Write(p []byte) (nn int, err error) {
	d.sha1hash.Write(p)
	return

}

// New returns a new hash.Hash computing the SHA1 checksum. The Hash also
// implements encoding.BinaryMarshaler and encoding.BinaryUnmarshaler to
// marshal and unmarshal the internal state of the hash.
func New() hash.Hash {
	d := new(digest)
	h  := sha1.New()
	d.sha1hash = h
	// d.Reset()
	return d
}

func (d *digest) Sum(in []byte) []byte {
	// Make a copy of d so that caller can keep writing and summing.
	sl := d.sha1hash.Sum(nil)

	return sl[0:16]
}

