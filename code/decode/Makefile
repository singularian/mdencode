# This is how we want to name the binary output
BINARY=decoderRandom

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	go build -o decoderRandom decoderRandom.go
	go build -o decoderRandom4 decoderRandom4.go 
	go build -o mdunzip mdunzip.go 
	go build -o mdzip mdzip.go 
	go build -o mdlist testBinReadMdFmt.go 
clean:
	rm -r decoderRandom decoderRandom4 mdzip mdunzip mdlist
list:
	echo "all"
