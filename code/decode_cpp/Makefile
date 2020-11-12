# This is how we want to name the binary output
BINARY=decoderRandom

CC=g++
DEPS=external/md6.h external/md6_mode.c external/md6_compress.c external/highwayhash.c
# DEPS=external/highwayhash.c
# CFLAGS=-O2 -I/usr/local/include

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
all:
#	g++ -I. external/highwayhash.c decoderRandomTestHC.cpp -o decoderRandomTestHC -lssl -lcrypto -lgmp -std=c++11 -pthread
#	g++ -I. external/highwayhash.c decoderRandomTestHC.cpp -o decoderRandomTestHC $(OBJ) -lssl -lcrypto -lgmp -std=c++11 -pthread
	g++ -I. $(DEPS) decoderRandomTestHC.cpp -o decoderRandomTestHC_gmp -lssl -lcrypto -lgmp -Xlinker -zmuldefs  -std=c++11 -pthread
	g++ -I. $(DEPS) decoderRandomTestHCthreads.cpp -o decoderRandomTestHCthreads_gmp -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11 -pthread
# opt:
# doesn't work
#	g++ -O3 -I. $(DEPS) decoderRandomTestHCthreads.cpp -o decoderRandomTestHCthreads_gmp -lssl -lcrypto -lgmp -Xlinker -zmuldefs -std=c++11 -pthread
test:
	g++ tests/test_gmp2.cpp -o test_gmp2 -lgmp -lgmpxx
	g++ tests/testArgs.cpp -lssl -lcrypto -o testargs -std=c++11
	g++ tests/testCSVSplit.cpp  -o testcsv -std=c++11
	g++ tests/testSwitch.cpp  -o testswitch -std=c++11
	g++ -I. external/highwayhash.c tests/testHCL.cpp  -o testhcl -lssl -lcrypto -lgmp -std=c++11
testlog:
#	g++ -I. example_logging_to_file.cpp -o testlog -std=c++11
#	g++  easylogging++.cc example_logging_to_file.cpp -o testlog -std=c++11
	g++ -I. external/easylogging++.cc tests/easylogger.cpp -o testlog -std=c++11
testhash:
	g++ tests/test_sha1.c -lssl -lcrypto -o test_sha1 -std=c++11
	g++ external/highwayhash.c tests/highwayhash_test.c  -o testhw -std=c++11
clean:
	rm -r decoderRandomTestHC decoderRandomTestHCthreads_gmp 
cleantest:
	rm test_gmp2 test_sha1 testhw testargs testhcl testcsv testlog testswitch *.log
list:
	echo "all test testlog clean test"
