#include <iostream>
#include <gmpxx.h>

int main() {
  mpz_class x("7612058254738945");
  mpz_class y("9263591128439081");

  std::cout << "    " << x << "\n"
            << "*\n"
            << "    " << y << "\n"
            << "--------------------\n"
            << x * y << "\n";

  return 0;
}
