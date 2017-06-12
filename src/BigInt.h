#ifndef BIGINT_H
#define BIGINT_H

#include <vector>
#include <string>

/*!
 * \class BigInt
 *
 * \brief A class to handle large integers in C++.
 *
 * BigInt implements integers of arbitrary length and arithmetic on 
 * those integers. It can be used identically to an int, although
 * using any operation between a BigInt and an int will produce a BigInt.
*/

class BigInt
{
    public:
        BigInt();
        BigInt(std::string stringToInt);
        BigInt(int intToInt);
        BigInt expt(const BigInt &power);
        std::vector<int> getVector();
        operator std::string()
        {
            std::string representation = "";
            for (int i : intVector)
            {
                representation.append(std::to_string(i));
            }

            if (!nonNegative)
                representation.insert(0, "-");

            return representation;
        };
        friend std::ostream& operator << (std::ostream& os, const BigInt&);
        friend BigInt operator+(const BigInt& b1, const BigInt& b2);
        friend BigInt operator+(const BigInt& bi, const int& i);
        friend BigInt operator*(const BigInt& b1, const BigInt& b2);
        friend BigInt operator*(const BigInt& bi, const int& i);
        friend BigInt operator-(const BigInt& b1, const BigInt& b2);
        friend BigInt operator/(const BigInt& dividend, const BigInt& 
                divisor);
        friend BigInt operator/(const BigInt& dividend, const int& divisor);
        static BigInt abs(const BigInt bi);
        bool operator==(const BigInt&) const;
        bool operator< (const BigInt&) const;
        bool operator> (const BigInt&) const;
        bool operator<=(const BigInt&) const;
        bool operator>=(const BigInt&) const;
        bool isNonNegative() const;

    private:
        std::vector<int> intVector;
        static BigInt multiplyByDigit(const BigInt& bi, int i);
        BigInt pow10(int power);
        static BigInt addTwoNegatives(BigInt bi1, BigInt bi2);
        static BigInt addTwoPositives(BigInt bi1, BigInt bi2);
        static BigInt addNegativeToPositive(BigInt positive, 
                BigInt negative);
        bool nonNegative;
        BigInt normalize();
};

#endif


