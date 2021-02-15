# Project name
PROJECT="mdencode mdzip C++"

# variables
MDZIP=mdzip_cpp
TESTCPP=../testdecode_cpp
CPPEXT=../testdecode_cpp/external

# compiler
CC=g++
# Dependencies
DEPS=$(CPPEXT)/cityhash/cityhash.c $(CPPEXT)/crc32/crc32.c $(CPPEXT)/crc64/crc64.c $(CPPEXT)/fasthash/fasthash.cpp
DEPS+=$(CPPEXT)/highwayhash/highwayhash.c $(CPPEXT)/metro64/metrohash64.cpp $(CPPEXT)/pengyhash/pengyhash.c
DEPS+=$(CPPEXT)/seahash/seahash.c $(CPPEXT)/spooky/Spooky.cpp
DEPS+=$(CPPEXT)/fnv/hash_32.c $(CPPEXT)/fnv/hash_32a.c $(CPPEXT)/fnv/hash_64.c $(CPPEXT)/fnv/hash_64a.c 
DEPS+=$(CPPEXT)/md6/md6.h $(CPPEXT)/md6/md6_mode.c $(CPPEXT)/md6/md6_compress.c 
# libs
LDLIBS=-lssl -lcrypto -lgmpxx -lgmp -Xlinker -zmuldefs -pthread -std=c++11

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
# you also need to install the libfnv library in the fnv subdirectory
#  it is included with -lfnv
all:
	@echo "Building mdzip C++"
	$(CC) -I.  $(DEPS) mdZip.cpp  -o mdzip $(LDLIBS)
	@echo ""
	@echo "Building mdunzip C++"
	$(CC) -I. $(DEPS) mdUnzip.cpp  -o mdunzip $(LDLIBS)
	@echo ""
	@echo "Building mdzip fasthash64 C++"
	$(CC) -I. $(DEPS) mdZipSimple.cpp -o mdzipnh $(LDLIBS)
	@echo ""
	@echo "Building mdunzip fasthash64 C++"
	$(CC) -I. $(DEPS) mdUnzipSimple.cpp -o mdunzipnh $(LDLIBS)
mdzip:
	@echo "Building mdzip C++"
	$(CC) -I. $(DEPS) mdZip.cpp  -o mdzip $(LDLIBS)
	@echo ""
	@echo "Building mdunzip C++"
	$(CC) -I. $(DEPS) mdUnzip.cpp  -o mdunzip $(LDLIBS)
mdzipsimple:	
	@echo "Building mdzip fasthash64 C++"
	$(CC) -I. $(DEPS) mdZipSimple.cpp -o mdzipnh $(LDLIBS)
	@echo ""
	@echo "Building mdunzip fasthash64 C++"
	$(CC) -I. $(DEPS) mdUnzipSimple.cpp -o mdunzipnh $(LDLIBS)
test:
	@echo "Building Test Decoder HCL C++"
	$(CC) -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 $(LDLIBS)
testhash:
	$(CC) -I. $(DEPS) tests/testHCL.cpp  -o testhcl $(LDLIBS)
	$(CC) -I. $(DEPS) tests/testFileSignatures.cpp -o  testhashfile $(LDLIBS)
lines:
	wc -l mdCore/*.h *.cpp
clean:
	rm -r mdzip mdunzip mdzipnh mdunzipnh decoderRandomTestHC2 *.log *.mdz *.mdsz *.out
cleantest:
	rm decoderRandomTestHC2 testhashfile testhcl *.log *.mdz *.mdsz *.out
# Display general help about this command
help:
	@echo "$(PROJECT) Makefile."
	@echo "GOPATH=$(GOPATH)"
	@echo "The following commands are available:"
	@echo "    make all         : Build the C++ mdzip/mdunzip program"
	@echo "    make mdzip       : Build the C++ mdzip/mdunzip program"
	@echo "    make mdzipsimple : Build the C++ mdzip/mdunzip program"
	@echo "    make test        : Build the test files"
	@echo "    make testhash    : Run the hash context list test"
	@echo "    make clean       : Remove any local build artifacts"
	@echo "    make cleantest   : Remove any build artifacts"
list:
	echo "all test testhash clean cleantest"
