package sigRand

// Package sigRand contains utility functions for generating random signature keys


import (
	"strconv"
	"regexp"
        "crypto/rand"
_	"fmt"
	"encoding/binary"
	"encoding/hex"
)

// GenerateRandomBytes
func GenerateRandomBytes(n int) ([]byte, error) {
	b := make([]byte, n)
	_, err := rand.Read(b)
	// Note that err == nil only if we read len(b) bytes.
	if err != nil {
		return nil, err
	}

	return b, nil
}

func GenerateRandomBytes64(n uint64) ([]byte, error) {
        b := make([]byte, n)
        _, err := rand.Read(b)
        // Note that err == nil only if we read len(b) bytes.
        if err != nil {
                return nil, err
        }

        return b, nil
}


// GenerateRandomHex
func GenerateRandomHex(n int) (string, error) {
	bytes := make([]byte, n)
	if _, err := rand.Read(bytes); err != nil {
		return "", err
	}

	return hex.EncodeToString(bytes), nil
}

// GenerateRandomString returns a securely generated random string.
// it returns a string and error 
func GenerateRandomString(n int) (string, error) {
	const letters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-"

	bytes, err := GenerateRandomBytes(n)
	if err != nil {
		return "", err
	}
	for i, b := range bytes {
		bytes[i] = letters[b%byte(len(letters))]
	}
	return string(bytes), nil
}

// Generate a Randint64
func RandInt64() (int64, error) {
        var b [8]byte
        if _, err := rand.Read(b[:]); err != nil {
                return 0, err
        }
        var result = int64(binary.LittleEndian.Uint64(b[:]))
        if result == 0 {
                result = 1
        }
        return result, nil
}

// Generate a Randuint64
func RandUint64() (uint64, error) {
        var b [8]byte
        if _, err := rand.Read(b[:]); err != nil {
                return 0, err
        }
        var result = uint64(binary.LittleEndian.Uint64(b[:])) 
        if result == 0 {
                result = 1
        }
        return result, nil
}


// Generate a random 32-byte hex string for high way hash
func GenRandomHighwayKey() (string, error) {

	m, _ := GenerateRandomHex(32)

	return m, nil  
}

// Convert a Hex string to a uint64
// hexString := "75bcd105"
// fmt.Println(ConvertHex2Int(hexString))
func ConvertHex2Int(hexString string) uint64 {

        result, _ := strconv.ParseUint(hexString, 16, 64)
        return uint64(result)
}

// Convert a Integer String to a uint64
// "9893489348439"
func ConvertString2Int(DecString string) uint64 {

	result, _ := strconv.ParseUint(DecString, 0, 64)
	return uint64(result)
}

// Convert a Integer String or Hex String to a Uint64
func ConvertDecHexString2Int(DecString string) uint64 {
	isHex, _ := regexp.MatchString("[[:xdigit:]]", DecString)
	isDec, _ := regexp.MatchString("[[:digit:]]",  DecString)

	// fmt.Println("match ", isHex, isDec)

	if isHex {
		return ConvertHex2Int(DecString)
	}

	if isDec {
		return ConvertString2Int(DecString)
	}


	return 0
}
