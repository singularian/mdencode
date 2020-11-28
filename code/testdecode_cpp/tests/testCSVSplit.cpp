#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include "../external/CLI11.hpp"

void print(std::vector<std::string> const &input);
std::vector<std::string> csv_split(std::string source, char delimeter);

int main(int argc, char** argv)
{

    CLI::App app{"App description"};

    std::string hashlist = "";
    app.add_option("-b,--bh", hashlist, "Hashlist string");

    std::vector<int> vals;
    app.add_option("-v,--vals", vals)->expected(-1); 

    // takess a csv arg and converts it to an int vector
    std::vector<int> csvvals;
    app.add_option("-c,--csvi", csvvals, "Block Hashlist csv string")->delimiter(',')->check(CLI::PositiveNumber);

    std::vector<int> ivals;
    app.add_option("-i,--hi", ivals, "Block Hashlist integers list")->check(CLI::PositiveNumber);

    CLI11_PARSE(app, argc, argv);


    std::cout << "hashlist " << hashlist << std::endl;

    std::regex re{"((?:[^\\\\,]|\\\\.)*?)(?:,|$)"};
    std::vector<std::string> m_vecFields {std::sregex_token_iterator(hashlist.begin(), hashlist.end(), re, 1), std::sregex_token_iterator()};

    std::cout << std::endl << "csv vector ";
    // print(m_vecFields);
    // for(std::string v : m_vecFields)
    for(int v : csvvals)
        std::cout << ": " << v << " "; 
 
    std::cout << std::endl << "Prefix";
    for(int v2 : ivals)
        std::cout << ": " << v2 << " ";
    std::cout << std::endl;

    

    return 0;
}

