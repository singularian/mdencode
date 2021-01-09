#include <cstring>
#include <gmp.h>
#include <sys/stat.h>


// pad a gmp bigint export byteblock
// if the count is less than the blocksize move the bytes over by the difference
// between the diff of the blocksize and count zero out the other bytes
// 
// gmp export block 80 00 00 00 00 00 00 00 
//     padded block 00 00 00 00 00 00 00 80 - shifted over 8
int padBlockBytes(size_t count, int blocksize, unsigned char* byteblock) {
   int n;
   int diff;

   if (count < blocksize) {
       diff = blocksize - count;
       for (n = (blocksize - diff); n >= 0; n--) byteblock[n+diff] = byteblock[n];
       for (n = 0; n < diff; n++) byteblock[n] = 0;
   }

   return 0;
}


// converts the modulus bit size to byte size
int calcModulusBytes (int modsize) {

    int modsizeBytes = modsize;
    if ((modsize % 8) == 0) {
         modsizeBytes = modsize / 8;
    } else if (modsize < 8) {
         modsizeBytes = 1;
    } else {
         modsizeBytes = (modsize / 8) + 1;
    }

    return modsizeBytes;

}

// calculate the number of file blocks based on the filesize and blocksize
long CalcFileBlocks(long filesize, long blocksize) {

     long remainder;
     long blocksCount = 0;
     remainder = filesize % blocksize;

     if (remainder == 0) {
         blocksCount = filesize / blocksize;
     } else {
         blocksCount = (filesize / blocksize) + 1;
     }

     return blocksCount;

}

// calculate the last block size based on the filesize and blocksize
// C doesn't have multiple returns so I had to split them
long CalcFileBlocksRemainder(long filesize, long blocksize) {

     long remainder;
     remainder = filesize % blocksize;

     if (remainder == 0) {
         remainder = blocksize;
     } 

     return remainder;

}

// calculates an exponent of 2 less than the byte block int
int calcExponent (mpz_t blockint) {
    int exponent = 0;

    mpz_t two, result;

    mpz_init_set_str(two, "2", 10);
    mpz_init_set_str(result, "2", 10);

    do {
      mpz_mul(result, result, two);
      exponent++;
    } while(mpz_cmp(result,blockint) < 0);

    mpz_clear(two);
    mpz_clear(result);

    return exponent;
}

// display the byteblock
void printByteblock(unsigned char *byteblock, long blocksize, bool ishex) {
        long i;
        for(i=0; i < blocksize; i++)
        {
            if (ishex == false) {
                printf("%d ",    byteblock[i]);
            } else {
                printf("%02X ", byteblock[i]);
            }
        }

        printf("\n");

        /* for (int f = 0; f < blocksize; f++) {
                  // std::cout << setw(2) << std::uppercase << std::hex << setfill('0') << (uint32_t)byteblock[f];
                  std::cout << " ";
        }
        std::cout << std::endl;
        */

}

// display the byteblock
void printByteblock3 (char *byteblock, long blocksize, bool ishex) {
        long i;
        for(i=0; i < blocksize; i++)
        {
            if (ishex == false) {
                printf("%d ",    byteblock[i]);
            } else {
                printf("%02X ", byteblock[i]);
            }
        }

        printf("\n");

}


/**
 * Get the size of a file.
 * @param filename The name of the file to check size for
 * @return The filesize, or 0 if the file does not exist.
 */
size_t getFilesize(const std::string& filename) {
    struct stat st;
    if(stat(filename.c_str(), &st) != 0) {
        return 0;
    }
    return st.st_size;
}


// calculate the input file size
long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

// copy a char buffer to an int
// https://stackoverflow.com/questions/34943835/convert-four-bytes-to-integer-using-c
int buffToInteger(char* buffer)
{
    int a;
    memcpy( &a, buffer, sizeof( int ) );
    return a;
}

// Checks if the platform is big- or little-endian.
// https://github.com/steinwurf/endian/blob/master/src/endian/is_big_endian.hpp
//
// From a test proposed here:
// http://stackoverflow.com/questions/1001307/
//
// @return True if the platform is big endian otherwise false.
inline bool is_big_endian()
{
    union
    {
        uint32_t i;
        uint8_t c[4];
    } test = {0x01020304};

    return test.c[0] == 1;
}
