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
        operator char*() const
        {
            std::string representation = "";
            for (int i : intVector)
            {
                representation.append(std::to_string(i));
            }

            if (!nonNegative)
                representation.insert(0, "-");

            char* charRep = new char[representation.size() + 1];
            std::copy(representation.begin(), 
                    representation.end(), charRep);
            charRep[representation.size()] = '\0';
            return charRep;
        };
        BigInt operator+(const BigInt&);
        BigInt operator*(const BigInt&);
        BigInt operator*(const int&);
        static BigInt abs(BigInt bi);
        bool operator==(const BigInt&);
        bool operator< (const BigInt&);
        bool operator> (const BigInt&);
        bool operator<=(const BigInt&);
        bool operator>=(const BigInt&);
        bool isNonNegative();

    private:
        std::vector<int> intVector;
        BigInt multiplyByDigit(int i);
        BigInt normalize();
        BigInt pow10(int power);
        static BigInt addTwoNegatives(BigInt bi1, BigInt bi2);
        static BigInt addTwoPositives(BigInt bi1, BigInt bi2);
        static BigInt addNegativeToPositive(BigInt positive, 
                BigInt negative);
        bool nonNegative;
};

#endif
