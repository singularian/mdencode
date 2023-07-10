# Project name
PROJECT="mdencode mdzip project"

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.8
BUILD_TIME=`date +%FT%T%z`

# build directory
BUILD_DIR=${GOBIN}

# command path
CMD=cmd
MDE=../code/mdencode
MDZ=../code/mdzip_go
MDZIP=../code/mdzip_go
TESTGO=../code/testdecode_go
# mdzip cpp
MDZIPCPP=../code/mdzip_cpp
TESTCPP=../code/testdecode_cpp
CPPEXT=../code/mdzip_cpp/external

# c compiler
# CC=g++ -g 
CC=g++

# C++ Dependencies
DEPS=$(CPPEXT)/cityhash/cityhash.c $(CPPEXT)/crc32/crc32.c $(CPPEXT)/crc64/crc64.c $(CPPEXT)/fasthash/fasthash.cpp
DEPS+=$(CPPEXT)/highwayhash/highwayhash.c $(CPPEXT)/metro64/metrohash64.cpp $(CPPEXT)/pengyhash/pengyhash.c
DEPS+=$(CPPEXT)/RHash-1.4.1/has160.c $(CPPEXT)/RHash-1.4.1/edonr.c
DEPS+=$(CPPEXT)/csiphash/csiphash.c $(CPPEXT)/siphash/siphash.c $(CPPEXT)/seahash/seahash.c $(CPPEXT)/spooky/Spooky.cpp
# DEPS+=$(CPPEXT)/fnv/hash_32.c $(CPPEXT)/fnv/hash_32a.c $(CPPEXT)/fnv/hash_64.c $(CPPEXT)/fnv/hash_64a.c 
DEPS+=$(CPPEXT)/md2/md2.c $(CPPEXT)/md6/md6.h $(CPPEXT)/md6/md6_mode.c $(CPPEXT)/md6/md6_compress.c 

# C++ libs
LDLIBS=-lssl -lcrypto -lgmpxx -lgmp -lcryptopp -Xlinker -zmuldefs -pthread -std=c++11

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X main.Version=${VERSION} -X main.BuildTime=${BUILD_TIME}"

# build all
all:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
	@echo "Building mdzip GO"
	go build $(LDFLAGS) -o mdzip ${MDZIP}/${CMD}/mdzip.go 
	go build $(LDFLAGS) -o mdunzip ${MDZIP}/${CMD}/mdUnzip.go 
	go build $(LDFLAGS) -o mdlist ${MDZIP}/${CMD}/mdlist.go 
	go build $(LDFLAGS) -o mdcmd ${MDZIP}/${CMD}/mdCmd.go 
	go build $(LDFLAGS) -o mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go 
	go build $(LDFLAGS) -o mdsig ${MDZIP}/${CMD}/mdsig.go 
	go build $(LDFLAGS) -o randfile ${MDZIP}/${CMD}/randomFile.go
	go build $(LDFLAGS) -o randfile2 ${MDZIP}/${CMD}/randomFile2.go
	@echo "Building mdzip GO test files"
	go build $(LDFLAGS) -o mdtestgo ${TESTGO}/${CMD}/mdTestHC.go
# build mdzip C++
	make buildcpp
