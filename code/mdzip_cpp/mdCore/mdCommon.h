#include <cstring>
#include <gmp.h>
#include <unordered_set>
#include <set>
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
// this is used in mdzip.cpp to set the modulus floor
// 2 ^ exponent < byteblock bigint
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

// Calculates an exponent of the modulus less than the byte block int
// This isn't really used
// It's more for Convenience to show the alternative exponent in decoderRandomTestHC.cpp
// mod ^ exponent < byteblock bigint
int calcExponentModulus (mpz_t modulus, mpz_t blockint) {
    int exponent = 0;

    mpz_t result;

    mpz_init_set_str(result, "", 10);
    mpz_add (result, result, modulus); 


    do {
      mpz_mul(result, result, modulus);
      exponent++;
    } while(mpz_cmp(result,blockint) < 0);

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

}

// display the byteblock
void printByteblock2 (char *byteblock, long blocksize, bool ishex) {
    long i;

    for (int i = 0; i < blocksize; i++) {
        std::cout << std::setw(2) << std::uppercase << std::hex << std::setfill('0') << (uint32_t)byteblock[i];
        std::cout << " ";
    }
    std::cout << std::endl;       
}

// unique a vector list and preserve the order
// 1, 2, 3, 3, 11, 4, 5, 11, 16, 11, 20, 19
// unique output 1 2 3 11 4 5 16 20 19
void use_std_hash_remove_dup( std::vector<int>& num )
{
    std::unordered_set<int> set;
    std::size_t pos = 0;
    for( int v : num ) if( set.insert(v).second ) num[pos++] = v;
    num.resize(pos);
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

// check if a file exists
bool CheckIfFileExists(std::string& filename) {
    std::ifstream ifile(filename.c_str());
    return (bool)ifile;
}

// get the file extension
// filename.mdz
// extension is mdz
std::string fileExtension(const std::string& file){
    std::string::size_type pos=file.find_last_of('.');
    if(pos!=std::string::npos&&pos!=0)return file.substr(pos+1);
    else return "";
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
