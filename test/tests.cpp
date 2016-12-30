#include <catch.hpp>
#include <vector>

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

TEST_CASE("Comparison tests")
{
    SECTION("Equality tests")
    {
        CHECK(BigInt("0") == BigInt(0));
        CHECK(BigInt(0) == BigInt("0"));

        CHECK(BigInt("0") == BigInt("-0"));
        CHECK(BigInt("-0") == BigInt("0"));


        CHECK(BigInt("123") == BigInt("123"));
        CHECK(BigInt("-123") == BigInt("-123"));

        CHECK_FALSE(BigInt("123") == BigInt("0"));
        CHECK_FALSE(BigInt("0") == BigInt("123"));

        CHECK_FALSE(BigInt("-123") == BigInt("123"));
        CHECK_FALSE(BigInt("123") == BigInt("-123"));
    }

    SECTION("< tests")
    {
        CHECK(BigInt("0") < BigInt("123"));
        CHECK(BigInt("123") < BigInt("124"));
        CHECK(BigInt("-13") < BigInt("13"));
        CHECK(BigInt("-14") < BigInt("-13"));

        CHECK_FALSE(BigInt("123") < BigInt("0"));
    }

    SECTION("> tests")
    {
        CHECK(BigInt("123") > BigInt("0"));
        CHECK(BigInt("124") > BigInt("123"));
        CHECK(BigInt("-13") > BigInt("-14"));
        CHECK(BigInt("13") > BigInt("-13"));
        
        CHECK_FALSE(BigInt("0") > BigInt("123"));
    }

    SECTION("<= tests")
    {
        CHECK(BigInt("123") <= BigInt("123"));
        CHECK(BigInt("-123") <= BigInt("-123"));
        CHECK(BigInt("0") <= BigInt("0"));

        CHECK(BigInt("-11111") <= BigInt("1848"));
        CHECK(BigInt("-1033") <= BigInt("999"));
        CHECK(BigInt("123") <= BigInt("246"));

        CHECK(BigInt::abs(BigInt("-123")) <= BigInt("246"));
        CHECK_FALSE(BigInt::abs(BigInt("246")) <= 
                BigInt::abs(BigInt("-123")));
    }

    SECTION(">= tests")
    {
        CHECK(BigInt("123") >= BigInt("123"));
        CHECK(BigInt("-123") >= BigInt("-123"));
        CHECK(BigInt("0") >= BigInt("0"));

        CHECK(BigInt("1848") >= BigInt("-11111"));
        CHECK(BigInt("999") >= BigInt("-1033"));
        CHECK(BigInt("246") >= BigInt("123"));

        CHECK(BigInt::abs(BigInt("246")) >= BigInt::abs(BigInt("-123")));
        CHECK_FALSE(BigInt::abs(BigInt("-123")) >= 
                BigInt::abs(BigInt("246")));
    }

}

TEST_CASE("abs tests")
{
    SECTION("abs of a positive is the same BigInt")
    {
        CHECK(BigInt::abs(BigInt("13")) == BigInt("13"));
    }

    SECTION("abs of zero is zero")
    {
        CHECK(BigInt::abs(BigInt("0")) == BigInt("0"));
    }

    SECTION("abs of a negative")
    {
        CHECK(BigInt::abs(BigInt("-19382")) == BigInt("19382"));
    }
}

