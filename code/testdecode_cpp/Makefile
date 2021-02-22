# Project name
PROJECT="mdencode test mdzip C++"

# variables
MDZIP=../mdzip_cpp/mdCore
CPPEXT=../mdzip_cpp/external

# c compiler
CC=g++

# C++ Dependencies
DEPS=$(CPPEXT)/cityhash/cityhash.c $(CPPEXT)/crc32/crc32.c $(CPPEXT)/crc64/crc64.c $(CPPEXT)/fasthash/fasthash.cpp
DEPS+=$(CPPEXT)/highwayhash/highwayhash.c $(CPPEXT)/metro64/metrohash64.cpp $(CPPEXT)/pengyhash/pengyhash.c
DEPS+=$(CPPEXT)/RHash-1.4.1/has160.c $(CPPEXT)/RHash-1.4.1/edonr.c
DEPS+=$(CPPEXT)/csiphash/csiphash.c $(CPPEXT)/siphash/siphash.c $(CPPEXT)/seahash/seahash.c $(CPPEXT)/spooky/Spooky.cpp
# DEPS+=$(CPPEXT)/fnv/hash_32.c $(CPPEXT)/fnv/hash_32a.c $(CPPEXT)/fnv/hash_64.c $(CPPEXT)/fnv/hash_64a.c 
DEPS+=$(CPPEXT)/md2/md2.c $(CPPEXT)/md6/md6.h $(CPPEXT)/md6/md6_mode.c $(CPPEXT)/md6/md6_compress.c 

# C++ Libs
LDLIBS=-lssl -lcrypto -lgmpxx -lgmp -lcryptopp -Xlinker -zmuldefs -pthread -std=c++11

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
all:
	@echo "Building Test Decoder HCL"
	$(CC) -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 $(LDLIBS) 
test:
	$(CC) -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 $(LDLIBS)
sync:
	cp $(MDZIP)/mdBlockhash.h mdCore
	cp $(MDZIP)/mdFilehash.h mdCore
	cp $(MDZIP)/mdHashContextList.h mdCore
	cp $(MDZIP)/mdRandom.h mdCore
	cp $(MDZIP)/mdRegisters.h mdCore	
	perl -i -pe "s/external/\.\.\/mdzip_cpp\/external/g" mdCore/mdFilehash.h
testdecode:
	./decoderRandomTestHC2 --mod=64 --threads=32 --block=12 --hl 1-34
testhash:
	$(CC) -I. $(DEPS) tests/testHCL.cpp  -o testhcl $(LDLIBS)
#	$(CC) tests/test_sha1.c -lssl -lcrypto -o test_sha1 -std=c++11
	$(CC) external/highwayhash/highwayhash.c tests/highwayhash_test.c  -o testhw -std=c++11
clean:
	rm -r decoderRandomTestHC2 
cleantest:
	rm test_gmp2 test_sha1 testhw testargs testhcl testcsv testlog testswitch *.log
list:
	echo "all testhash testlog clean test"
