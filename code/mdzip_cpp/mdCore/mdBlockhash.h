/*
    Some of these signatures hashes don't have all in one functions
    mdBlockhash.h is just to create an all in one function if it doesn't exist
*/


// execute the librhash edonr-224
int getEDONR224(unsigned char *byteblock, int blocksize, unsigned char *digest) {

    edonr_ctx edr;
    rhash_edonr224_init(&edr);
    rhash_edonr256_update(&edr, byteblock, blocksize);
    rhash_edonr256_final(&edr, digest);


    return 0; 

}  

// execute the librhash has-160 rhash
int getHAS160(unsigned char *byteblock, int blocksize, unsigned char *digest) {

    has160_ctx hash;
    rhash_has160_init(&hash);

    rhash_has160_update(&hash, byteblock, blocksize);
    rhash_has160_final(&hash, digest);

    return 0; 

}  

// execute the libcrypto++ ripe128 hash on a byte block
int getRIPEMD128(unsigned char *byteblock, int blocksize, unsigned char *digest) {
    CryptoPP::RIPEMD128 ripe128;
    ripe128.Update(byteblock, blocksize);
    ripe128.Final(digest);
    return 0; 

}        

// execute the libcrypto++ ripe160 hash on a byte block
int getRIPEMD160(unsigned char *byteblock, int blocksize, unsigned char *digest) {
    CryptoPP::RIPEMD160 ripe160;
    ripe160.Update(byteblock, blocksize);
    ripe160.Final(digest);
    return 0; 
}

// execute the libcrypto++ ripe256 hash on a byte block
int getRIPEMD256(unsigned char *byteblock, int blocksize, unsigned char *digest) {
    CryptoPP::RIPEMD256 ripe256;
    ripe256.Update(byteblock, blocksize);
    ripe256.Final(digest);
    return 0; 
}

// execute the libcrypto++ ripe320 hash on a byte block
int getRIPEMD320(unsigned char *byteblock, int blocksize, unsigned char *digest) {
    CryptoPP::RIPEMD320 ripe320;
    ripe320.Update(byteblock, blocksize);
    ripe320.Final(digest);
    return 0;      
}        

// execute the libcrypto++ tiger 192 hash on a byte block
int getTiger(unsigned char *byteblock, int blocksize, unsigned char *digest) {
    CryptoPP::Tiger hash;
    hash.Update(byteblock, blocksize);
    hash.Final(digest);
    return 0;
}        
