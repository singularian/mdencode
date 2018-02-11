// Package mdRand contains utility functions for working with random bit strings
package mdRand


import (
        "crypto/rand"
	"fmt"
	"encoding/binary"
)

// generate a rand bit string for the file hash list or block hash list
// this use crypto/rand 
func GetRandomBits(length int) string {
        var result string = ""

        var n int32
        binary.Read(rand.Reader, binary.LittleEndian, &n)
        c := n % 50
        if c < 0 {
                c = 5
        }

        b := make([]byte, c)
        _, _ = rand.Read(b)

        // The slice should now contain random bytes instead of only zeroes.
        for v := range b {
                //      s := fmt.Sprintf("%b", b[v])
                s := fmt.Sprintf("%b", b[v]%2)
                result += s
        }
        if result == "" {
                return "1"
        }

        return result
}

// generate a random file block size
func GetRandomBlockSize () (int64) {
	var result int64 = 50

        // if _, err := rand.Read(b[:]); err != nil {
        //        return 500
        //}
        // result = int64(binary.LittleEndian.Uint64(b[:])) % 1000
	binary.Read(rand.Reader, binary.LittleEndian, &result)
	c := result % 1000

	// should be greater than zero or make this a uint64
        if c <= 0 {
		result = 100 
		c = 100
        }
	fmt.Println("test ", c)
        return c 

}

func Randint64() (int64, error) {
        var b [8]byte
        if _, err := rand.Read(b[:]); err != nil {
                return 0, err
        }
        var result = int64(binary.LittleEndian.Uint64(b[:])) % 5
        if result == 0 {
                result = 1
        }
        return result, nil
}
