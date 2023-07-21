/**
 *
 * https://github.com/singularian/mdencode
 *
 * Project mdencode test ranges
 *
 * itTestRanges.cpp
 *
*/
#include <iostream>
#include "external/CLI11.hpp"
// lib fmt print formatter
//#include <fmt/format.h>
//#include <fmt/core.h>
//#include <fmt/chrono.h>
// set Ranges
#include "setRange/setRange.h"

using namespace std;

bool testRanges(int lotnumbers, std::string rangelist, std::vector<int> t1, std::string name);
void usage();

int main (int argc, char **argv) {

    int rc = 0;
    int lotnumbers       = 10000;

    // process the command line argument with the CLI11 command line parser
    CLI::App app{"Iterator C++ Program"};

    // integer eliminator list parameter
    itSetRange isr;
    std::vector<int> intvals;
    app.add_option("-n,--num", [&intvals, &lotnumbers, &isr](std::vector<std::string> val){
        return isr.splitRange(val, intvals, lotnumbers);
    }, "Test Eliminators Integer List")->expected(0,lotnumbers)->allow_extra_args(true);

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

    // fmt::print("range list {}\n", fmt::join(intvals, " "));
    std::cout << "Range List ";
    for (int i: intvals) std::cout << i << ' ';
    std::cout << "\n";

    // test 1
    std::vector<int> testvals;
    std::string rangelist = "1,2,3-5,7-12,";
    vector<int> t1{1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12};

    testRanges(lotnumbers, rangelist, t1, "Test 1");

    // test 2 descending range
    rangelist = "5-3,33,";
    vector<int> t2{5,4,3,33};
    testRanges(lotnumbers, rangelist, t2, "Test 2");

    // test 3 equal ranges
    rangelist = "1-1,0,2-2,3-3,";
    vector<int> t3{1,0,2,3};
    testRanges(lotnumbers, rangelist, t3, "Test 3");

    // test 4 fail test
    // this just tests for a mismatch case
    rangelist = "5-3,33,";
    vector<int> t4{5,4,3,33, 44};
    testRanges(lotnumbers, rangelist, t4, "Test 4");

    return rc;
}

// test ranges
bool testRanges(int lotnumbers, std::string rangelist, std::vector<int> t1, std::string name) {
    itSetRange isr;

    std::vector<int> testvals;
    std::vector<std::string> rval = isr.splitString(rangelist,  ',');
    isr.splitRange(rval, testvals, lotnumbers);
    bool isPassed = true;

    if(testvals==t1) {
        // fmt::print("\nRange {} Range {} Passed\n", name, rangelist);
        // fmt::print("Range test r1 [{:<2}]\n", fmt::join(testvals, " "));
        // fmt::print("Range test t1 [{:<2}]\n", fmt::join(t1, " "));
        std::cout << "Passed" << std::endl;
    } else {
        // fmt::print("\nRange {} Range {} Failed\n", name, rangelist);
        // fmt::print("Range Test r1 [{:<2}]\n", fmt::join(testvals, " "));
        // fmt::print("Range Test t1 [{:<2}]\n", fmt::join(t1, " "));
        std::cout << "Failed" << std::endl;
        isPassed = false;
    }

    return isPassed;

}


// display the usage
void usage() {
std::string usageline = R"(
Iterator Examples:
   ./testranges -n 1-5 20 30
   ./testranges --num 1-5 20 30
)";

    std::cout << usageline << std::endl;
    std::cout << std::endl;

}