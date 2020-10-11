# This is how we want to name the binary output
BINARY=decoderRandom

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	g++ sha1.c decoderRandomTestHC.cpp -o decoderRandomTestHC_gmp -lgmp
	g++ test_gmp2.cpp -o test_gmp2 -lgmp -lgmpxx
	g++ sha1.c test_gmp3.cpp -o test_gmp3 -lgmp -lgmpxx
	g++ thread.cpp -o thread -pthread 
clean:
	rm -r thread test_gmp2 decoderRandomTestHC_gmp
list:
	echo "all"
