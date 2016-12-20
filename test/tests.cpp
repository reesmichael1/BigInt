#include <catch.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "../src/BigInt.h"

TEST_CASE("Constructor tests")
{
    SECTION("Constructor with value tests")
    {
        BigInt test123("123");
        std::vector<int> expectedTest123 = {1, 2, 3};
        CHECK(test123.getVector() == expectedTest123);

        BigInt test000("000");
        std::vector<int> expectedTest000 = {0};
        CHECK(test000.getVector() == expectedTest000);
         
        BigInt test001("001");
        std::vector<int> expectedTest001 = {1};
        CHECK(test001.getVector() == expectedTest001);

        BigInt test100("100");
        std::vector<int> expectedTest100 = {1, 0, 0};
        CHECK(test100.getVector() == expectedTest100);
    }

    SECTION("Constructor without value tests")
    {
        BigInt test;
        std::vector<int> expectedTest = {0};
        CHECK(test.getVector() == expectedTest);
    }

    SECTION("Constructor with non-integer string fails")
    {
        CHECK_THROWS(BigInt test("123abc"));
    }
}

TEST_CASE("Arithmetic tests")
{
    SECTION("Equality tests")
    {
        BigInt test0;
        BigInt testExplicit0("0");
        CHECK(test0 == testExplicit0);
        CHECK(testExplicit0 == test0);

        BigInt test123("123");
        CHECK(test123 == test123);

        CHECK_FALSE(test123 == test0);
        CHECK_FALSE(test0 == test123);
    }

    SECTION("Addition tests")
    {
        BigInt test123("123");
        BigInt test0;
        CHECK(test123 + test0 == test123);
        CHECK(test0 + test123 == test123);

        BigInt expected246 = BigInt("246");
        CHECK(test123 + test123 == expected246);

        BigInt test999("999");
        BigInt expected1998("1998");
        CHECK(test999 + test999 == expected1998);

        BigInt expected2997("2997");
        BigInt test1998("1998");
        CHECK(test1998 + test999 == expected2997);
        CHECK(test999 + test1998 == expected2997);

        BigInt testBig("2147483847");
        BigInt expectedBig("4294967694");
        CHECK(testBig + testBig == expectedBig);
    }

    SECTION("int multiplication tests")
    {
        BigInt test123("123");
        BigInt test0;
        CHECK(test123 * 1 == test123);
        CHECK(test123 * 0 == test0);
        CHECK(test123 * 2 == BigInt("246"));
        CHECK(BigInt("999") * 4 == BigInt("3996"));
    }

    SECTION("BigInt multiplication tests")
    {
        BigInt test123("123");
        BigInt test1("1");
        BigInt test0;
        CHECK(test123 * test0 == test0);
        CHECK(test123 * test1 == test123);
    }
}
