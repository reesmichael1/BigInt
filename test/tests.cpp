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
        CHECK(test001.isNonNegative());

        BigInt test100("100");
        std::vector<int> expectedTest100 = {1, 0, 0};
        CHECK(test100.getVector() == expectedTest100);
        CHECK(test100.isNonNegative());

        BigInt testNeg100("-100");
        CHECK(testNeg100.getVector() == expectedTest100);
        CHECK_FALSE(testNeg100.isNonNegative());
        CHECK_FALSE(BigInt("-3").isNonNegative());
    }

    SECTION("Constructor without value tests")
    {
        BigInt test;
        std::vector<int> expectedTest = {0};
        CHECK(test.getVector() == expectedTest);
        CHECK(test.isNonNegative());
    }

    SECTION("Constructor with integer value tests")
    {
        std::vector<int> expectedTest100 = {1, 0, 0};
        std::vector<int> expectedTest9 = {9};
        CHECK(BigInt(100).getVector() == expectedTest100);
        CHECK(BigInt(-100).getVector() == expectedTest100);
        CHECK(BigInt(9).getVector() == expectedTest9);
        CHECK(BigInt(9039193) == BigInt("9039193"));
        CHECK(BigInt(100).isNonNegative());
        CHECK_FALSE(BigInt(-100).isNonNegative());
    }

    SECTION("Constructor with non-integer string fails")
    {
        CHECK_THROWS(BigInt test("123abc"));
    }
}

TEST_CASE("Arithmetic tests")
{
    SECTION("Comparison tests")
    {
        BigInt test0;
        BigInt testExplicit0("0");
        CHECK(test0 == testExplicit0);
        CHECK(testExplicit0 == test0);

        BigInt test123("123");
        CHECK(test123 == test123);

        CHECK_FALSE(test123 == test0);
        CHECK_FALSE(test0 == test123);

        CHECK_FALSE(BigInt("-123") == BigInt("123"));
        CHECK_FALSE(BigInt("123") == BigInt("-123"));

        CHECK(BigInt("0") < BigInt("123"));
        CHECK(BigInt("123") > BigInt("0"));
        CHECK(BigInt("123") < BigInt("124"));
        CHECK(BigInt("123") <= BigInt("123"));
        CHECK(BigInt("123") >= BigInt("123"));

        CHECK(BigInt("0") == BigInt("-0"));
        CHECK(BigInt("-0") == BigInt("0"));

        CHECK_FALSE(BigInt("0") > BigInt("123"));
        CHECK_FALSE(BigInt("123") < BigInt("0"));
        CHECK(BigInt("1848") >= BigInt("-11111"));
        CHECK(BigInt("999") >= BigInt("-1033"));

        CHECK(BigInt("-13") < BigInt("13"));
        CHECK(BigInt("-13") > BigInt("-14"));
        CHECK(BigInt("13") > BigInt("-13"));
        CHECK(BigInt("-14") < BigInt("-13"));
    }

    SECTION("abs tests")
    {
        CHECK(BigInt::abs(BigInt("13")) == BigInt("13"));
        CHECK(BigInt::abs(BigInt("0")) == BigInt("0"));
        CHECK(BigInt::abs(BigInt("-19382")) == BigInt("19382"));
    }

    SECTION("Addition tests")
    {
        BigInt test123("123");
        BigInt test0;
        CHECK(test123 + test0 == test123);
        CHECK(test0 + test123 == test123);
        
        BigInt testNeg123("-123");
        CHECK(test123 + testNeg123 == test0);

        BigInt expected246 = BigInt("246");
        CHECK(test123 + test123 == expected246);

        CHECK(BigInt("246") + testNeg123 == test123);

        CHECK(BigInt("-999") + BigInt("-999") == BigInt("-1998"));
        CHECK(BigInt("1093") + BigInt("-999") == BigInt("94"));
        CHECK(BigInt("999") + BigInt("-1033") == BigInt("-34"));
        CHECK(BigInt("1848") + BigInt("-11111") == BigInt("-9263"));

        BigInt test999("999");
        BigInt expected1998("1998");
        CHECK(test999 + test999 == expected1998);

        CHECK(BigInt("391") + BigInt("1700") == BigInt("2091"));
        CHECK(BigInt("1700") + BigInt("391") == BigInt("2091"));

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
        CHECK(BigInt("100") * 10 == BigInt("1000"));
        CHECK(BigInt("490") * 27 == BigInt("13230"));
        CHECK(BigInt("-17") * -3 == BigInt("51"));
        CHECK(BigInt("-17") * 3 == BigInt(-51));
        CHECK(BigInt("17") * -3 == BigInt(-51));
    }

    SECTION("BigInt multiplication tests")
    {
        BigInt test123("123");
        BigInt test1("1");
        BigInt test15129("15129");
        BigInt test17("17");
        BigInt test3("3");
        BigInt testNeg3("-3");
        BigInt testNeg17("-17");
        BigInt test51("51");
        BigInt testNeg51("-51");
        BigInt test2091("2091");
        BigInt test0;
        CHECK(test123 * test0 == test0);
        CHECK(test0 * test123 == test0);
        CHECK(test123 * test1 == test123);
        CHECK(test1 * test123 == test123);
        CHECK(test123 * test123 == test15129);
        CHECK(test17 * test123 == test2091);
        CHECK(test123 * test17 == test2091);
        CHECK(BigInt("-17") * BigInt("-3") == BigInt("51"));
        CHECK(BigInt("-17") * BigInt("3") == BigInt("-51"));
        CHECK(BigInt("17") * BigInt("-3") == BigInt("-51"));
        CHECK(testNeg17 * testNeg3 == test51);
        CHECK(testNeg17 * test3 == testNeg51);
        CHECK(test17 * testNeg3 == testNeg51);
        CHECK(BigInt("10000000000") * BigInt("30000000000") * 2 == 
                BigInt("600000000000000000000"));
        CHECK(BigInt("30000000000") * BigInt("10000000000") * 2 == 
                BigInt("600000000000000000000"));
    }

    SECTION("Exponentiation tests")
    {
        CHECK(BigInt("123").expt(BigInt("0")) == BigInt("1"));
        CHECK(BigInt("123").expt(BigInt("1")) == BigInt("123"));
        CHECK(BigInt("123").expt(BigInt("2")) == BigInt("15129"));
        CHECK(BigInt("100").expt(BigInt("10")) == 
                BigInt("100000000000000000000"));
        CHECK_THROWS(BigInt("100").expt(BigInt(-3)));
    }
}
