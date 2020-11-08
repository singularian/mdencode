#include <iostream>
#include "../external/CLI11.hpp"

int main(int argc, char** argv)
{

    CLI::App app{"App description"};

    std::string filename = "default";
    app.add_option("-f,--file", filename, "A help string");

    bool b = false;
    app.add_option("-b,--bool", b, "bool test");

    int count{0};
    app.add_option("-v,--value", count, "Value in range")->check(CLI::Range(3, 6));

    // int num1{-1}, num2{-1};
    // app.add_option("num2", num2, "second number")->check(CLI::Number);

    double value{0.0};  // = 3.14;
    app.add_option("-d,--double", value, "Some Value")->group("Other");

    int modulo{0};
    app.add_option("-m", modulo, "Some Value")->group("Other");

    CLI11_PARSE(app, argc, argv);


    std::cout << filename << std::endl;
    std::cout << "double " << value << std::endl;
    std::cout << std::boolalpha << b << std::endl;

    return 0;
}
