/**
 *
 * https://github.com/singularian/mdencode
 *
 * Project mdencode
 *
 * setRange.h is the command line range parameter class
 *
 * setRange parses range arguments into an integer vector
 * input  1 2 3 4-8 10-12
 * output 1 2 3 4 5 6 7 8 10 11 12
 *
 */
#include <iostream>
#include <ctime>
#include <unordered_set>
#include <set>
#include <regex>
#include <unordered_map>
// #include <fmt/format.h>

using namespace std;

class itSetRange {
    private:
        int size = 0;
    public:

    // Default constructor
    itSetRange()
    {
    }

    // Destructor
    ~itSetRange()
    {
    }

    void init()
    {

    }

    // convert a csv or int vector range string to an int vector
    // this is for the CLI11 lambda function for hash lists
    // range example 1-9 or numberstart dash numberend
    // 1-3 12 19 20-22 = 1 3 12 19 20 21 22
    // 1-3,12,19,20-22 = 1 3 12 19 20 21 22
    bool splitRange(std::vector<std::string> &val, std::vector<int> &intvals, int signum)
    {
        char delim = '-';
        int number, start, end;
        std::vector<std::string> rangeVal;

        for (int i = 0; i < val.size(); i++)
        {
            // match for numbers
            if (std::regex_match(val[i], std::regex("([0-9]+)")))
            {

                // convert the number line to an integer
                try {
                    number = std::stoi(val[i]);
                } catch(exception &err) {
                    std::cout << "Conversion failure " << number << std::endl;
                    return false;
                }
                // if the range is greater than the range signum invalidate and return false
                if (number > signum) return false;

                intvals.push_back(number);

            }
            // check range numbers 1-9 or 10-23 or 23-1
            else if (std::regex_match(val[i], std::regex("([0-9]+\\-[0-9]+)")))
            {
                std::string number = val[i];
                rangeVal = splitString(number, delim);
                if (rangeVal.size() == 2)
                {
                    // convert the number line to an integer
                    try {
                        start = std::stoi(rangeVal[0]);
                        end   = std::stoi(rangeVal[1]);
                    } catch(exception &err) {
                        std::cout << "Conversion failure " << start << " " << end << std::endl;
                        return false;
                    }
                    // if the range is greater than the range signum invalidate and return false
                    if (start > signum || end > signum) return false;

                    // range values ascending
                    // the range should be 2-5 or the first number less than the second
                    // this adds 2 3 4 5 to the range list
                    if (start < end)
                    {
                        for (int j = start; j <= end; j++) {
                            intvals.push_back(j);
                        }
                    }
                    // range values descending
                    // the range should be 5-2 or the first number greater than the second
                    // this adds 5 4 3 2 to the range list
                    else if (start > end)
                    {
                       for (int j = start; j >= end; j--) {
                            intvals.push_back(j);
                       }
                    }
                    // equal range
                    // a = b
                    // 1-1 or 2-2
                    else if (start == end)
                    {
                        intvals.push_back(start);
                    }
                    else
                    {
                        return false;
                    }
                }
            }
            else
            {
                return false;
            }
        }
        return true;
    }

    // split a string
    // split("1,2,3,4", ',') = 1 2 3 4
    // split("1-2", '-')     = 1 2
    std::vector<std::string> splitString(std::string &s, char delim)
    {
        std::stringstream ss(s);
        std::string item;
        std::vector<std::string> elems;

        while (std::getline(ss, item, delim))
        {
            elems.push_back(item);
        }

        return elems;
    }
};
