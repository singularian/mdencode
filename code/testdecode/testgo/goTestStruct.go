package main

import (
	"fmt"
//	"time"
//	"math/rand"
        "strconv"
)

type node struct {
    FirstName  string 
    LastName   string
}

// test of multiple object struct initializer
//so this can initialize multiple mdencode modulus can objects in go routines
func main() {

    cities:=[]*node{}
    city:=new(node)
    city.FirstName=strconv.Itoa(0)
    cities=append(cities,city)

for i := 1; i<47 ;i++ {
    city=new(node)
    city.FirstName=strconv.Itoa(i)
    cities=append(cities,city)
}

  fmt.Println("test ", cities[2].FirstName)
  fmt.Println("test ", cities[4].FirstName)
}
