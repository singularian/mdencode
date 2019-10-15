# This is how we want to name the binary output
BINARY=mdzip

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`


BUILD_DIR=${GOBIN}

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	go build -o mdunzip mdUnzip.go 
	go build -o mdzip mdzip.go 
	go build -o mdlist testBinReadMdFmt.go 
build: 
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip mdzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist testBinReadMdFmt.go
clean:
	rm -r mdzip mdunzip mdlist
list:
	echo "all"
