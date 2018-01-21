# This is how we want to name the binary output
BINARY=testHash

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	go build -o decoder4 decoder4.go
	go build -o decoder5 decoder5.go
	go build -o decoderRandom decoderRandom.go
clean:
	rm -r decoderRandom decoder4 decoder5
list:
	echo "all"
