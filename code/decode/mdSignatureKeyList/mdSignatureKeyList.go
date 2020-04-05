package mdSignatureKeyList

// Package mdSignatureKeyList contains the signature keys list


import (
_	"fmt"
)

// KeyList Struct
type KeyList struct {
        KeyID		string
        KeyDescription	string
        KeyType		string
}

var keylist = []KeyList{
                { KeyID: "aes8",	KeyDescription: "aes8        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "ax1",		KeyDescription: "ax1         ", KeyType: "(xxhash128 key 1) uint64 integer 1 to 18 numbers" },
                { KeyID: "ax2",		KeyDescription: "ax2         ", KeyType: "(xxhash128 key 2) uint64 integer 1 to 18 numbers" },
                { KeyID: "blake2s_128",	KeyDescription: "blake2s_128 ", KeyType: "16+ hex characters" },
                { KeyID: "blake2s_256",	KeyDescription: "blake2s_256 ", KeyType: "16+ hex characters" },
                { KeyID: "ct64",	KeyDescription: "ct64        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "fh32",	KeyDescription: "fh32        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "fh64",	KeyDescription: "fh64        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "hw32",	KeyDescription: "hw32        ", KeyType: "1 to 64 hex characters" },
                { KeyID: "hw64",	KeyDescription: "hw64        ", KeyType: "1 to 64 hex characters" },
                { KeyID: "hw128",	KeyDescription: "hw128       ", KeyType: "1 to 64 hex characters" },
                { KeyID: "hw256",	KeyDescription: "hw256       ", KeyType: "1 to 64 hex characters" },
                { KeyID: "jn64",	KeyDescription: "jn64        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "me32",	KeyDescription: "me32        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "me64",	KeyDescription: "me64        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "me128",	KeyDescription: "me128       ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "mm32",	KeyDescription: "mm32        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "murmur3",	KeyDescription: "murmur3     ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "sip64",	KeyDescription: "sip64       ", KeyType: "1 to 32 hex characters" },
                { KeyID: "sip128",	KeyDescription: "sip128      ", KeyType: "1 to 32 hex characters" },
                { KeyID: "spk32",	KeyDescription: "spk32       ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "spk64",	KeyDescription: "spk64       ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "t1ha",	KeyDescription: "t1ha        ", KeyType: "uint64 integer 1 to 18 numbers" },
                { KeyID: "xxh32",	KeyDescription: "xxh32       ", KeyType: "uint32 integer 1 to 9 numbers" },
                { KeyID: "wy",		KeyDescription: "wy          ", KeyType: "uint64 integer 1 to 18 numbers" },
}

// GetKeyList
func GetKeyList() (string) {
	keys := `Keylist:
aes8        - uint64 integer 1 to 18 numbers
ax1         - (xxhash128 key 1) uint64 integer 1 to 18 numbers
ax2         - (xxhash128 key 2) uint64 integer 1 to 18 numbers
blake2s_128 - 16+ hex characters
blake2s_256 - 16+ hex characters
ct64        - uint64 integer 1 to 18 numbers
fh32        - uint64 integer 1 to 18 numbers
fh64        - uint64 integer 1 to 18 numbers
hw32        - 1 to 64 hex characters
hw64        - 1 to 64 hex characters
hw128       - 1 to 64 hex characters
hw256       - 1 to 64 hex characters
jn64        - uint64 integer 1 to 18 numbers
me32        - uint64 integer 1 to 18 numbers
me64        - uint64 integer 1 to 18 numbers
me128       - uint64 integer 1 to 18 numbers
mm32        - uint32 integer 1 to 9 numbers
murmur3     - uint64 integer 1 to 18 numbers
sip64       - 1 to 32 hex characters
sip128      - 1 to 32 hex characters
spk32       - uint64 integer 1 to 18 numbers
spk64       - uint64 integer 1 to 18 numbers
t1ha        - uint64 integer 1 to 18 numbers
xxh32       - uint32 integer 1 to 9 numbers
xxhash64    - uint64 integer 1 to 18 numbers
wy          - uint64 integer 1 to 18 numbers
	`

	return keys
}

