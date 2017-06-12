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

    std::string firstChar = intString.substr(0, 1);
    if (firstChar.compare("-") == 0)
    {
        nonNegative = false;
        intString.erase(0, 1);
    }
    else
        nonNegative = true;

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
 * Construct a BigInt from an integer \a intToInt.
 *
 * This BigInt will have the same value as \a intToInt.
*/

BigInt::BigInt(int intToInt)
{
    intVector = BigInt(std::to_string(intToInt)).intVector;
    if (intToInt < 0)
        nonNegative = false;
    else
        nonNegative = true;

    normalize();
}

/*!
 * Construct a BigInt with no given initial value.
 * In this case, the BigInt is initialized to 0.
*/

BigInt::BigInt()
{
    intVector = {0};
    nonNegative = true;
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

BigInt BigInt::addTwoNegatives(BigInt bi1, BigInt bi2)
{
    bi1.nonNegative = true;
    bi2.nonNegative = true;

    return (bi1 + bi2) * -1;
}

BigInt BigInt::addTwoPositives(BigInt bi1, BigInt bi2)
{
    BigInt sum;
    sum.nonNegative = true;

    std::vector<int> sumVector;

    std::vector<int> shortVector;
    std::vector<int> longVector;
    if (bi1.intVector.size() > bi2.intVector.size())
    {
        shortVector = bi2.intVector;
        longVector = bi1.intVector;
    }
    else
    {
        shortVector = bi1.intVector;
        longVector = bi2.intVector;
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
    sum.nonNegative = true;

    return sum.normalize();
}

BigInt BigInt::addNegativeToPositive(BigInt positive, BigInt negative)
{
    BigInt result;
    std::vector<int> resultVector;
    int carry = 0;
    int nextTerm;

    if (BigInt::abs(positive) >= BigInt::abs(negative) || 
            positive.intVector.size() != negative.intVector.size())
    {
        std::vector<int> longVector;
        std::vector<int> shortVector;

        if (positive.intVector.size() >= negative.intVector.size())
        {
            longVector = positive.intVector;
            shortVector = negative.intVector;
        }
        else
        {
            longVector = negative.intVector;
            shortVector = positive.intVector;
        }

        int longVectorMaxIndex = longVector.size() - 1;
        int shortVectorMaxIndex = shortVector.size() - 1;

        for (int i = shortVectorMaxIndex; i >= 0; i--)
        {
            nextTerm = longVector.at(longVectorMaxIndex 
                    - shortVectorMaxIndex + i) - 
                shortVector.at(i) + carry;

            if (nextTerm < 0)
            {
                carry = -1;
                nextTerm += 10;
            }
            else
                carry = 0;

            resultVector.insert(resultVector.begin(), nextTerm);
        }

        for (int i = longVectorMaxIndex - shortVectorMaxIndex - 1; i >= 0; i--)
        {
            nextTerm = longVector.at(i) + carry;
            if (nextTerm < 0)
            {
                carry = -1;
                nextTerm += 10;
            }
            else
            {
                carry = 0;
            }

            resultVector.insert(resultVector.begin(), nextTerm);
        }

        if (carry != 0)
            resultVector.insert(resultVector.begin(), 1);

        result.intVector = resultVector;
        result.nonNegative = 
            (BigInt::abs(positive) >= BigInt::abs(negative));

        return result.normalize();
    }

    for (int i = positive.intVector.size() - 1; i >= 0; i--)
    {
        nextTerm = negative.intVector.at(i) - positive.intVector.at(i) 
            + carry;
        if (nextTerm < 0)
        {
            carry = -1;
            nextTerm += 10;
        }
        else
            carry = 0;

        resultVector.insert(resultVector.begin(), nextTerm);
    }

    result.intVector = resultVector;
    result.nonNegative = (BigInt::abs(positive) >= BigInt::abs(negative));

    return result.normalize();

}

int getNumberOfDigitsInInt(int i)
{
    float logOfInt = log10(i);
    if (floor(logOfInt) == logOfInt)
        return logOfInt + 1;
    return ceil(logOfInt);
}

/*! 
 * Check that two BigInts are equal.
 *
 * Two BigInts are said to be equal if and only if their digits are the same.
*/

bool BigInt::operator==(const BigInt& bi) const
{
    // For zero, we don't care about the sign
    std::vector<int> zeroVector = {0};
    if (intVector == zeroVector && bi.intVector == zeroVector)
        return true;
    return (intVector == bi.intVector && isNonNegative() == bi.nonNegative);
}

/*! 
 * Decide if one BigInt is less than another.
 *
 * Return true if this BigInt is less than \a bi.
 */

bool BigInt::operator< (const BigInt& bi) const
{
    if (this->nonNegative && bi.nonNegative)
    {
        BigInt comparison = bi;
        BigInt normalizedComparison = comparison.normalize();

        if (normalizedComparison.intVector.size() < 
                this->intVector.size())
            return false;
        if (normalizedComparison.intVector.size() >
                this->intVector.size())
            return true;

        for (unsigned int i = 0; i < this->intVector.size(); i++)
        {
            if (this->intVector.at(i) > 
                    normalizedComparison.intVector.at(i))
                return false;
            else if (this->intVector.at(i) < 
                    normalizedComparison.intVector.at(i))
                return true;
        }

        // The two BigInts are equal
        return false;
    }
    else if (!(this->nonNegative) && !bi.nonNegative)
    {
        return (BigInt::abs(bi) < BigInt::abs(*this));
    }
    else if (this->nonNegative && !bi.nonNegative)
        return false;
    return true;
}

/*! 
 * Decide if one BigInt is greater than another.
 *
 * Return true if this BigInt is greater than \a bi.
 */

bool BigInt::operator> (const BigInt& bi) const
{
    return !(*this < bi || *this == bi);
}

/*! 
 * Decide if one BigInt is less than or equal to another.
 *
 * Return true if this BigInt is less than or equal to \a bi.
 */

bool BigInt::operator<=(const BigInt& bi) const
{
    return (*this < bi || *this == bi);
}

/*! 
 * Decide if one BigInt is greater than or equal to another.
 *
 * Return true if this BigInt is greater than or equal to \a bi.
 */

bool BigInt::operator>=(const BigInt& bi) const
{
    return ((*this > bi) || *this == bi);
}

BigInt BigInt::normalize()
{
    while (intVector.at(0) == 0 && intVector.size() > 1)
        intVector.erase(intVector.begin(), intVector.begin() + 1);
    return *this;
}

/*!
 * Raise one BigInt to the exponent given by \a power.
*/

BigInt BigInt::expt(const BigInt &power)
{
    BigInt exponentInt = *this;
    if (!power.nonNegative)
        throw ("expt only accepts non-negative values");
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

bool BigInt::isNonNegative() const
{
    return nonNegative;
}

BigInt BigInt::abs(const BigInt bi) 
{
    BigInt calculatedAbs = bi;
    calculatedAbs.nonNegative = true;

    return calculatedAbs;
}

/*!
 * Add two BigInts.
 *
 * This constructs a new BigInt whose value is the sum
 * of the self BigInt and the given \a bi.
*/
BigInt operator+(const BigInt& b1, const BigInt& b2)
{
    BigInt lhs = b1;
    BigInt rhs = b2;
    if (!lhs.nonNegative && !rhs.nonNegative)
        return BigInt::addTwoNegatives(lhs, rhs);
    if (lhs.nonNegative && rhs.nonNegative)
        return BigInt::addTwoPositives(lhs, rhs);
    if (lhs.nonNegative && !rhs.nonNegative)
        return BigInt::addNegativeToPositive(lhs, rhs);
    return BigInt::addNegativeToPositive(rhs, lhs);
}

BigInt operator+(const BigInt& bi, const int& i)
{
    return BigInt(i) + bi;
}

/* !
 * Implement division between two BigInts.
 *
 * This constructs a new BigInt whose value is the quotient of the self
 * BigInt and the given \a divisor.
*/
BigInt operator/(const BigInt& b1, const BigInt& b2)
{
    BigInt dividend = b1;
    BigInt divisor = b2;

    // Simple base cases
    if (divisor == 1)
        return dividend;

    if (divisor == -1)
        return -1 * dividend;

    // In this case, return 0
    if (BigInt::abs(divisor) > BigInt::abs(dividend))
        return BigInt();

    if (divisor == 0)
        throw("Attempt to divide by zero");

    BigInt quotientInt;

    divisor = BigInt::abs(divisor);
    dividend = BigInt::abs(dividend);

    // Find the first chunk of digits in the dividend larger than the divisor
    int counter = 0;
    BigInt smallInt(dividend.intVector.at(counter));

    while (smallInt < divisor)
    {
        counter++;
        smallInt.intVector.push_back(dividend.intVector.at(counter));
    }

    // Figure out what digit we need to multiply the divisor by 
    // to get the closest to smallInt
    int i = 1;
    for (; i < 10; i++)
    {
        if ((i + 1) * divisor > smallInt)
            break;

        else if (i * divisor == smallInt)
            break;
    }

    quotientInt.intVector.push_back(i);

    // Find out how many powers of ten to multiply by
    int powersOfTen = dividend.intVector.size() - counter - 1;
    quotientInt = quotientInt * pow(10, powersOfTen);

    BigInt remainder = dividend - (divisor * quotientInt);
    quotientInt = quotientInt.normalize() + remainder / divisor;
    quotientInt.nonNegative = !(b1.nonNegative ^ b2.nonNegative);
    return quotientInt;
}

BigInt operator/(const BigInt& bi, const int& i)
{
    return bi / BigInt(i);
}

/*! 
 * Implement subtraction between two BigInts.
 *
 * This constructs a new BigInt whose value is the difference between
 * the self BigInt and the given \a b1.
*/

BigInt operator-(const BigInt& b1, const BigInt& b2)
{
    BigInt negative = b2;
    negative.nonNegative = !negative.nonNegative;
    return b1 + negative;
}

/*!
 * Implement multiplication between BigInts.
 *
 * Returns the product of the integers represented by \a b1 and \a b2.
*/

BigInt operator*(const BigInt& b1, const BigInt& b2)
{
    BigInt productInt;

    unsigned int factor1Length = b2.intVector.size();
    int currentDigitCounter = factor1Length - 1;

    while (currentDigitCounter >= 0)
    {
        int currentPowerOf10 = (factor1Length - 1) - currentDigitCounter;
        int currentDigit = b2.intVector.at(currentDigitCounter);

        BigInt currentTerm = b1 * currentDigit;
        currentTerm = currentTerm.pow10(currentPowerOf10);
        productInt = productInt + currentTerm;

        currentDigitCounter -= 1;
    }

    productInt.nonNegative = !(b1.nonNegative ^ b2.nonNegative);

    return productInt.normalize();
}

BigInt BigInt::multiplyByDigit(const BigInt& bi, int i)
{
    BigInt product;

    std::vector<int> productIntVector;
    int indexMax = bi.intVector.size() - 1;
    
    for (unsigned int j = 0; j < bi.intVector.size(); j++)
    {
        productIntVector.insert(productIntVector.begin(), 
                                    bi.intVector.at(indexMax - j) * i);
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

/*!  
 * Implement usual integer multiplication of BigInts.
 *
 * Note that \a i is a usual C++ int, not a BigInt.
*/

BigInt operator*(const BigInt& bi, const int& i)
{
    BigInt productInt;

    int factor = i;

    bool multiplyingByNegative;

    if (factor < 0)
    {
        multiplyingByNegative = true;
        factor *= -1;
    }
    else
        multiplyingByNegative = false;

    int numberOfDigits = getNumberOfDigitsInInt(factor);

    for (int j = 0; j < numberOfDigits; j++)
    {
        int currentDigit = 
            floor((factor % (int)pow(10, j + 1)) / pow(10, j));
        BigInt currentTerm = BigInt::multiplyByDigit(bi, currentDigit);
        
        currentTerm = currentTerm.pow10(j);
        productInt = productInt + currentTerm;
    }

    if (multiplyingByNegative)
    {
        if (bi.nonNegative)
            productInt.nonNegative = false;
        else
            productInt.nonNegative = true;
    }
    else if (!bi.nonNegative)
        productInt.nonNegative = false;

    return productInt.normalize();
}

std::ostream& operator<<(std::ostream& os, const BigInt& bi)
{
    std::string representation = "";
    for (int i : bi.intVector)
    {
        representation.append(std::to_string(i));
    }

    if (!bi.nonNegative)
        representation.insert(0, "-");

    os << representation;
    return os;
}
