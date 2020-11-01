# This is how we want to name the binary output
BINARY=decoderRandom

# These are the values we want to pass for Version and BuildTime
VERSION=1.0.0
BUILD_TIME=`date +%FT%T%z`

# decoderRandomTestHCthreads.cpp requires the lib gmp and openssl
all:
	g++ -I. decoderRandomTestHC.cpp -o decoderRandomTestHC -lssl -lcrypto -lgmp -std=c++11 -pthread
	g++ -I. decoderRandomTestHCthreads.cpp -o decoderRandomTestHCthreads_gmp -lssl -lcrypto -lgmp -std=c++11 -pthread
test:
	g++ tests/test_gmp2.cpp -o test_gmp2 -lgmp -lgmpxx
	g++ tests/test_sha1.c -lssl -lcrypto -o test_sha1 -std=c++11
	g++ tests/testArgs.cpp -lssl -lcrypto -o testargs -std=c++11
	g++ tests/testCSVSplit.cpp  -o testcsv -std=c++11
logtest:
#	g++ -I. example_logging_to_file.cpp -o testlog -std=c++11
#	g++  easylogging++.cc example_logging_to_file.cpp -o testlog -std=c++11
	g++ -I. easylogging++.cc tests/easylogger.cpp -o testlog -std=c++11
clean:
	rm -r decoderRandomTestHC decoderRandomTestHCthreads_gmp test_gmp2 test_sha1 testargs testcsv *.log
list:
	echo "all test clean"
