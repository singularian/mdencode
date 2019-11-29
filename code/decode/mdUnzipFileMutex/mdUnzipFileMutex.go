package mdUnzipFileMutex 

import (
	"sync"
)

type Nums struct {
    matchCount int
    thread int
}

// FileMutex is a block match mutex
type FileMutex struct {
	mux sync.RWMutex
	// matchcount / threadnumber
	matchList map[Nums][]byte
	lastThread int
	filebuffer []byte
	isMatched bool
	isRunning bool
	matchCount int
}

// Init returns a new FileMutex object  
func Init() (mt *FileMutex) {
        mux := new(FileMutex)
        mux.isMatched   = false
        mux.isRunning   = true
	mux.matchCount  = 0
	mux.matchList   = make(map[Nums][]byte)
	mux.lastThread  = 0
        return mux
}

// GetMatchListKey returns the current value of the buffer for the given key.
func (fm *FileMutex) GetMatchListKey(key int) []byte {
	fm.mux.Lock()
	defer fm.mux.Unlock()
	numbers := Nums{matchCount: fm.matchCount, thread: key}
	return fm.matchList[numbers]
}

// GetLastThread resturn the last match thread or zero if not found
func (fm *FileMutex) GetLastThread() int {
	return fm.lastThread
}

// GetFileBuffer returns the current byte block 
func (fm *FileMutex) GetFileBuffer() []byte {
        fm.mux.Lock()
        defer fm.mux.Unlock()
        return fm.filebuffer
}

// GetMatchStatus Return the current match Status
func (fm *FileMutex) GetMatchStatus() bool {
	return fm.isMatched

}

// GetRunStatus Return the current match Status
func (fm *FileMutex) GetRunStatus() bool {
        return fm.isRunning
}

// Stop sets the File Mutex to stopped
func (fm *FileMutex) Stop() {
	fm.mux.Lock()
	fm.isRunning = false
	fm.mux.Unlock()
}

// SetFileBuffer sets the current block found with the thread
// probably should include the digital signature as well
func (fm *FileMutex) SetFileBuffer(threadNumber int, data []byte) {
	fm.mux.Lock()
	fm.filebuffer   = data
	// add the last match to the matchList Map
	numbers := Nums{matchCount: fm.matchCount, thread: threadNumber}
	fm.matchList[numbers]   = data

	fm.lastThread   = threadNumber + 1
	fm.matchCount++
	fm.isMatched    = true
	fm.mux.Unlock()

}

// ResetMutex
// Reset the match Mutex for the next block
func (fm *FileMutex) ResetMutex() {
	data := []byte{0}
        fm.mux.Lock()
	fm.matchList    = make(map[Nums][]byte)
        fm.isMatched    = false
        fm.isRunning    = true
	fm.filebuffer   = data
	fm.lastThread   = 0
	fm.matchCount   = 0
        fm.mux.Unlock()

}
