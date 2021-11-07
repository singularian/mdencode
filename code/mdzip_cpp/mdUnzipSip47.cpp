/**
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode GMP C++ Modulus Scan mdunzip Program
 * 
 * mdUnzip.cpp unzips a mdzip file or displays the block list
 *
 * 
 * 
*/
#include <iostream>
#include "external/CLI11.hpp"
#include "mdZip/mdUnzip47.h"


using namespace std;

int validateMDzip(std::string filename, bool validatemdzipfile);
int mdlist(std::string filename, bool listfile, bool runlogging);
int mdunzipfile(std::string filename, int threadcount, bool overwrite, bool runlogging, bool validate);
void displayInfo(std::string& filename, double mdversion, long filesize, long blocksize, long blockcount, long blockremainder, int modsize, 
                 int modsizeBytes, std::string& filehashnames, std::string& blockhashnames, int hclfileblocksize, int hclblockblocksize, 
                 std::string& filehashvector, std::string& blockhashvector, std::string& blockkeys, std::string& filesig, bool mdlist, int threadcount );
void displayBlockInfo(std::string action, int blocksize, int blk, int lastblk, long blockremainder, int modexponent, mpz_t modulusIntRemainder, 
                      mdHashContextList &hclblock, mdMutexLog &log);
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
     CLI::App app{"MDEncode MDunzip Sip 40 C++ Program"};
     app.add_option("-f,--file",    filename,    "MDunzip filename")->check(CLI::ExistingFile)->required();
     app.add_option("-t,--thread,--threads", threadcount, "Thread count number")->check(CLI::PositiveNumber);

     // display the block hash list 
     bool list = false;
     app.add_flag("-l,--list", list, "List the mdzip file");

     // Add an mdzip option boolean to just allow mdlist to be specified
     // if runmdzip is false you don't run it
     // you can run mdlist or mdunzip or both mdlist and mdunzip
     bool runmdzip = false;
     app.add_flag("-u,--unzip", runmdzip, "MDunzip a file");

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
         return mdunzip.validateMDzip(filename, true);
     }

     // execute the mdlist display mdzip file blocks if list is true
     // ./mdunzip --file=test.mdz --list=true
     try {
         mdunzip.mdlist(filename, list, runlogging);
     } catch (exception& ex) {
         std::cout << "MDList Exception" << std::endl;
     } 
 
     // run the mdunzipfile
     // should add a block only format with a specified signature and modsize and no header
     // like highway hash and mod 32 and block 14 and 1 or no signature key
     // ./mdunzip --file=phone.txt.mdz --threads=32
     // currently creates phone.txt.mdz.out
     int rc = 0;
     //mdZip47 mdz(filename, blocksize, modsize, flcsvvals, blocklist, randombh, increment, decrement);
     //mdUnzip47 mdunzip(std::string fileName, int threadCount, bool overWrite, bool runLogging, bool Validate, bool listFile)
     try {
         if (!list) rc = mdunzip.mdunzipfile();
         if (list && runmdzip) rc = mdunzip.mdunzipfile();
     } catch (exception& ex) {
         std::cout << "MDunzip Exception" << std::endl;
     }

     return rc;
}

// display the usage
void usage() {
std::string usageline = R"(
MDunzip Examples:
   mdunzip --file=filename.mdz --thread=16 
   mdunzip --file=test.mdz --thread=32 
   mdunzip --file=test.mdz     --list --val
   mdunzip --file=filename.mdz --list --unzip
   mdunzip --file=filename.mdz --valmdzip

MDzip Examples:
   mdzip --file=test.txt 
   mdzip --file=test.txt --fh 1 2 3 
   mdzip --file=test.txt --fh 11     --randbh
   mdzip --file=test.txt --fh 11     --randbh=false
   mdzip --file=randfile --fh 13     --randbh --inc
   mdzip --file=randfile --fh 13     --randbh --dec

)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

    // Add the current hash list signatures currently supported
    // to the usage
    cout << "Hashlist:" << endl << endl;
    mdHashContextList hcl;
    hcl.displayHLRegistry(0);

}

