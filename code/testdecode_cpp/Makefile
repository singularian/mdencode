# This is how we want to name the binary output
BINARY=decoderRandom

CPPEXT=external

CC=g++
DEPS=external/cityhash/cityhash.c external/crc32/crc32.c external/crc64/crc64.c external/fasthash/fasthash.cpp external/highwayhash/highwayhash.c external/metro64/metrohash64.cpp external/pengyhash/pengyhash.c external/seahash/seahash.c external/spooky/Spooky.cpp external/fnv/hash_32.c external/fnv/hash_32a.c external/fnv/hash_64.c external/fnv/hash_64a.c external/md6/md6.h external/md6/md6_mode.c external/md6/md6_compress.c
# DEPS=external/highwayhash.c
# CFLAGS=-O2 -I/usr/local/include

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
# you also need to install the libfnv library in the fnv subdirectory
#  it is included with -lfnv
all:
	@echo "Building Test Decoder HCL"
	g++ -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 -lfnv -lssl -lcrypto -lgmpxx -lgmp -Xlinker -zmuldefs -pthread -std=c++11 
test:
	g++ -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 -lfnv -lssl -lcrypto -lgmpxx  -lgmp -Xlinker -zmuldefs -pthread -std=c++11
testdecode:
	./decoderRandomTestHC2 --mod=64 --threads=32 --block=12 --hl 1-34
testhash:
	g++ -I. $(DEPS) tests/testHCL.cpp  -o testhcl -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11
#	g++ tests/test_sha1.c -lssl -lcrypto -o test_sha1 -std=c++11
	g++ external/highwayhash/highwayhash.c tests/highwayhash_test.c  -o testhw -std=c++11
clean:
	rm -r decoderRandomTestHC2 
cleantest:
	rm test_gmp2 test_sha1 testhw testargs testhcl testcsv testlog testswitch *.log
list:
	echo "all testhash testlog clean test"
