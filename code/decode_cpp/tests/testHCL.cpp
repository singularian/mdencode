#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <array>
#include "../mdHashContextList.h"

int main()
{
    mdHashContextList hcl;
    hcl.displayHashList(0);
  
    std::vector<int> v = { 1, 2, 3 };
    hcl.setFileHashList(v);

    hcl.displayHLvectors();
}
