# This is how we want to name the binary output
BINARY=mdziptest

# Project name
PROJECT="mdencode mdzip test"

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.7
BUILD_TIME=`date +%FT%T%z`

# build directory
BUILD_DIR=${GOBIN}

# command path
CMD=cmd

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X main.Version=${VERSION} -X main.BuildTime=${BUILD_TIME}"

all:
	go build $(LDFLAGS) -o testSignaturelist testSignaturelist.go 
# build: 
#	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${CMD}/mdUnzip.go
test:
	./random.sh 100 1010101 11 64 16
clean:
	rm -r randomfile randomfile.mdz randomfile.mdz.out *.log
