#include <cmath>
#include <iostream>

#include "BigInt.h"

/*! 
 * Construct a BigInt instance from a std::string \a intString.
 *
 * @param intString An integer string. The value of the constructed 
 *  BigInt will be the value of the string.
*/

BigInt::BigInt(std::string intString)
{
    bool hitFirstNonZeroDigit = false;

    for (unsigned int i = 0; i < intString.length(); i++)
    {
        std::string c = intString.substr(i,1);
        if (isdigit(c[0]))
        {
            int nextDigit = std::stoi(c);
            if (hitFirstNonZeroDigit || nextDigit > 0)
            {
                intVector.push_back(nextDigit);
                if (!hitFirstNonZeroDigit)
                    hitFirstNonZeroDigit = true;
            }
        }
        else
            throw("Non-integer string given to constructor. Giving up.");
    } 

    if (intVector.size() == 0)
        intVector = {0};
}

/*!
 * Construct a BigInt with no given initial value.
 * In this case, the BigInt is initialized to 0.
*/

BigInt::BigInt()
{
    intVector = {0};
}

/*! 
 * Return the internal vector representation of the int.
 *
 * This should generally not be accessed. It exists for testing only.
*/

std::vector<int> BigInt::getVector()
{
    return intVector;
}

/*!
 * Add two BigInts.
 *
 * This constructs a new BigInt whose value is the sum
 * of the self BigInt and the given \a bi.
*/
BigInt BigInt::operator+(const BigInt& bi)
{
    BigInt sum;

    std::vector<int> sumVector;

    std::vector<int> shortVector;
    std::vector<int> longVector;
    if (bi.intVector.size() > intVector.size())
    {
        shortVector = intVector;
        longVector = bi.intVector;
    }
    else
    {
        shortVector = bi.intVector;
        longVector = intVector;
    }

    unsigned int longMaxIndex = longVector.size() - 1;
    unsigned int shortMaxIndex = shortVector.size() - 1;

    unsigned int i = 0;

    int carry = 0;

    while (i <= shortMaxIndex)
    {
        int nextTerm = longVector.at(longMaxIndex - i) + 
                shortVector.at(shortMaxIndex - i) + carry;

        if (nextTerm < 10)
        {
            sumVector.insert(sumVector.begin(), nextTerm);
            carry = 0;
        }
        else
        {
            carry = 1;
            sumVector.insert(sumVector.begin(), nextTerm - 10);
        }
        
        i++;
    }

    while (i <= longMaxIndex)
    {
        int nextTerm = longVector.at(longMaxIndex - i) + carry;
        if (nextTerm < 10)
        {
            sumVector.insert(sumVector.begin(), nextTerm);
            carry = 0;
        }
        else
        {
            sumVector.insert(sumVector.begin(), nextTerm - 10);
            carry = 1;
        }
        i++;
    }

    if (carry == 1)
    {
        sumVector.insert(sumVector.begin(), 1);
    }

    sum.intVector = sumVector;

    return sum;
}

/*!
 * Implement multiplication between BigInts.
 *
 * Returns the product of the integers represented by this BigInt and \a bi.
*/

BigInt BigInt::operator*(const BigInt& bi)
{
    return *this;
}

/*!  
 * Implement usual integer multiplication of BigInts.
 *
 * Note that \a i is a usual C++ int, not a BigInt.
*/

BigInt BigInt::operator*(const int& i)
{
    BigInt product;

    std::vector<int> productIntVector;
    int indexMax = intVector.size() - 1;
    
    for (unsigned int j = 0; j < intVector.size(); j++)
    {
        productIntVector.insert(productIntVector.begin(), 
                                    intVector.at(indexMax - j) * i);
    }

    int j = 0;
    int carry = 0;

    while (j <= indexMax)
    {
        int currentTerm = productIntVector.at(indexMax - j) + carry;
        if (currentTerm > 10)
        {
            carry = floor(currentTerm / 10);
            productIntVector.at(indexMax - j) = currentTerm - 10 * carry;
        }
        else
        {
            carry = 0;
            productIntVector.at(indexMax - j) = currentTerm;
        }
        j++;
    }

    if (carry > 0)
        productIntVector.insert(productIntVector.begin(), carry);

    while (productIntVector.at(0) == 0 && productIntVector.size() > 1)
        productIntVector.erase(productIntVector.begin(), productIntVector.begin() + 1);

    product.intVector = productIntVector;
    return product;
}

/*! 
 * Check that two BigInts are equal.
 *
 * Two BigInts are said to be equal if and only if their digits are the same.
*/

bool BigInt::operator==(const BigInt& bi)
{
    return (intVector == bi.intVector);
}
