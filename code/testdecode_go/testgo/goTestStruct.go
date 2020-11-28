package main

import (
	"fmt"
//	"time"
//	"math/rand"
        "strconv"
)

type node struct {
    Name  string 
    Country   string
}

// test of multiple object struct initializer
// this test can initialize multiple mdencode modulus objects in go routines
func main() {

    cities:=[]*node{}
    city:=new(node)
    city.Name=strconv.Itoa(0)
    cities=append(cities,city)

for i := 1; i<40 ;i++ {
    city=new(node)
    city.Name=strconv.Itoa(i)
    cities=append(cities,city)
}

  fmt.Println("test ", cities[2].Name)
  fmt.Println("test ", cities[4].Name)
}
