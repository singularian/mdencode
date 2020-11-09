#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include <openssl/sha.h>
#include <sstream>
#include "../mdHashContextList.h"
// #include "../external/format.h"

int main()
{
    mdHashContextList hcl;
    hcl.displayHashList(0);
  
    std::vector<int> v = { 1, 2, 3 };
    hcl.setFileHashList(v);
    hcl.setBlockHashList(v);

    hcl.displayHLvectors();

    unsigned char byteblock [] = { 139, 176, 100, 82, 220, 98, 148, 121, 155, 202, 74 };
    hcl.setBlockHashList(byteblock, 11);

    // fmt::print("strftime-like format: {:%x)\n", num);

    /* std::stringstream ss;
    for(int i=0; i<11; ++i)
    ss << std::hex << (int)num[i];
    std::string mystr = ss.str();
 
    std::cout << "test " << mystr << std::endl;
    */
    std::cout << hcl.displayHLhashes();
}
