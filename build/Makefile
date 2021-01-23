# This is how we want to name the binary output
BINARY=decoderRandom

# Project name
PROJECT="mdencode mdzip testdecode"

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
CPPEXT=../code/testdecode_cpp/external

CC=g++
DEPS=$(CPPEXT)/cityhash/cityhash.c $(CPPEXT)/crc32/crc32.c $(CPPEXT)/crc64/crc64.c $(CPPEXT)/fasthash/fasthash.cpp $(CPPEXT)/highwayhash/highwayhash.c $(CPPEXT)/metro64/metrohash64.cpp $(CPPEXT)/pengyhash/pengyhash.c $(CPPEXT)/seahash/seahash.c $(CPPEXT)/spooky/Spooky.cpp $(CPPEXT)/fnv/hash_32.c $(CPPEXT)/fnv/hash_32a.c $(CPPEXT)/fnv/hash_64.c $(CPPEXT)/fnv/hash_64a.c $(CPPEXT)/md6/md6.h $(CPPEXT)/md6/md6_mode.c $(CPPEXT)/md6/md6_compress.c
# DEPS=$(CPPEXT)/cityhash/cityhash.c $(CPPEXT)/crc32/crc32.c $(CPPEXT)/crc64/crc64.c (CPPEXT)/fasthash/fasthash.cpp $(CPPEXT)/highwayhash/highwayhash.c $(CPPEXT)/metro64/metrohash64.cpp $(CPPEXT)/pengyhash/pengyhash.c $(CPPEXT)/seahash/seahash.c $(CPPEXT)/spooky/Spooky.cpp $(CPPEXT)/fnv/hash_32.c $(CPPEXT)/fnv/hash_32a.c $(CPPEXT)/fnv/hash_64.c $(CPPEXT)/fnv/hash_64a.c $(CPPEXT)/md6/md6.h $(CPPEXT)/md6/md6_mode.c $(CPPEXT)/md6/md6_compress.c $(TESTCPP)/plog/Log.h

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X main.Version=${VERSION} -X main.BuildTime=${BUILD_TIME}"


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
	@echo "Building mdzip GO test files"
	go build $(LDFLAGS) -o decoderRandom ${TESTGO}/${CMD}/decoderRandom.go
	go build $(LDFLAGS) -o decoderRandomTest ${TESTGO}/${CMD}/decoderRandomTest.go
	go build $(LDFLAGS) -o decoderRandomTestHC ${TESTGO}/${CMD}/decoderRandomTestHC.go
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
buildcpp:
	@echo "Building mdzip C++"
	g++ -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/decoderRandomTestHC.cpp -o decoderRandomTestHC2 -lfnv -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11
	g++ -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdZip.cpp  -o mdzipcpp -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11
	g++ -I$(MDZIPCPP) $(DEPS) $(MDZIPCPP)/mdUnzip.cpp  -o mdunzipcpp -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11
mdencode:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
mdzip: 
	@echo "Building mdzip GO"
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip ${MDZIP}/${CMD}/mdzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${MDZIP}/${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist ${MDZIP}/${CMD}/mdlist.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdcmd ${MDZIP}/${CMD}/mdCmd.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdsig ${MDZIP}/${CMD}/mdsig.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile ${MDZIP}/${CMD}/randomFile.go
test:
	@echo "Building mdzip GO decodeRandom Tests"
	go build -o decoderRandom ${TESTGO}/${CMD}/decoderRandom.go
	go build -o decoderRandomTest ${TESTGO}/${CMD}/decoderRandomTest.go
	go build -o decoderRandomTestHC ${TESTGO}/${CMD}/decoderRandomTestHC.go
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
	go build -o decoderRandomTestHC.exe ${TEST}/${CMD}/decoderRandomTestHC.go
# run mdzip test
runtest:
	@echo "Running mdzip Test"
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
hctest:
	@echo "Running Hash Context List Tests"
	./decoderRandomTestHC -all -mod=64 -block=11 -thread=16
clean:
	rm -r mdzip mdunzip mdcmd mdlist mdlistTest mdsig randfile md mdencode mdnewdb mddbreport mdbinlist decoderRandom decoderRandomTest decoderRandomTestHC decoderRandomTestHC2 randomfile randomfile.mdz randomfile.mdz.out *.log
cleanbuild:
	rm -r ${BUILD_DIR}/mdzip ${BUILD_DIR}/mdunzip ${BUILD_DIR}/mdcmd ${BUILD_DIR}/mdlist ${BUILD_DIR}/mdsig ${BUILD_DIR}/randfile ${BUILD_DIR}/mdencode ${BUILD_DIR}/mdbinlist ${BUILD_DIR}/md ${BUILD_DIR}/mddbreport ${BUILD_DIR}/mdnewdb
# Display general help about this command
help:
	@echo "$(PROJECT) Makefile."
	@echo "GOPATH=$(GOPATH)"
	@echo "The following commands are available:"
	@echo "    make all         : Build the mdencode files to the local build directory"
	@echo "    make buid        : Build the mdencode project files"
	@echo "    make mdencode    : Build the mdencode files"
	@echo "    make mdzip       : Build the mdzip files"
	@echo "    make test        : Build the test files"
	@echo "    make runtest     : Run an mdzip/mdunzip test"
	@echo "    make hctest      : Run the hash context list test"
	@echo "    make clean       : Remove any local build artifacts"
	@echo "    make cleanbuild  : Remove any build artifacts"
list:
	echo "all build mdencode mdzip test clean list"
