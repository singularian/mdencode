/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan Test Program
 * decoderRandomTestHC.cpp
 *
 * This is a C++ GMP modulus scan multithreaded test program 
 * MDencode GMP requires the GMP Library to build https://gmplib.org/ and the openssl library
 * It also requires the libfnv library to be built and installed for the FNV1 signature
 * MDencode uses a hash context list configured with the command arguments to run against a modulus scan 
 *
 * This includes the MD2 and MD4 and MD5 and MD6 signatures in the hash context list.
 *
 * Paramters: modulus size, threadnumber, blocksize and hashlists and hashlist keys (TODO), hexbytes, enable logging
 * 
 */

#include <iostream>
#include "external/CLI11.hpp"
#include "mdZip/mdTest.h"

using namespace std;

void usage();

// main
int main (int argc, char **argv) {

      size_t blocksize = 12;
      int modsize      = 64;
      int threadnumber = 0;
      int threadcount  = std::thread::hardware_concurrency();
      // current signature number handled in the hash context list
      int signum       = LAST - 1;

      // process the command line argument with the CLI11 command line parser
      CLI::App app{"MDEncode GMP C++ Test Program"};
      app.add_option("-b,--block", blocksize, "Blocksize number")->check(CLI::PositiveNumber)->check(CLI::Range(1,100));
      app.add_option("-m,--mod", modsize, "Modulus size number")->check(CLI::PositiveNumber);
      app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);
      //app.add_option("-v,--version", version, "Version number");

      // add the block hashlist parameter
      // csv hash list    
      std::vector<int> csvvals;
      app.add_option("-r,--bh", [&csvvals, &signum](std::vector<std::string> val){
         return splitRange(val, csvvals, signum);
      }, "Block Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);

      // integer block hash list
      std::vector<int> intvals;
      app.add_option("-s,--hl", [&intvals, &signum](std::vector<std::string> val){
            return splitRange(val, intvals, signum);
      }, "Block Hashlist integers list")->expected(1,signum)->allow_extra_args(true);

      // randomize the keylist for the block hashes
      bool randombh = false;
      app.add_flag("--randbh", randombh, "Randomize the Block Hash Keylist");

      // process the hex byte arguments
      std::string hexstring; 
      // I think CLI11 uses -h for help so I can't use -h
      app.add_option("-x,--hex", hexstring, "Hex Byteblock string");

      bool runlogging = false;
      app.add_flag("-l,--log", runlogging, "Run Logging");

      // TODO Should add an option to disable the modulus scan and just run the hash
      bool skipDecode = false;
      app.add_flag("-q,--skip", skipDecode, "Skip the Modulus Scan");

      try {
         app.parse(argc, argv);
      } catch(const CLI::ParseError &e) {
         return app.exit(e);
      }

      // combine csvvals and the integer vals block hash list
      // the hash context list will unique them in non sorted order
      csvvals.insert(csvvals.end(), intvals.begin(), intvals.end());

      // if the hash context list is empty assign a default value
      if (csvvals.size() == 0) {
         csvvals = { FAST64 };
      }    

      // check if the argument count is less than 2
      // then display the usage
      if (argc < 2)
      {
            cout << app.help() << endl;
            usage();
            return 0;
      }

      // initialize the mdText object
      mdTest mdt;
      
      unsigned char *byteblock;
      // generate a random n byte byteblock if the hexstring is empty
      if (hexstring.empty()) {
         byteblock = mdt.genRandomByteBlock(blocksize);
      // process the hex string into a byte block
      } else {
         byteblock = mdt.convertHexToByteBlock(hexstring);
         blocksize = hexstring.length() / 2;
      }

      // run the modulus scan decode on the byteblock
      // decodeRandomBlock(blocksize, modsize, randombh, csvvals, byteblock, threadnumber, threadcount, skipDecode, runlogging);  
      mdt.decodeRandomBlock(blocksize, modsize, randombh, csvvals, byteblock, threadnumber, threadcount, skipDecode, runlogging);

      return 0;
}


// display the usage
void usage() {
std::string usageline = R"(
Examples:
   decoderRandomTestHC2 -b 12 -m 64 -t 16
   decoderRandomTestHC2 --block=12 --mod=64    --threads=16
   decoderRandomTestHC2 --block=12 --mod=128   --threads=16
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=0011
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011 --log --hl 1 2 3 4 5
   decoderRandomTestHC2 --mod=64 --threads=16 --hex=FFd033FF202020202011 --log --bh 1,5,7
   decoderRandomTestHC2 --mod=64 --threads=32  --hl 1 2 3 4 5 --randbh --block=12
   decoderRandomTestHC2 --mod=64 --threads=32 --bh=6-8,23,33,34 --hl 1 2 3 4-8 --randbh --block=12
)";

      cout << usageline << endl;
      cout << endl; 

      // Add the current hash list signatures currently supported
      // to the usage
      cout << "Hashlist" << endl << endl;
      mdHashContextList hcl;
      hcl.displayHLRegistry(0);

}
