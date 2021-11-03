# Project name
PROJECT="mdencode mdzip C++"

# variables
MDZIP=mdzip_cpp
CPPEXT=external

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
	@echo "Building mdzip C++"
	$(CC) -I. $(DEPS) mdZip.cpp  -o mdzip $(LDLIBS)
	@echo ""
	@echo "Building mdunzip C++"
	$(CC) -I. $(DEPS) mdUnzip.cpp  -o mdunzip $(LDLIBS)
	@echo ""
	@echo "Building mdzip fasthash64 C++"
	$(CC) -I. $(DEPS) mdZipSimple.cpp -o mdzipnh $(LDLIBS)
	@echo ""
	@echo "Building mdunzip fasthash64 C++"
	$(CC) -I. $(DEPS) mdUnzipSimple.cpp -o mdunzipnh $(LDLIBS)
	@echo ""
	@echo "Building mdzip sip40 C++"
	$(CC) -I. $(DEPS) mdZipSip40.cpp -o mdzipsip40 $(LDLIBS)
	@echo ""
	@echo "Building mdunzip sip40 C++"
	$(CC) -I. $(DEPS) mdUnzipSip40.cpp -o mdunzipsip40 $(LDLIBS)
	@echo ""
	@echo "Building mdzip sip47 bitstream C++ (Currently the Gold One)"	
	$(CC) -I. $(DEPS) mdZipSip47.cpp -o mdzipsip47 $(LDLIBS)
	@echo ""
	@echo "Building mdunzip sip47 bitstream C++"
	$(CC) -I. $(DEPS) mdUnzipSip47.cpp -o mdunzipsip47 $(LDLIBS)	
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
mdzipsip:	
	@echo "Building mdzip sip40 C++"
	$(CC) -I. $(DEPS) mdZipSip40.cpp -o mdzipsip40 $(LDLIBS)
	@echo ""
	@echo "Building mdunzip sip40 C++"
	$(CC) -I. $(DEPS) mdUnzipSip40.cpp -o mdunzipsip40 $(LDLIBS)
mdzipsip47:
	@echo ""
	@echo "Building mdzip sip47 bitstream C++"	
	$(CC) -I. $(DEPS) mdZipSip47.cpp -o mdzipsip47 $(LDLIBS)
	@echo ""
	@echo "Building mdunzip sip47 bitstream C++"
	$(CC) -I. $(DEPS) mdUnzipSip47.cpp -o mdunzipsip47 $(LDLIBS)	
test:
	@echo "Building Test Decoder HCL C++"
	$(CC) -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC2 $(LDLIBS)
testhash:
	$(CC) -I. $(DEPS) tests/testHCL.cpp  -o testhcl $(LDLIBS)
	$(CC) -I. $(DEPS) tests/testFileSignatures.cpp -o  testhashfile $(LDLIBS)
runtestsip:	
	@echo ""
	@echo "Starting the mdzip  C++ sip40"
	@echo "Creating randomfile"
	# cp randomfile randomfile.2
	rm -f randomfile
	# randfile2 -blocknum=100 -bsize=11 -zsize=4 -first=14 -out=randomfile
	randfile2 -blocknum=100 -bsize=11 -zsize=3 -first=1 -out=randomfile
	@echo "Zipping the file randomfile"
	mdzipsip40 --file=randomfile --randbh --fh=11 --inc
	# validating the mdzip file
	@echo ""
	@echo "Validating the randomfile.mdz"
	mdunzipsip40 --file=randomfile.mdz --valmdzip
	@echo ""
	@echo "Running mdlist randomfile.mdz"
	mdunzipsip40 --file=randomfile.mdz --list 
	@echo ""
	@echo "Unzipping randomfile"
	mdunzipsip40 --file=randomfile.mdz --over --val=true
	# @echo "Checking differences"
	# diff randomfile randomfile.mdz.out	
lines:
	wc -l mdCore/*.h *.cpp
clean:
	rm -r mdzip mdunzip mdzipnh mdunzipnh mdzipsip40 mdunzipsip40 mdzipsip47 mdunzipsip47 decoderRandomTestHC2 *.log *.mdz *.mdsz *.out randfile
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
