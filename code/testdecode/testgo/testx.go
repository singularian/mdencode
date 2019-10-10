package main

import (
	"fmt"
	"sync"
)

var wg sync.WaitGroup

func factorial(x int) int {
	if x > 0 {
		return x * factorial(x-1)
	}
	return 1
}

func fact_worker(fact_resp_chan chan string, x int) {
	defer wg.Done()
	response := factorial(x)
	fact_resp_chan <- fmt.Sprintf("Factorial of %d is : %d", x, response)
}

func main() {
	fact_chan := make(chan string)

	for n := 0; n <= 5; n++ {
		wg.Add(1)
		go fact_worker(fact_chan, n)
	}

	go func() {
		wg.Wait()
		close(fact_chan)
	}()

	for resp := range fact_chan {
		fmt.Printf("%s\n", resp)
	}
}