TEST_CASE("Addition tests")
{
    SECTION("0 is the additive identity")
    {
        CHECK(BigInt("123") + BigInt("0") == BigInt("123"));
        CHECK(BigInt("-123") + BigInt("0") == BigInt("-123"));
    }

    SECTION("Addition by additive inverse results in 0")
    {
        CHECK(BigInt("123") + BigInt("-123") == BigInt("0"));
        CHECK(BigInt("0") + BigInt("-0") == BigInt("0"));
    }

    SECTION("Addition of positive BigInts is correct")
    {
        CHECK(BigInt("123") + BigInt("123") == BigInt("246"));
        CHECK(BigInt("391") + BigInt("1700") == BigInt("2091"));
        CHECK(BigInt("1700") + BigInt("391") == BigInt("2091"));
        CHECK(BigInt("999") + BigInt("1998") == BigInt("2997"));
        CHECK(BigInt("2147483847") + BigInt("2147483847") == 
                BigInt("4294967694"));
    }

    SECTION("Addition of negative BigInts is correct")
    {
        CHECK(BigInt("-999") + BigInt("-999") == BigInt("-1998"));
        CHECK(BigInt("-999") + BigInt("-1") == BigInt("-1000"));
    }

    SECTION("Addition of positive and negative BigInts is correct")
    {
        CHECK(BigInt("246") + BigInt("-123") == BigInt("123"));
        CHECK(BigInt("1093") + BigInt("-999") == BigInt("94"));
        CHECK(BigInt("999") + BigInt("-1033") == BigInt("-34"));
        CHECK(BigInt("1848") + BigInt("-11111") == BigInt("-9263"));
        CHECK(BigInt("17") + BigInt("-20") == BigInt("-3"));
    }
}

TEST_CASE("Subtraction tests")
{
    SECTION("Subtraction by 0 changes nothing")
    {
        CHECK(BigInt("10") - BigInt("0") == BigInt("10"));
        CHECK(BigInt("-10") - BigInt("0") == BigInt("-10"));
    }

    SECTION("x - x = 0")
    {
        CHECK(BigInt("10") - BigInt("10") == BigInt("0"));
        CHECK(BigInt("-10") - BigInt("-10") == BigInt("0"));
    }

    SECTION("Positive big minus positive small")
    {
        CHECK(BigInt("123") - BigInt("119") == BigInt("4"));
        CHECK(BigInt("1000") - BigInt("1") == BigInt("999"));
        CHECK(BigInt("1000") - BigInt("100") == BigInt("900"));
    }

    SECTION("Positive small minus positive big")
    {
        CHECK(BigInt("17") - BigInt("20") == BigInt("-3"));
        CHECK(BigInt("100") - BigInt("1000") == BigInt("-900"));
        CHECK(BigInt("1") - BigInt("123") == BigInt("-122"));
    }

    SECTION("Negative small minus negative big")
    {
        CHECK(BigInt("-1") - BigInt("-100") == BigInt("99"));
        CHECK(BigInt("-123") - BigInt("-246") == BigInt("123"));
    }

    SECTION("Negative big minus negative small")
    {
        CHECK(BigInt("-100") - BigInt("-1") == BigInt("-99"));
        CHECK(BigInt("-246") - BigInt("-123") == BigInt("-123"));
    }

    SECTION("Positive minus negative")
    {
        CHECK(BigInt("123") - BigInt("-123") == BigInt("246"));
        CHECK(BigInt("100") - BigInt("-200") == BigInt("300"));
        CHECK(BigInt("1") - BigInt("-100") == BigInt("101"));
    }

    SECTION("Negative minus positive")
    {
        CHECK(BigInt("-1038") - BigInt("100") == BigInt("-1138"));
        CHECK(BigInt("-1") - BigInt("2") == BigInt("-3"));
        CHECK(BigInt("-14") - BigInt("234") == BigInt("-248"));
    }
}

