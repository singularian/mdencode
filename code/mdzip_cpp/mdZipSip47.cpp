/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan MDzip Program
 * 
 * mdZip.cpp zips a file with the mdzip format 
 * 
 * The current mdzip file extension is mdz
 *
 * 
 * 
*/
#include <iostream>
#include "external/CLI11.hpp"
#include "mdZip/mdZip47.h"

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
     app.add_option("--fhs", [&flcsvvals, &signum](std::vector<std::string> val){
        return splitRange(val, flcsvvals, signum);
     }, "File Hashlist csv string")->delimiter(',')->expected(1,signum)->allow_extra_args(true);

     // integer file hash list parameter
     std::vector<int> flvals;
     app.add_option("--fh", [&flvals, &signum](std::vector<std::string> val){
        return splitRange(val, flvals, signum);
     }, "File Hashlist integers list")->expected(1,signum)->allow_extra_args(true);

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


     std::vector<int> blocklist = { SIP40 };

     // possibly create a rolling signature where it can run si40/48 on one and hw40/48 on alternate runs 
     // byteblock mod x run this sig
     // alternate between mdscan iterations? Has to be repeatable
     // it can have a setter to set the rolling signature number in the hashcontext list before generating it
     // 
     // another alternative is sip48 mod24 and a 3-6/7bitstream file block encoding. It trades some speed for a larger signature
     // run mdzipfile
     mdZip47 mdz(filename, blocksize, modsize, flcsvvals, blocklist, randombh, increment, decrement, runlogging);
     mdz.mdzipfile();

}

// display the usage
void usage(bool displayHCL) {
   std::string usageline = R"(
   MDzip Examples:
      mdzip --file=test.txt 
      mdzip --file=test.txt --fh 1 2 3 
      mdzip --file=test.txt --fh 11     --randbh
      mdzip --file=test.txt --fh 11     --randbh=false
      mdzip --file=randfile --fh 13     --randbh --inc
      mdzip --file=randfile --fh 13     --randbh --dec

   MDunzip Examples:
      mdunzip --file=filename.mdz --thread=16 
      mdunzip --file=test.mdz --thread=16 
      mdunzip --file=test.mdz --list
      mdunzip --file=filename.mdz --list --unzip 
      mdunzip --file=filename.mdz --valmdzip 
   )";

   std::cout << usageline << std::endl;
   std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
   if (displayHCL) {
      std::cout << "Hashlist:" << std::endl << std::endl;
      mdHashContextList hcl;
      hcl.displayHLRegistry(0);
   }

}         