# build the mdencode GO files	
build:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
	@echo "Building mdzip GO"
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip ${MDZIP}/${CMD}/mdzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${MDZIP}/${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist ${MDZIP}/${CMD}/mdlist.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdcmd ${MDZIP}/${CMD}/mdCmd.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdsig ${MDZIP}/${CMD}/mdsig.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile ${MDZIP}/${CMD}/randomFile.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile2 ${MDZIP}/${CMD}/randomFile2.go
# build mdzip C++
buildcpp:
	@echo "Building mdzip C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdZip.cpp  -o mdzipcpp $(LDLIBS)
	@echo ""
	@echo "Building mdunzip C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdUnzip.cpp  -o mdunzipcpp $(LDLIBS)
	@echo ""
	@echo "Building mdzip fasthash64 C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdZipSimple.cpp -o mdzipnh  $(LDLIBS)
	@echo ""
	@echo "Building mdunzip fasthash64 C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdUnzipSimple.cpp -o mdunzipnh  $(LDLIBS)
	@echo ""
	@echo "Building mdzip sip40 C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdZipSip40.cpp  -o mdzipsip40 $(LDLIBS)
	@echo ""
	@echo "Building mdunzip sip40 C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdUnzipSip40.cpp  -o mdunzipsip40 $(LDLIBS)	
	@echo ""
	@echo "Building mdzip sip47 C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdZipSip47.cpp  -o mdzipsip47 $(LDLIBS)
	@echo ""
	@echo "Building mdunzip sip47 C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdUnzipSip47.cpp  -o mdunzipsip47 $(LDLIBS)	
	@echo ""
	@echo "Building Test Decode C++"
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdTest.cpp -o mdtest $(LDLIBS)
# build mdencode GO
mdencode:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
# build mdzip GO	
mdzip: 
	@echo "Building mdzip GO"
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip ${MDZIP}/${CMD}/mdzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${MDZIP}/${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist ${MDZIP}/${CMD}/mdlist.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdcmd ${MDZIP}/${CMD}/mdCmd.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdsig ${MDZIP}/${CMD}/mdsig.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile ${MDZIP}/${CMD}/randomFile.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile2 ${MDZIP}/${CMD}/randomFile2.go
# build the mdzip test GO
test:
	@echo "Building mdzip GO decodeRandom Tests"
	go build -o mdtestgo ${TESTGO}/${CMD}/mdTestHC.go
testcpp:
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/tests/testHCL.cpp -o testhcl  $(LDLIBS)
	$(CC) -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/tests/testFileSignatures.cpp -o testfs  $(LDLIBS)
# 	build mdzip c++
mdzipcpp: buildcpp
#	make buildcpp	
# windows build
windows:
	@echo "Windows Build"
	@echo "Building mdencode"
	go build -o md.exe ${MDE}/${CMD}/mdencode.go
	go build -o mdencode.exe ${MDE}/${CMD}/mdencode.go
	go build -o mdnewdb.exe ${MDE}/${CMD}/mdnewdb.go
	go build -o mddbreport.exe ${MDE}/${CMD}/mdencodeReport.go
	go build -o mdbinlist.exe ${MDE}/${CMD}/testBinReadMdFmt.go
	@echo "Building mdzip"
	go build -o mdzip.exe ${MDZIP}/${CMD}/mdzip.go
	go build -o mdunzip.exe ${MDZIP}/${CMD}/mdUnzip.go
	go build -o mdlist.exe ${MDZIP}/${CMD}/mdlist.go
	go build -o mdcmd.exe ${MDZIP}/${CMD}/mdCmd.go
	go build -o mdlistTest.exe ${MDZIP}/${CMD}/testBinReadMdFmt.go
	go build -o mdsig.exe ${MDZIP}/${CMD}/mdsig.go
	go build -o randfile.exe ${MDZIP}/${CMD}/randomFile.go
	@echo "Building mdzip decodeRandom Tests"
	go build -o mdtestgo.exe ${TEST}/${CMD}/mdTestHC.go
# run mdzip go test
runtest:
	@echo "Running mdzip GO Test"
	@echo ""
	@echo "Creating randomfile"
	head -c 100 /dev/urandom > randomfile
	@echo ""
	@echo "Zipping the file randomfile"
	mdzip -file=randomfile -bh=111100111 -fh=000111 -mod=64 -block=11 -out=randomfile.mdz
	@echo ""
	@echo "Running mdlist randomfile.mdz"
	mdlist randomfile.mdz
	@echo ""
	@echo "Unzipping randomfile"
	mdunzip -file=randomfile.mdz -out=randomfile.mdz.out -thread=16 -val=true
# run the Go Hash Context List Tests	
hctest:
	@echo "Running GO Hash Context List Tests"
	./mdtestgo -all -mod=64 -block=11 -thread=16	
# run the C++ mdzip test
runtestcpp:
	@echo "Running mdzip C++ Test"
	@echo ""
	@echo "Creating randomfile"
	head -c 100 /dev/urandom > randomfile
	@echo ""
	@echo "Zipping the file randomfile"
	# mdzipcpp --file=randomfile --fh=4 --bh=30 --mod=64 --block=11 --randbh
	mdzipcpp --file=randomfile --fh=4 --bh=30 --mod=64 --block=11 --randbh --dec
	# mdzipcpp --file=randomfile --fh=4 --bh=30 --mod=64 --block=11 --randbh --inc
	@echo ""
	@echo "Running mdlist randomfile.mdz"
	mdunzipcpp --file=randomfile.mdz --val=true --list 
	@echo ""
	@echo "Unzipping randomfile"
	mdunzipcpp --file=randomfile.mdz --val=true	--over
# test the mdzip C++ no header fasthash64 encoder 
# needs more cpu threads to finish	
runtestcpp2:	
	@echo ""
	@echo "Starting the mdzip  C++ no header test"
	@echo "Creating randomfile"
	rm -f randomfile
	# head -c 100 /dev/urandom > randomfile
	# randfile2 -size=1 -out=randomfile - old args
	# originally I was using this byte block with padded zero bytes improve the test time 
	# bytes 1 - 5 as zero
	# byte six set at dec 20 hex 14 
	# bytes 7 - 14 random
	# Example Block Bytes         00 00 00 00 00 14 04 6E 1B 0A 1E AA F3 0F
	randfile2 -blocknum=10 -bsize=14 -zsize=6 -first=14 -out=randomfile
	# randfile2 -blocknum=17 -bsize=14 -zsize=6 -first=14 -out=randomfile
	@echo "Zipping the file randomfile"
	mdzipnh --file=randomfile --rand
	@echo ""
	@echo "Running mdlist randomfile.mdz"
	mdunzipnh --file=randomfile.mdsz --list 
	@echo ""
	@echo "Unzipping randomfile"
	mdunzipnh --file=randomfile.mdsz --over
	@echo "Checking differences"
	diff randomfile randomfile.mdsz.out
# test the mdzip C++ sip40 non incrementing key
runtestsip:	
	@echo ""
	@echo "Starting the mdzip  C++ sip40 non incrementing key"
	@echo "Creating randomfile"
	# cp randomfile randomfile.2
	rm -f randomfile
	# randfile2 -blocknum=100 -bsize=11 -zsize=4 -first=14 -out=randomfile
	# this test uses a random block in this format 
	# it could also use a full non padded block 
	# 0   0   1   127 231 57  115 27  53  37  74
	# 00  00  01  7F  E7  39  73  1B  35  25  4A
	randfile2 -blocknum=10 -bsize=11 -zsize=3 -first=1 -out=randomfile
	@echo "Zipping the file randomfile"
	mdzipsip40 --file=randomfile --randbh --fh=11
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
# test the mdzip C++ sip40 decrement key
runtestsipdec:	
	@echo ""
	@echo "Starting the mdzip  C++ sip40 decrement key"
	@echo "Creating randomfile"
	# cp randomfile randomfile.2
	rm -f randomfile
	# randfile2 -blocknum=100 -bsize=11 -zsize=4 -first=14 -out=randomfile
	randfile2 -blocknum=10 -bsize=11 -zsize=3 -first=1 -out=randomfile
	@echo "Zipping the file randomfile"
	mdzipsip40 --file=randomfile --randbh --fh=11 --dec
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
# test the mdzip C++ sip40 increment key	
runtestsipinc:	
	@echo ""
	@echo "Starting the mdzip C++ sip40 increment key"
	@echo "Creating randomfile"
	# cp randomfile randomfile.2
	rm -f randomfile
	# randfile2 -blocknum=100 -bsize=11 -zsize=4 -first=14 -out=randomfile
	randfile2 -blocknum=10 -bsize=11 -zsize=3 -first=1 -out=randomfile
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
# test the mdzip C++ sip47 non incrementing key
# this uses a 7 bit bitstream to encode the modulus scan exponent block
# this means it can save 1 to 5 bits per 80 bit 10 byte block
# it can also use RLE to encode the modulus scan exponent block
runtestsip47:	
	@echo ""
	@echo "Starting the mdzip C++ sip47 modulus bitstream test"
	@echo "Creating randomfile"
	# cp randomfile randomfile.2
	rm -f randomfile
	# randfile2 -blocknum=10 -bsize=10 -zsize=2 -first=14 -out=randomfile
	# this test uses a random block in this format 
	# it could also use a full non padded block 
	# 0   1   127 231 57  115 27  53  37  74
	# 00  14  7F  E7  39  73  1B  35  25  4A
	# out to add a last block param
	# 900 blocks is about compressive for 7 bit encoding
	randfile2 -blocknum=10 -bsize=10 -zsize=2 -first=1 -out=randomfile
	@echo ""
	@echo "Zipping the file randomfile"
	# mdzipsip47 --file=randomfile --randbh --fh=11 --inc
	mdzipsip47 --file=randomfile --randbh --fh=11
	# validating the mdzip file
	@echo ""
	@echo "Validating the randomfile.mdz"
	mdunzipsip47 --file=randomfile.mdz --valmdzip
	@echo ""
	@echo "Running mdlist randomfile.mdz"
	mdunzipsip47 --file=randomfile.mdz --list 
	@echo ""
	@echo "Unzipping randomfile"
	mdunzipsip47 --file=randomfile.mdz --over --val=true
	@echo "Checking differences"
	diff randomfile randomfile.mdz.out			
# run the C++ Hash Context List Tests	
hctestcpp:
	@echo "Running C++ Hash Context List Tests"
	mdtest --mod=64 --block=11 --bh=6-8,23,33,34 --hl 1 2 3 4-8 --randbh
# clean the build directory	
clean:
	rm -r mdzip mdunzip mdzipcpp mdunzipcpp mdzipnh mdunzipnh mdzipsip47 mdzipsip47 mdtest 
	rm -r mdcmd mdlist mdlistTest mdsig randfile randfile2 md mdencode mdnewdb mddbreport mdbinlist decoderRandom decoderRandomTest decoderRandomTestHC 
#	rm -r randomfile randomfile.mdz randomfile.mdz.out *.mdsz *.mdsz.out *.log
# clean the build directory
cleanbuild:
	rm -r ${BUILD_DIR}/mdzip ${BUILD_DIR}/mdunzip ${BUILD_DIR}/mdcmd ${BUILD_DIR}/mdlist ${BUILD_DIR}/mdsig ${BUILD_DIR}/randfile ${BUILD_DIR}/mdencode ${BUILD_DIR}/mdbinlist ${BUILD_DIR}/md ${BUILD_DIR}/mddbreport ${BUILD_DIR}/mdnewdb
# Display general help about this command
help:
	@echo "$(PROJECT) Makefile."
	@echo "GOPATH=$(GOPATH)"
	@echo "The following commands are available:"
	@echo "    make all           : Build the mdencode files to the local build directory"
	@echo "    make buid          : Build the mdencode project files"
	@echo "    make buidcpp       : Build the mdencode mdzip C++ project files"
	@echo "    make mdencode      : Build the mdencode files"
	@echo "    make mdzip         : Build the mdzip GO files"
	@echo "    make test          : Build the test files"
	@echo "    make runtest       : Run the GO mdzip/mdunzip test"
	@echo "    make hctest        : Run the GO Hash Context List test"
	@echo "    make runtestcpp    : Run the C++ mdzip/mdunzip test"
	@echo "    make runtestcpp2   : Run the C++ mdzip/mdunzip fasthash test"
	@echo "    make runtestsip    : Run the Siphash 40 C++ mdzip/mdunzip fasthash 64 test"
	@echo "    make runtestsipdec : Run the Siphash 40 C++ decrement key mdzip/mdunzip test"
	@echo "    make runtestsipinc : Run the Siphash 40 C++ increment key mdzip/mdunzip test"
	@echo "    make hctestcpp     : Run the C++ Hash Context List test"
	@echo "    make clean         : Remove any local build artifacts"
	@echo "    make cleanbuild    : Remove any build artifacts"
list:
	@echo "all build mdencode mdzip test clean list"
