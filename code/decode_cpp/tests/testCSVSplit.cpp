#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include "../CLI11.hpp"

void print(std::vector<std::string> const &input);
std::vector<std::string> csv_split(std::string source, char delimeter);

int main(int argc, char** argv)
{

    CLI::App app{"App description"};

    std::string hashlist = "default";
    app.add_option("-b,--bh", hashlist, "A help string");

    char sep = ',';
    // std::vector<std::string> hashlistmap;
    { std::vector<std::string> hashlistmap = csv_split(hashlist, sep); }

    CLI11_PARSE(app, argc, argv);


    std::cout << "hashlist " << hashlist << std::endl;

    std::regex re{"((?:[^\\\\,]|\\\\.)*?)(?:,|$)"};
    std::vector<std::string> m_vecFields {std::sregex_token_iterator(hashlist.begin(), hashlist.end(), re, 1), std::sregex_token_iterator()};

    print(m_vecFields); 
 
    // print(hashlistmap);

    std::vector<std::string> input = { "a", "b", "c" };
    print(input); 
    std::cout << std::endl;

    return 0;
}

void print(std::vector<std::string> const &input)
{
	std::copy(input.begin(),
			input.end(),
			std::ostream_iterator<std::string>(std::cout, " "));
}

std::vector<std::string> csv_split(std::string source, char delimeter) {
    std::vector<std::string> ret;
    std::string word = "";
    int start = 0;

    bool inQuote = false;
    for(int i=0; i<source.size(); ++i){
        if(inQuote == false && source[i] == '"'){
            inQuote = true;
            continue;
        }
        if(inQuote == true && source[i] == '"'){
            if(source.size() > i && source[i+1] == '"'){
                ++i;
            } else {
                inQuote = false;
                continue;
            }
        }

        if(inQuote == false && source[i] == delimeter){
            ret.push_back(word);
            word = "";
        } else {
            word += source[i];
        }
    }
    ret.push_back(word);

    return ret;
}


