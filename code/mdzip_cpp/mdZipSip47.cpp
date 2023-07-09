/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdunzip Program
 * 
 * mdUnzip.cpp unzips a mdzip file or displays the block list
 * 
 * It can use two file signature block formats 
 * siphash40 with a 32 bit modulus and a 10 block file.
 * siphash48 with a 24 bit modulus and a 10 block file (--sip48 option)
 *
 * siphash40 uses 
 * siphash 40 
 * 32 bit modulus
 * 7 bit bitream for the modulus exponent
 * 10 bytes for the file block
 * 
 * siphash48 uses 
 * siphash 48
 * 24 bit modulus
 * 7 bit bitream for the modulus exponent
 * 10 bytes for the file block
 * 
*/
#include <iostream>
#include "external/CLI11.hpp"
#include "mdZip/mdZip47.h"
#include "setRange/setRange.h"

void usage(bool displayHCL); 

using namespace std;


int main (int argc, char **argv) {

      std::string filename;
      long filesize;
      long blockcount;
      long blocksize   = 10;
      int modsize      = 32;
      int threadnumber = 0;
      int threadcount  = 8;
      // current signature number handled in the hash context list
      int signum       = LAST - 1;

      // process the command line argument with the CLI11 command line parser
      CLI::App app{"MDEncode MDzip Sip 47 C++ Program"};
      app.add_option("-f,--file",    filename,    "MDzip filename")->check(CLI::ExistingFile)->required();
      // app.add_option("-b,--block",   blocksize,   "Blocksize number")->check(CLI::PositiveNumber)->check(CLI::Range(1,100));
      // app.add_option("-m,--mod",     modsize,     "Modulus size number")->check(CLI::PositiveNumber);
      // app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

      // add the file hash list parameter
      std::vector<int> flcsvvals;
      itSetRange isr;
      app.add_option("--fhs", [&flcsvvals, &signum, &isr](std::vector<std::string> val){
         return isr.splitRange(val, flcsvvals, signum);
      }, "File Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);

      // integer file hash list parameter
      std::vector<int> flvals;
      app.add_option("--fh", [&flvals, &signum, &isr](std::vector<std::string> val){
         return isr.splitRange(val, flvals, signum);
      }, "File Hashlist integers list")->expected(1,signum)->allow_extra_args(true);

      // specify sip48 for the block hashes
      bool sip48 = false;
      app.add_flag("--sip48", sip48, "Use Siphash48 for the block hash and modulus 24");   
      
      // specify hw48 for the block hashes
      bool hw48 = false;
      app.add_flag("--hw48", hw48, "Use Highway Hash 48 for the block hash and modulus 24");          

      // add the csv block hashlist parameter   
      /* std::vector<int> csvvals;
      app.add_option("-r,--bhs", [&csvvals, &signum](std::vector<std::string> val){
         return splitRange(val, csvvals, signum);
      }, "File Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);
      */

      // integer block hash list parameter
      /* std::vector<int> intvals;
      app.add_option("-s,--bh", [&intvals, &signum](std::vector<std::string> val){
         return splitRange(val, intvals, signum);
      }, "Block Hashlist integers list")->expected(1,signum)->allow_extra_args(true);
      */

      // add a hash keylist parameter
      // TODO Not currently used
      bool randkey;
      std::string keylist;
      app.add_option("-k,--keylist", keylist, "Keylist csv string");

      // TODO randomize the keylist for the file hashes

      // randomize the keylist for the block hashes
      bool randombh = false;
      app.add_flag("--randbh", randombh, "Randomize the Block Hash Keylist");

      // set list blocks not currently used
      // bool listzip = false;
      // app.add_option("-x,--list", listzip, "Display the Block list");

      bool increment = false;
      app.add_flag("--inc", increment, "Increment the Block Hash Keylist");
      bool decrement = false;
      app.add_flag("--dec", decrement, "Decrement the Block Hash Keylist");

      // set logging
      bool runlogging = false;
      app.add_flag("-l,--log", runlogging, "Run Logging");

      // display the usage without the hashlist 
      // bool nohashlist = false;
      // app.add_flag("-n,--usage", nohashlist, "MDzip Usage without the hash list"); 

      // display the usage without the hashlist
      // requires the file parameter so this doesn't quite work
      /* if (nohashlist) {
         std::cout << app.help() << std::endl;
         usage(false);
         return 0;
      } */   

      // check the argument count and display the usage if it's not specified
      if (argc < 2)
      {
         std::cout << app.help() << std::endl;
         usage(true);
         return 0;
      }

      // process the command arguments
      try {
         app.parse(argc, argv);
      } catch(const CLI::ParseError &e) {
         return app.exit(e);
      }

      // process the file hashlists
      // combine the flcsvvals and flvals vectors
      flcsvvals.insert(flcsvvals.end(), flvals.begin(), flvals.end());

      // set the default block hash list to siphash 40
      std::vector<int> blocklist = { SIP40 };

      // if the siphash 48 flag is set use siphash 48 and modulus 24 
      if (sip48) {
         blocklist = { SIP48 };
         modsize = 24;
      }

      // if the hw48 flag is set use siphash 48 and modulus 24 
      if (hw48) {
         blocklist = { HW48 };
         modsize = 24;
      }

      // possibly create a rolling signature where it can run si40/48 on one and hw40/48 on alternate runs 
      // byteblock mod x run this sig
      // alternate between mdscan iterations? Has to be repeatable
      // it can have a setter to set the rolling signature number in the hashcontext list before generating it
      // 
      // another alternative is sip48/hw48 mod24 and a 3-6/7bitstream file block encoding. It trades some speed for a larger signature
      // run mdzipfile
      mdZip47 mdz(filename, blocksize, modsize, flcsvvals, blocklist, randombh, increment, decrement, runlogging);
      mdz.mdzipfile();

}

// display the usage
void usage(bool displayHCL) {
   std::string usageline = R"(
   MDzip Examples:
      mdzipsip47 --file=test.txt 
      mdzipsip47 --file=test.txt --fh 1 2 3 
      mdzipsip47 --file=test.txt --fh 11     --randbh
      mdzipsip47 --file=test.txt --fh 11     --randbh=false
      mdzipsip47 --file=randfile --fh 13     --randbh --inc
      mdzipsip47 --file=randfile --fh 13     --randbh --dec

   MDunzip Examples:
      mdunzipsip47 --file=filename.mdz --thread=16 
      mdunzipsip47 --file=test.mdz --thread=16 
      mdunzipsip47 --file=test.mdz --list
      mdunzipsip47 --file=filename.mdz --list --unzip 
      mdunzipsip47 --file=filename.mdz --valmdzip 
   )";

   std::cout << usageline << std::endl;
   std::cout << std::endl;

    // Add the current file hash list signatures currently supported
    // to the usage
   if (displayHCL) {
      std::cout << "File Hashlist:" << std::endl << std::endl;
      mdHashContextList hcl;
      hcl.displayHLRegistry(0);
   }

}         

