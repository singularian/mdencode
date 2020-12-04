# This is how we want to name the binary output
BINARY=decoderRandom

CC=g++
DEPS=external/cityhash/cityhash.c external/fasthash/fasthash.cpp external/highwayhash/highwayhash.c external/metro64/metrohash64.cpp external/pengyhash/pengyhash.c external/seahash/seahash.c external/spooky/Spooky.cpp external/fnv/hash_32.c external/fnv/hash_32a.c external/fnv/hash_64.c external/fnv/hash_64a.c external/md6/md6.h external/md6/md6_mode.c external/md6/md6_compress.c
# DEPS=external/highwayhash.c
# CFLAGS=-O2 -I/usr/local/include

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
# you also need to install the libfnv library in the fnv subdirectory
#  it is included with -lfnv
all:
#	g++ -I. external/highwayhash.c decoderRandomTestHC.cpp -o decoderRandomTestHC -lssl -lcrypto -lgmp -pthread -std=c++11
#	g++ -I. external/highwayhash.c decoderRandomTestHC.cpp -o decoderRandomTestHC $(OBJ) -lssl -lcrypto -lgmp -pthread -std=c++11
	g++ -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC1 -lfnv -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11 
	g++ -I. $(DEPS) decoderRandomTestHCthreads.cpp -o decoderRandomTestHC2 -lfnv -lssl -lcrypto -lgmp -Xlinker -zmuldefs -pthread -std=c++11
# opt:
# doesn't work
#	g++ -O3 -I. $(DEPS) decoderRandomTestHCthreads.cpp -o decoderRandomTestHCthreads_gmp -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11 -pthread
sync:
	cp decoderRandomTestHCthreads.cpp decoderRandomTestHC.cpp
test:
	g++ tests/test_gmp2.cpp -o test_gmp2 -lgmp -lgmpxx
	g++ tests/testArgs.cpp -lssl -lcrypto -o testargs -std=c++11
	g++ tests/testCSVSplit.cpp  -o testcsv -std=c++11
	g++ -I. $(DEPS) tests/testHCL.cpp  -o testhcl -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11
testlog:
#	g++ -I. example_logging_to_file.cpp -o testlog -std=c++11
#	g++  easylogging++.cc example_logging_to_file.cpp -o testlog -std=c++11
	g++ -I. external/easylogging++.cc tests/easylogger.cpp -o testlog -std=c++11
#	g++ tests/testThreadLocal.cpp -o test -pthread -std=c++11
testhash:
	g++ tests/test_sha1.c -lssl -lcrypto -o test_sha1 -std=c++11
	g++ external/highwayhash.c tests/highwayhash_test.c  -o testhw -std=c++11
clean:
	rm -r decoderRandomTestHC1 decoderRandomTestHC2 
cleantest:
	rm test_gmp2 test_sha1 testhw testargs testhcl testcsv testlog testswitch *.log
list:
	echo "all test testlog clean test"
