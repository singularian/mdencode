package mdUnzipFileMutex 

import (
//	"fmt"
	"sync"
//	"time"
)

type Nums struct {
    matchCount int
    thread int
}

// FileMutex is safe to use concurrently.
type FileMutex struct {
	mux sync.RWMutex
	// matchcount / threadnumber
	items map[Nums][]byte
	filebuffer []byte
	isMatched bool
	matchCount int
}

// Init returns a new FileMutex object  
func Init() (mt *FileMutex) {
        mux := new(FileMutex)
        mux.isMatched   = false
	mux.matchCount  = 0
	mux.items = make(map[Nums][]byte)
	// mux.mux         = new(sync.RWMutex)
        return mux
}

// Inc increments the counter for the given key.
/* func (c *FileMutex) Inc(key string) {
	c.mux.Lock()
	// Lock so only one goroutine at a time can access the map c.v.
	// c.v[key]++
	c.mux.Unlock()
} */

// Value returns the current value of the buffer for the given key.
func (c *FileMutex) Value(key int) []byte {
	c.mux.Lock()
	// Lock so only one goroutine at a time can access the map 
	defer c.mux.Unlock()
	numbers := Nums{matchCount: c.matchCount, thread: key}
	return c.items[numbers]
}

// Value returns the current value of the file block buffer
func (c *FileMutex) GetFileBuffer() []byte {
        c.mux.Lock()
        defer c.mux.Unlock()
        return c.filebuffer
}

func (c *FileMutex) GetMatchStatus() bool {
	return c.isMatched

}

func (c *FileMutex) SetFileBuffer(threadNumber int, data []byte) {
	c.mux.Lock()
	// c.items[key]   = data
	c.filebuffer   = data
	numbers := Nums{matchCount: c.matchCount, thread: threadNumber}
	c.items[numbers]   = data
	c.matchCount++
	c.isMatched    = true
	c.mux.Unlock()

}

func (c *FileMutex) ResetFileBuffer(size int, data []byte) {
        c.mux.Lock()
	c.items        = make(map[Nums][]byte)
        c.isMatched    = false
	c.filebuffer   = data 
	c.matchCount   = 0
        c.mux.Unlock()

}
