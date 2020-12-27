#include <gmp.h>
#include <sys/stat.h>

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

// calculate the file blocks based on the filesize and blocksize
long CalcFileBlocks(long filesize, long blocksize) {

     long remainder;
     long blocksCount = 0;
     remainder = filesize % blocksize;
     if (remainder == 0) {
         blocksCount = filesize / blocksize;
         remainder = blocksize;
     } else {
         blocksCount = (filesize / blocksize) + 1;
     }

     return blocksCount;

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


