# This is how we want to name the binary output
BINARY=decoderRandom

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
all:
	g++ decoderRandomTestHC.cpp -o decoderRandomTestHC -lssl -lcrypto -lgmp -std=c++11 -pthread
	g++ decoderRandomTestHCthreads.cpp -o decoderRandomTestHCthreads_gmp -lssl -lcrypto -lgmp -std=c++11 -pthread
test:
	g++ test_gmp2.cpp -o test_gmp2 -lgmp -lgmpxx
	g++ test_sha1.c -lssl -lcrypto -o test_sha1
	g++ testArgs.cpp -lssl -lcrypto -o testargs -std=c++11
clean:
	rm -r decoderRandomTestHC decoderRandomTestHCthreads_gmp test_gmp2 test_sha1 testargs
list:
	echo "all test clean"
