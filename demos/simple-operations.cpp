#include <iostream>

#include "BigInt.h"

int main()
{
    BigInt big1("10000000000"); // Values can be arbitrarily large
    BigInt big2("30000000000");

    BigInt big3 = big1 + big2;

    std::cout << big3 << std::endl;
    std::cout << 1234 + big1 * 18 * big2 * 2 << std::endl;

    return 0;
}