TEST_CASE("int multiplication tests")
{
    SECTION("Multiplication by 0 is 0")
    {
        CHECK(BigInt("123") * 0 == BigInt("0"));
        CHECK(BigInt("-123") * 0 == BigInt("0"));
        CHECK(BigInt("0") * 0 == BigInt("0"));
    }

    SECTION("1 is the multiplicative identity")
    {
        CHECK(BigInt("123") * 1 == BigInt("123"));
        CHECK(BigInt("-123") * 1 == BigInt("-123"));
        CHECK(BigInt("0") * 1 == BigInt("0"));
    } 

    SECTION("Multiplication of positive by positive int")
    {
        CHECK(BigInt("123") * 2 == BigInt("246"));
        CHECK(BigInt("999") * 4 == BigInt("3996"));
        CHECK(BigInt("100") * 10 == BigInt("1000"));
        CHECK(BigInt("490") * 27 == BigInt("13230"));
    }

    SECTION("Multiplication of positive by negative int")
    {
        CHECK(BigInt("17") * -3 == BigInt(-51));
        CHECK(BigInt("123") * -123 == BigInt("-15129"));
        CHECK(BigInt("246") * -2 == BigInt("-492"));
    }

    SECTION("Multiplication of negative by negative int")
    {
        CHECK(BigInt("-17") * -3 == BigInt("51"));
        CHECK(BigInt("-1") * -1 == BigInt("1"));
        CHECK(BigInt("-134") * -12 == BigInt("1608"));
    }

    SECTION("Multiplication of negative by positive int")
    {
        CHECK(BigInt("-17") * 3 == BigInt(-51));
        CHECK(BigInt("-1") * 2 == BigInt("-2"));
        CHECK(BigInt("-134") * 12 == BigInt("-1608"));
    }
}

TEST_CASE("BigInt multiplication tests")
{
    SECTION("Multiplication by 0 returns 0")
    {
        CHECK(BigInt("123") * BigInt("0") == BigInt("0"));
        CHECK(BigInt("0") * BigInt("123") == BigInt("0"));
        CHECK(BigInt("-123") * BigInt("0") == BigInt("0"));
        CHECK(BigInt("0") * BigInt("-123") == BigInt("0"));
    }

    SECTION("1 is the multiplicative identity")
    {
        CHECK(BigInt("123") * BigInt("1") == BigInt("123"));
        CHECK(BigInt("1") * BigInt("123") == BigInt("123"));
        CHECK(BigInt("-123") * BigInt("1") == BigInt("123"));
        CHECK(BigInt("1") * BigInt("-123") == BigInt("123"));
    }

    SECTION("Multiplication of positive by positive")
    {
        CHECK(BigInt("123") * BigInt("123") == BigInt("15129"));
        CHECK(BigInt("17") * BigInt("123") == BigInt("2091"));
        CHECK(BigInt("123") * BigInt("17") == BigInt("2091"));
        CHECK(BigInt("10000000000") * BigInt("30000000000") * 2 == 
                BigInt("600000000000000000000"));
        CHECK(BigInt("30000000000") * BigInt("10000000000") * 2 == 
                BigInt("600000000000000000000"));
    }

    SECTION("Multiplication of positive by negative")
    {
        CHECK(BigInt("17") * BigInt("-3") == BigInt("-51"));
        CHECK(BigInt("123") * BigInt("-123") == BigInt("-15129"));
        CHECK(BigInt("1043") * BigInt("-12") ==  BigInt("-12516"));
        CHECK(BigInt("17") * BigInt("-1") == BigInt("-17"));
    }

    SECTION("Multiplication of negative by negative")
    {
        CHECK(BigInt("-17") * BigInt("-3") == BigInt("51"));
        CHECK(BigInt("-1") * BigInt("-1") == BigInt("1"));
        CHECK(BigInt("-100") * BigInt("-10") == BigInt("1000"));
    }

    SECTION("Multiplication of negative by positive")
    {
        CHECK(BigInt("-17") * BigInt("3") == BigInt("-51"));
        CHECK(BigInt("-100") * BigInt("10") == BigInt("-1000"));
        CHECK(BigInt("-123") * BigInt("123") == BigInt("-15129"));
    }
}

TEST_CASE("Exponentiation tests")
{
    SECTION("Valid exponents")
    {
        CHECK(BigInt("123").expt(BigInt("0")) == BigInt("1"));
        CHECK(BigInt("123").expt(BigInt("1")) == BigInt("123"));
        CHECK(BigInt("123").expt(BigInt("2")) == BigInt("15129"));
        CHECK(BigInt("100").expt(BigInt("10")) == 
                BigInt("100000000000000000000"));
    }

    SECTION("Invalid exponents")
    {
        CHECK_THROWS(BigInt("100").expt(BigInt(-3)));
    }
}
