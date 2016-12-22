#include <cmath>

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

BigInt::BigInt(int intToInt)
{
    intVector = BigInt(std::to_string(intToInt)).intVector;
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

    return sum.normalize();
}

/*!
 * Implement multiplication between BigInts.
 *
 * Returns the product of the integers represented by this BigInt and \a bi.
*/

BigInt BigInt::operator*(const BigInt& bi)
{
    BigInt productInt;

    unsigned int factor1Length = bi.intVector.size();
    int currentDigitCounter = factor1Length - 1;

    while (currentDigitCounter >= 0)
    {
        int currentPowerOf10 = (factor1Length - 1) - currentDigitCounter;
        int currentDigit = bi.intVector.at(currentDigitCounter);

        BigInt currentTerm = *this * currentDigit;
        currentTerm = currentTerm.pow10(currentPowerOf10);
        productInt = productInt + currentTerm;

        currentDigitCounter -= 1;
    }

    return productInt.normalize();
}

BigInt BigInt::multiplyByDigit(int i)
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

    product.intVector = productIntVector;
    return product.normalize();
}

int getNumberOfDigitsInInt(int i)
{
    float logOfInt = log10(i);
    if (floor(logOfInt) == logOfInt)
        return logOfInt + 1;
    return ceil(logOfInt);
}

/*!  
 * Implement usual integer multiplication of BigInts.
 *
 * Note that \a i is a usual C++ int, not a BigInt.
*/

BigInt BigInt::operator*(const int& i)
{
    BigInt productInt;

    int numberOfDigits = getNumberOfDigitsInInt(i);

    for (int j = 0; j < numberOfDigits; j++)
    {
        int currentDigit = floor((i % (int)pow(10, j + 1)) / pow(10, j));
        BigInt currentTerm = this->multiplyByDigit(currentDigit);
        
        currentTerm = currentTerm.pow10(j);
        productInt = productInt + currentTerm;
    }

    return productInt.normalize();
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

/* 
 * Decide if one BigInt is less than another.
 *
 * Return true if this BigInt is less than \a bi.
 */

bool BigInt::operator< (const BigInt& bi)
{
    BigInt normalizedThis = this->normalize();
    BigInt comparison = bi;
    BigInt normalizedComparison = comparison.normalize();

    if (normalizedComparison.intVector.size() < 
            normalizedThis.intVector.size())
        return false;
    if (normalizedComparison.intVector.size() >
            normalizedThis.intVector.size())
        return true;

    for (unsigned int i = 0; i < normalizedThis.intVector.size(); i++)
    {
        if (normalizedThis.intVector.at(i) > 
                normalizedComparison.intVector.at(i))
            return false;
        else if (normalizedThis.intVector.at(i) < 
                normalizedComparison.intVector.at(i))
            return true;
    }

    // The two BigInts are equal
    return false;
}

/* 
 * Decide if one BigInt is greater than another.
 *
 * Return true if this BigInt is greater than \a bi.
 */

bool BigInt::operator> (const BigInt& bi)
{
    return !(*this < bi || *this == bi);
}

/* 
 * Decide if one BigInt is less than or equal to another.
 *
 * Return true if this BigInt is less than or equal to \a bi.
 */

bool BigInt::operator<=(const BigInt& bi)
{
    return (*this < bi || *this == bi);
}

/* 
 * Decide if one BigInt is greater than or equal to another.
 *
 * Return true if this BigInt is greater than or equal to \a bi.
 */

bool BigInt::operator>=(const BigInt& bi)
{
    return (!(bi > *this) || *this == bi);
}

BigInt BigInt::normalize()
{
    while (intVector.at(0) == 0 && intVector.size() > 1)
        intVector.erase(intVector.begin(), intVector.begin() + 1);
    return *this;
}

/*
 * Raise one BigInt to the exponent given by \a power.
*/

BigInt BigInt::expt(const BigInt &power)
{
    BigInt exponentInt = *this;
    if (power.intVector.at(0) == 0 && power.intVector.size() == 1)
        return BigInt("1");

    BigInt powerCount("1");
    while (!(powerCount == power))
    {
        exponentInt = exponentInt * *this;
        powerCount = powerCount + BigInt("1");
    }
    return exponentInt;
}

BigInt BigInt::pow10(int power)
{
    for (int j = 0; j < power; j++)
        this->intVector.push_back(0);

    return *this;
}
