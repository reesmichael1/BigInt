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
        BigInt expt(const BigInt &power);
        std::vector<int> getVector();
        operator std::string()
        {
            std::string representation = "";
            for (int i : intVector)
            {
                representation.append(std::to_string(i));
            }

            return representation;
        };
        operator char*() const
        {
            std::string representation = "";
            for (int i : intVector)
            {
                representation.append(std::to_string(i));
            }

            char* charRep = new char[representation.size() + 1];
            std::copy(representation.begin(), representation.end(), charRep);
            charRep[representation.size()] = '\0';
            return charRep;
        };
        BigInt operator+(const BigInt&);
        BigInt operator*(const BigInt&);
        BigInt operator*(const int&);
        bool operator==(const BigInt&);

    private:
        std::vector<int> intVector;
        BigInt multiplyByDigit(int i);
        BigInt normalize();
};

#endif
