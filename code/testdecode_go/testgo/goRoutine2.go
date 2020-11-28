package main

import (
	"fmt"
	"time"
//	"math/rand"
)


func main() {

    c1 := make(chan string)
    // c2 := make(chan string)

    go func() {
        // var number int
        // number = rand.Intn(10)
        time.Sleep(time.Second * 6)
        c1 <- "one"
    }()
    go func() {
        //var number int
        //number2 := rand.Intn(10)
        time.Sleep(time.Second * 10)
        c1 <- "two"
    }()

    for i := 0; i < 2; i++ {
        // Await both of these values
        // simultaneously, printing each one as it arrives.
        select {
        case msg1 := <-c1:
            fmt.Println("received", msg1)
        //case msg2 := <-c2:
        //    fmt.Println("received", msg2)
        } 
    }
}
