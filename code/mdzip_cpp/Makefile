# This is how we want to name the binary output
BINARY=mdzip

MDZIP=mdzip_cpp
TESTCPP=../testdecode_cpp
CPPEXT=../testdecode_cpp/external

CC=g++
# use the testdecode files so the mdzip_cpp can reuse the same hash signature files and be more modular
# DEPS=external/cityhash/cityhash.c external/fasthash/fasthash.cpp external/highwayhash/highwayhash.c external/metro64/metrohash64.cpp external/pengyhash/pengyhash.c external/seahash/seahash.c external/spooky/Spooky.cpp external/fnv/hash_32.c external/fnv/hash_32a.c external/fnv/hash_64.c external/fnv/hash_64a.c external/md6/md6.h external/md6/md6_mode.c external/md6/md6_compress.c
DEPS=$(CPPEXT)/cityhash/cityhash.c $(CPPEXT)/crc32/crc32.c $(CPPEXT)/crc64/crc64.c $(CPPEXT)/fasthash/fasthash.cpp $(CPPEXT)/highwayhash/highwayhash.c $(CPPEXT)/metro64/metrohash64.cpp $(CPPEXT)/pengyhash/pengyhash.c $(CPPEXT)/seahash/seahash.c $(CPPEXT)/spooky/Spooky.cpp $(CPPEXT)/fnv/hash_32.c $(CPPEXT)/fnv/hash_32a.c $(CPPEXT)/fnv/hash_64.c $(CPPEXT)/fnv/hash_64a.c $(CPPEXT)/md6/md6.h $(CPPEXT)/md6/md6_mode.c $(CPPEXT)/md6/md6_compress.c

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
# you also need to install the libfnv library in the fnv subdirectory
#  it is included with -lfnv
all:
# need to merge these or use lnk
	@echo "Building mdzip cpp"
	g++ -I. $(DEPS) mdZip.cpp  -o mdzip -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11
	@echo ""
	@echo "Building mdunzip cpp"
	g++ -I. $(DEPS) mdUnzip.cpp  -o mdunzip -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11
	@echo ""
	@echo "Building mdzip fasthash64 cpp"
	g++ -I. $(DEPS) mdZipSimple.cpp -o mdzipnh  -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11
	@echo ""
	@echo "Building mdunzip fasthash64 cpp"
	g++ -I. $(DEPS) mdUnzipSimple.cpp -o mdunzipnh  -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11
test:
	@echo "Building test mdzip cpp"
	g++ -I. $(DEPS) tests/testHCL.cpp  -o testhcl -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11
	g++ -I. $(DEPS) tests/testSHA1file.cpp  -o  testSHA1file -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11
	g++ -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 -lfnv -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11

testhash:
	g++ tests/test_sha1.c -lssl -lcrypto -o test_sha1 -std=c++11
	g++ external/highwayhash.c tests/highwayhash_test.c  -o testhw -std=c++11
clean:
	rm -r mdzip mdunzip mdzipnh mdunzipnh *.log *.mdz *.mdsz
cleantest:
	rm decoderRandomTestHC2 test_sha1 testhw testhcl *.log *.mdz *.mdsz
list:
	echo "all test clean test"
