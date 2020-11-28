// To wait for multiple goroutines to finish, we can
// use a *wait group*.

package main

import (
    "fmt"
    "sync"
    "time"
    "math/rand"
)

// This is the function we'll run in every goroutine.
// Note that a WaitGroup must be passed to functions by
// pointer.
func worker(id int, wg *sync.WaitGroup) {
    fmt.Printf("Worker %d starting\n", id)

    rand.Seed(time.Now().UnixNano())
    r := rand.Intn(10)
    

    // Sleep to simulate an expensive task.
    time.Sleep(time.Duration(r) * time.Second)
    fmt.Printf("Worker %d seconds %d done\n", id, r)

    // Notify the WaitGroup that this worker is done.
    wg.Done()
}

func main() {

    // This WaitGroup is used to wait for all the
    // goroutines launched here to finish.
    var wg sync.WaitGroup
    // only add 1 wait group since we are only using the first result for mdencode
    wg.Add(1)

    // Launch several goroutines and only set the WaitGroup count to one for the mdencode modulus scan test
    for i := 1; i <= 6; i++ {
	// wg.Add(1)
        go worker(i, &wg)
    }

    // Block until the WaitGroup counter goes back to 0;
    // all the workers notified they're done.
    wg.Wait()
}

