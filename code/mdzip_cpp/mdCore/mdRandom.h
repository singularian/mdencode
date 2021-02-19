#include <algorithm>
#include <array>
#include <iostream>
#include <random>
#include <functional> // ref()
#include <climits>

using namespace std;

// generate a random int
uint32_t randInt()
{  
   
    random_device rd;
    uint32_t randomnum = 0;

    uniform_int_distribution<unsigned long long> dist2(0, ULONG_MAX);
    mt19937 engine1(rd());
    randomnum = dist2(engine1);
    return randomnum;
}

// generate a random long 
uint64_t randLong()
{

    random_device rd;
    uint64_t randomnum = 0;

    uniform_int_distribution<unsigned long long> dist2(0, ULLONG_MAX);
    mt19937 engine1(rd());
    randomnum = dist2(engine1);
    return randomnum;
}

// randomize a char array
int genRandomByteBlock(char *stream, int num_bytes) {
    srand(time(0));

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = (rand() % 255);
    }

    return 0;
}


// randomize an uint8_t array
int genRandomUnsignedByteBlock(unsigned char *stream, int num_bytes) {
    srand(time(0));

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = (rand() % 255);
    }

    return 0;
}


// randomize an uint8_t array
int genRandomUnsignedByteBlock2(uint8_t *stream, int num_bytes) {
    srand(time(0));

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = (rand() % 255);
    }

    return 0;
}

// randomize a uint64_t 64 bit array
int genRandomLongBlock(uint64_t *stream, int num_bytes) {

    for (int f = 0; f < num_bytes; f++) {
       stream[f] = randLong();
    }

    return 0;
}

