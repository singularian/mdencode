# This is how we want to name the binary output
BINARY=decoderRandom

# Project name
PROJECT="mdencode mdzip testdecode"

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.7
BUILD_TIME=`date +%FT%T%z`

# build directory
BUILD_DIR=${GOBIN}

# command path
CMD=cmd
MDE=../code/mdencode
MDZ=../code/mdzip
MDZIP=../code/decode
TEST=../code/testdecode

# Setup the -ldflags option for go build here, interpolate the variable values
LDFLAGS=-ldflags "-X main.Version=${VERSION} -X main.BuildTime=${BUILD_TIME}"


all:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
	@echo "Building mdzip"
	go build $(LDFLAGS) -o mdzip ${MDZIP}/${CMD}/mdUnzip.go 
	go build $(LDFLAGS) -o mdunzip ${MDZIP}/${CMD}/mdUnzip.go 
	go build $(LDFLAGS) -o mdlist ${MDZIP}/${CMD}/mdlist.go 
	go build $(LDFLAGS) -o mdcmd ${MDZIP}/${CMD}/mdCmd.go 
	go build $(LDFLAGS) -o mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go 
	go build $(LDFLAGS) -o mdsig ${MDZIP}/${CMD}/mdsig.go 
	go build $(LDFLAGS) -o randfile ${MDZIP}/${CMD}/randomFile.go
	@echo "Building mdzip test files"
	go build $(LDFLAGS) -o decoderRandom ${TEST}/${CMD}/decoderRandom.go
	go build $(LDFLAGS) -o decoderRandomTest ${TEST}/${CMD}/decoderRandomTest.go
	go build $(LDFLAGS) -o decoderRandomTestHC ${TEST}/${CMD}/decoderRandomTestHC.go
build:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
	@echo "Building mdzip"
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip ${MDZIP}/${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${MDZIP}/${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist ${MDZIP}/${CMD}/mdlist.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdcmd ${MDZIP}/${CMD}/mdCmd.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdsig ${MDZIP}/${CMD}/mdsig.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile ${MDZIP}/${CMD}/randomFile.go
mdencode:
	@echo "Building mdencode"
	go build $(LDFLAGS) -o ${BUILD_DIR}/md ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdencode ${MDE}/${CMD}/mdencode.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdnewdb ${MDE}/${CMD}/mdnewdb.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mddbreport ${MDE}/${CMD}/mdencodeReport.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdbinlist ${MDE}/${CMD}/testBinReadMdFmt.go
mdzip: 
	@echo "Building mdzip"
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdunzip ${MDZIP}/${CMD}/mdUnzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdzip ${MDZIP}/${CMD}/mdzip.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlist ${MDZIP}/${CMD}/mdlist.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdcmd ${MDZIP}/${CMD}/mdCmd.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdlistTest ${MDZIP}/${CMD}/testBinReadMdFmt.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/mdsig ${MDZIP}/${CMD}/mdsig.go
	go build $(LDFLAGS) -o ${BUILD_DIR}/randfile ${MDZIP}/${CMD}/randomFile.go
test:
	@echo "Building mdzip decodeRandom Tests"
	go build -o decoderRandom ${TEST}/${CMD}/decoderRandom.go
	go build -o decoderRandomTest ${TEST}/${CMD}/decoderRandomTest.go 
	go build -o decoderRandomTestHC ${TEST}/${CMD}/decoderRandomTestHC.go 
clean:
	rm -r mdzip mdunzip mdcmd mdlist mdlistTest mdsig randfile md mdencode mdnewdb mddbreport mdbinlist decoderRandom decoderRandomTest decoderRandomTestHC *.log
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
	@echo "    make clean       : Remove any local build artifacts"
	@echo "    make cleanbuild  : Remove any build artifacts"
list:
	echo "all build mdencode mdzip test clean list"
