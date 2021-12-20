# This is how we want to name the binary output
BINARY=decoderRandom

# Project name
PROJECT="mdencode mdzip testdecode"

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.6
BUILD_TIME=`date +%FT%T%z`

# command path
CMD=cmd

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	go build -o mdtestgo ${CMD}/mdTest.go 
	go build -o mdtest ${CMD}/mdTestHC.go 
clean:
	rm -r mdtestgo mdtest *.log
# Display general help about this command
help:
	@echo "$(PROJECT) Makefile."
	@echo "GOPATH=$(GOPATH)"
	@echo "The following commands are available:"
	@echo "    make all         : Build the mdencode files"
	@echo "    make clean       : Remove any build artifact"
list:
	echo "all clean list"
