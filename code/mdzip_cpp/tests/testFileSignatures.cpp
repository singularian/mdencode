/*
 *
 * https://github.com/singularian/mdencode
 * 
 * Project MDencode testFileSignatures.cpp
 *
 * 
 */
#include <cstring>
#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <sstream>
#include "../mdCore/mdHashContextList.h"
#include "../testdecode_cpp/external/CLI11.hpp" 

//  testhashfile --file=file.txt --hl 14 15 13
int main (int argc, char **argv) {

    // current signature number handled in the hash context list
    int signum       = LAST - 1;

    // process the command line argument with the CLI11 command line parser
    CLI::App app{"MDEncode GMP C++ Test Program"};

    std::string filename;
    app.add_option("-f,--file", filename, "MDunzip filename")->check(CLI::ExistingFile)->required();

    // std::vector<int> vals;
    std::vector<int> fhlist = { 1, 10, 11, 12, 13, 14, 21, 22, 23, 24, 25, 26 };
    app.add_option("-s,--hl", fhlist, "File Hashlist integers list")->check(CLI::PositiveNumber)->check(CLI::Range(1,signum));

    try {
        app.parse(argc, argv);
    } catch(const CLI::ParseError &e) {
        return app.exit(e);
    }

    if (argc < 2)
    {
         cout << app.help() << endl;
         // usage();
         return 0;
     }
    

    // initialize the hash list object
    mdHashContextList hclfile;

    // display the current hash signature list
    // hclfile.displayHLRegistry(0);

    // initialize a file hash list vector  
    // hclfile.setVectorHL(file, HASHFILE);
    hclfile.setVectorHL(fhlist, HASHBLOCK);
    hclfile.setFileHashList(filename); // set the file hash list variables

    // display the block hash list
    // std::string vectorlist = hclfile.getHLvectorsString(HASHBLOCK);
    // std::string hashlist   = hclfile.displayHLhashes();

    std::cout << "File Hash Signature List" << std::endl;
    std::cout << hclfile.getHLvectorsString(HASHBLOCK) << std::endl;

    std::cout << "file hash list " << std::endl;
    std::cout << hclfile.displayHLhashes() << std::endl;
}

