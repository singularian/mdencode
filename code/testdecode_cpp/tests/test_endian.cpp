#include <iostream>
#include <gmp.h>
#include <cstring>

using namespace std;

unsigned long long mpz_get_ull(mpz_t n);
void string2hexString(unsigned char* input, char* output);

int main() {

   unsigned char byteblock[20];
   mpz_t blockInt, Int2;
   mpz_init_set_str(blockInt, "938439489204890234", 10);
   mpz_init_set_str(Int2, "1110", 10);

   size_t count;
   uint8_t results[20];
   char value[100];

   unsigned long long result = 0;

   for (int n = 0; n < 20; n++) {
       byteblock[n] = 0;
   }
   // export this to a bunch of longs???
   // mpz_export(byteblock, &count, -1, sizeof(byteblock[0]), 0, 0, blockInt);
   mpz_export(byteblock, &count, 0, sizeof(byteblock[0]), 0, 0, blockInt);
     
   for (int n = 0; n < 20; n++) {
          printf("%02x ", byteblock[n]);
          // sprintf(value, "%02X", (int) byteblock[n]);
   }
   printf("\nTest value ");

   string2hexString((unsigned char *) byteblock, value);

   printf("hex_str: %s\n", value);

   result = strtoll(value, NULL, 16);
   printf("long long : %lld\n", result);

   printf("\n");

   unsigned long long temp = 0;
   temp = result;
   unsigned char buf[8];

   buf[0] = (temp >> 8)  & 0xFF;
   buf[1] = (temp >> 16) & 0xFF;
   buf[2] = (temp >> 24) & 0xFF;
   buf[4] = (temp >> 32) & 0xFF;
   buf[5] = (temp >> 40) & 0xFF;
   buf[6] = (temp >> 48) & 0xFF;
   buf[7] = (temp >> 56) & 0xFF;

   // ulltoa(result,buf,16);
   //for (int n = 0; n < 8; n++) {
   // for (int n = 8; n >= 0; n--) {
   //      printf("%lld ", ((temp >> 56)));
   //}

   // dump the hex bytes in the long long int
   unsigned char* pointer = (unsigned char*)&temp;
   // little endian has them in reverse order
   // for(int i=0; i<sizeof(temp); i++)
   for(int i=(sizeof(temp) -1); i >= 0; i--)
        cout << hex << (unsigned int)pointer[i] << " ";

   printf("\n");

   mpz_add (blockInt, blockInt, Int2);

   gmp_printf("%Zd\n", blockInt);
   // printf("mpz_t %d", blockInt);

     
   mpz_clear(blockInt);
   mpz_clear(Int2);
   return 0;
}

unsigned long long mpz_get_ull(mpz_t n)
{
    unsigned int lo, hi;
    mpz_t tmp;

    mpz_init( tmp );
    mpz_mod_2exp( tmp, n, 64 );   /* tmp = (lower 64 bits of n) */

    lo = mpz_get_ui( tmp );       /* lo = tmp & 0xffffffff */ 
    mpz_div_2exp( tmp, tmp, 32 ); /* tmp >>= 32 */
    hi = mpz_get_ui( tmp );       /* hi = tmp & 0xffffffff */

    mpz_clear( tmp );

    return (((unsigned long long)hi) << 32) + lo;
}

//function to convert ascii char[] to hex-string (char[])
void string2hexString(unsigned char* input, char* output)
{
    int loop;
    int i; 
    
    i=0;
    loop=0;
    
    while(input[loop] != '\0')
    {
        sprintf((char*)(output+i),"%02X", input[loop]);
        loop+=1;
        i+=2;
    }
    //insert NULL at the end of the output string
    output[i++] = '\0';
}

