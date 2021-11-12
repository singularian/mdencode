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
#include "mdZip/mdUnzip47.h"

using namespace std;

void usage();

int main (int argc, char **argv) {

     std::string filename;
     long filesize;
     long blockcount;
     long blocksize   = 10;
     int modsize      = 32;
     int threadnumber = 0;
     int threadcount  = std::thread::hardware_concurrency();
     // current signature number handled in the hash context list
     int signum       = LAST - 1;

     // process the command line argument with the CLI11 command line parser
     CLI::App app{"MDEncode MDunzip Sip 47 C++ Program"};
     app.add_option("-f,--file",    filename,    "MDunzip filename")->check(CLI::ExistingFile)->required();
     app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

     // display the block hash list 
     bool list = false;
     app.add_flag("-l,--list", list, "List the mdzip file");

     // Add an mdzip option boolean to just allow mdlist to be specified
     // if runmdzip is false you don't run it
     // you can run mdlist or mdunzip or both mdlist and mdunzip
     bool rununzip = false;
     app.add_flag("-u,--unzip", rununzip, "MDunzip a file");

     // overwrite the unzipped output file if it exists
     // stop and throw an error if it doesn't
     bool overwrite = false;
     app.add_flag("-o,--over", overwrite, "Overwrite an existing mdunzip output file");

     // set logging
     bool runlogging = false;
     app.add_flag("--log", runlogging, "Run Logging");

     // set debug
     bool debug = false;;
     app.add_flag("--debug", debug, "Run Dubug");

     // run post mdunzip validation 
     bool validate = false;
     app.add_flag("--val", validate, "Run the File Signatures on the output uncompressed file");

     // validate the mdzip file 
     bool validatemdzip = false;
     app.add_flag("--valmdzip", validatemdzip, "Validate the mdzip file");

     // check the argument count and display the usage if it's not specified
     if (argc < 2)
     {
        std::cout << app.help() << std::endl;
        usage();
        return 0;
     }

     // process the command arguments
     try {
        app.parse(argc, argv);
     } catch(const CLI::ParseError &e) {
        return app.exit(e);
     }

     // initialize the mdunzip object 
     mdUnzip47 mdunzip(filename, threadcount, overwrite, runlogging, validate, list);

     // validate the mdzip file
     if (validatemdzip) {
         return mdunzip.validateMDzip();
     }

     // execute the mdlist display mdzip file blocks if list is true
     // ./mdunzip --file=test.mdz --list=true
     try {
         mdunzip.mdlist();
     } catch (exception& ex) {
         std::cout << "MDList Exception" << std::endl;
     } 
 
     // run the mdunzipfile
     // should add a block only format with a specified signature and modsize and no header
     // like highway hash and mod 32 and block 14 and 1 or no signature key
     // ./mdunzip --file=phone.txt.mdz --threads=32
     // currently creates phone.txt.mdz.out
     int rc = 0;
     try {
         if (!list) rc = mdunzip.mdunzipfile();
         // if the list and mdunzip flags are set run both
         // I might make it just run one
         if (list && rununzip) rc = mdunzip.mdunzipfile();
     } catch (exception& ex) {
         std::cout << "MDunzip Exception" << std::endl;
     }

     return rc;
}

// display the usage
void usage() {
std::string usageline = R"(
MDunzip Examples:
   mdunzipsip47 --file=filename.mdz --thread=16 
   mdunzipsip47 --file=test.mdz --thread=32 
   mdunzipsip47 --file=test.mdz     --list --val
   mdunzipsip47 --file=filename.mdz --list --unzip
   mdunzipsip47 --file=filename.mdz --valmdzip

MDzip Examples:
   mdzipsip47 --file=test.txt 
   mdzipsip47 --file=test.txt --fh 1 2 3 
   mdzipsip47 --file=test.txt --fh 11     --randbh
   mdzipsip47 --file=test.txt --fh 11     --randbh=false
   mdzipsip47 --file=randfile --fh 13     --randbh --inc
   mdzipsip47 --file=randfile --fh 13     --randbh --dec

)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported  to the usage
    // This is currently only used for the file signatures in mdzip sip47
    cout << "File Hashlist:" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}

