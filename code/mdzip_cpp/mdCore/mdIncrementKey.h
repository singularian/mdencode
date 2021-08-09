
// add a blocknumber long integer to a gmp bigint
int incrementByteblock(int blocksize, unsigned char *byteblock, long blocknumber)
{
    // initialize the gmp bigint variables         
    int keysize = 16;
    int byteorder = 0;
    int endian    = 0;
    size_t count;
    mpz_t keyblockInt;
    mpz_init_set_str(keyblockInt, "0", 10);

    // convert the input byte block to an mpz bigint
    mpz_import (keyblockInt, keysize, byteorder, sizeof(byteblock[0]), endian, 0, byteblock);

    // add the blocknumber to the byte block big int
    mpz_add_ui (keyblockInt, keyblockInt, blocknumber);

    // cout << "testing byteblock incrementer st " << std::endl;
    // printByteblock(byteblock, keysize, true); 

    // export the gmp bigint results back to the input byte block
    mpz_export(byteblock, &count, byteorder, sizeof(byteblock[0]), endian, 0, keyblockInt);      
    padBlockBytes(count, keysize, byteblock);

    // printByteblock(byteblock, keysize, true);
    // cout << "testing byteblock incrementer end " << std::endl;

    mpz_clear(keyblockInt);

    return 0;
}
