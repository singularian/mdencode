#include <iostream>
#include <gmpxx.h>
#include <gmp.h>

int main() {
  mpz_class x("7612058254738945");
  mpz_class y("9263591128439081");

  std::cout << "    " << x << "\n"
            << "*\n"
            << "    " << y << "\n"
            << "--------------------\n"
            << x * y << "\n";

  size_t count;
  mpz_t byteblockInt, modremainder;
  mpz_init_set_str(byteblockInt, "0", 10);  
  unsigned char byteblock [] = { 0, 176, 100, 82, 220, 198, 148, 121, 155, 202, 74 };
  unsigned char byteblock2 [11];
  ////// void mpz_import (mpz_t rop, size_t count, int order, size_t size, int endian, size_t nails, const void *op)
  // within each word endian can be 1 for most significant byte first, -1 for least significant first, or 0 for the native endianness of the host CPU
  // order has to match the modulus scan mpz_export
  // 1 for most significant byte first, 
  // -1 for least significant first, 
  // or 0 for the native endianness of the host CPU
  mpz_import (byteblockInt, 11,  -1, sizeof(byteblock[0]), 0, 0, byteblock);
  gmp_printf("input byte byteblockint %Zd\n", byteblockInt);
  // mpz_export(test, &count, 0, sizeof(byteblock[0]), 0, 0, byteblockInt);
  mpz_export(byteblock2, &count, -1, sizeof(byteblock2[0]), 0, 0, byteblockInt);
  gmp_printf("output byte byteblockint %Zd count\n", byteblockInt);
  printf("count %ld\n", count);

  int i = 0;
  for (i = 0; i < 11; i++) {
        printf("%d ", byteblock2[i]); 
  }
  printf("\n");

  char a [1000];
  // char * mpz_get_str (char *str, int base, mpz_t op)
  mpz_get_str (a, 10, byteblockInt);
  printf("\nOutput bigint string %s aaaaa\n", a);

  // test2
  unsigned char byteblock3 [] = { 0, 0, 176, 1 };
  // unsigned char byteblock3 [] = { 255, 0, 0, 176 };
  // unsigned char byteblock3 [] = { 255, 0, 176,0 };
  // unsigned char byteblock3 [] = { 0, 176,0 };
  unsigned char byteblock4 [3];

  mpz_import (byteblockInt, 4,  1, sizeof(byteblock[0]), 0, 0, byteblock3);
  // mpz_import (byteblockInt, 3,  0, sizeof(byteblock[0]), 0, 0, byteblock3);
  // mpz_import (byteblockInt, 3,  -1, sizeof(byteblock[0]), 0, 0, byteblock3);
  gmp_printf("input byte byteblockint %Zd\n", byteblockInt);
  // mpz_export(byteblock4, &count, 0, sizeof(byteblock[0]), 0, 0, byteblockInt);
  mpz_export(byteblock4, &count, 1, sizeof(byteblock3[0]), 0, 0, byteblockInt);
  // mpz_export(byteblock4, &count, -1, sizeof(byteblock3[0]), 0, 0, byteblockInt); // -1 works but its slower
  // gmp_printf("output byte byteblockint %Zd count\n", byteblockInt); // works
  printf("count %ld\n", count);

  int diff;
  int blocksize = 4;
  if (count < blocksize) {
               diff = blocksize - count;
               printf ("count less than blocksize %ld %d diff %d\n", count, blocksize, diff);
               // for (n = 0; n < blocksize; n++) byteblock[0] = 0;
               printf("byteblock "); 
               // for (i = 0; i < blocksize; i++) printf("%02X ", byteblock4[i]);
               for (i = 0; i < blocksize; i++) printf("%0d ", byteblock4[i]);
               printf("\n"); 
               for (i = (blocksize - diff); i >= 0; i--) byteblock4[i+diff] = (unsigned char) byteblock4[i];
  }
  for (i = 0; i < diff; i++) byteblock4[i] = 0;


  for (i = 0; i < 4; i++) {
        printf("%d ", byteblock4[i]); 
  }
  printf("\n");


  mpz_init_set_str(byteblockInt, "7", 10);  
  mpz_init_set_str(modremainder, "1", 10); 
  mpz_mul(modremainder, byteblockInt, modremainder);
  gmp_printf("output modmult %Zd count\n", modremainder); 
 

  mpz_clear(byteblockInt);
  mpz_clear(modremainder);

  return 0;
}
