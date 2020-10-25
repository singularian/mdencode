# This is how we want to name the binary output
BINARY=decoderRandom

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X github.com/nsross/mdencode/core.Version=${VERSION} -X github.com/nsross/mdencode/core.BuildTime=${BUILD_TIME}"

all:
	g++ sha1.c decoderRandomTestHC.cpp -o decoderRandomTestHC_gmp -lgmp
test:
	g++ test_gmp2.cpp -o test_gmp2 -lgmp -lgmpxx
# 	g++ sha1.c test_gmp3.cpp -o test_gmp3 -lgmp -lgmpxx
#	g++ sha1.c test_gmp4.cpp -o test_gmp4 -lgmp -lgmpxx -mavx
	g++ sha1.c test_endian.cpp -o test_endian -lgmp -lgmpxx
	g++ thread.cpp -o test_thread -pthread
	g++ test_thread2.cpp -o test_thread2 -pthread
clean:
	rm -r decoderRandomTestHC_gmp test_gmp2 test_endian test_thread
list:
	echo "all test clean"
