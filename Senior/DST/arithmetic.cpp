#include <bits/stdc++.h>

using namespace std;

class BigDecimal
{
    bool negative;
    // A linked list representing the integer part of the BigDecimal
    vector<char> integer;
    // A linked list representing the decimal part of the BigDecimal
    vector<char> decimal;

public:
    /**
     * Construct a new BigDecimal object with no parameter
     */
    BigDecimal() {}

    /**
     * Construct a new BigDecimal object with parameter
     * @param data data to put in to the BigDecimal, eg: "123.456" then 123 should be put in at integer and 456 should be put in at decimal
     */
    BigDecimal(string data)
    {
        int cur;
        if (data[0] == '-')
        {
            negative = true;
            cur = 1;
        }
        else
        {
            negative = false;
            cur = 0;
        }

        while (data[cur] != '.' && cur < data.length())
        {
            integer.push_back(data[cur]);
            cur++;
        }

        while (cur < data.length())
        {
            decimal.push_back(data[cur]);
            cur++;
        }
    }

    /**
     * Overload the operator + to add two BigDecimal
     * @param bigDecimal BigDecimal to add
     *
     * Rounding rule:
     *  Banker's rounding to 2nd decimal place
     */
    BigDecimal *operator+(BigDecimal *bigDecimal)
    {
    }

    /**
     * Overload the operator - to subtract two BigDecimal
     * @param bigDecimal BigDecimal to subtract
     *
     * Rounding rule:
     *  Banker's rounding to 2nd decimal place
     */
    BigDecimal *operator-(BigDecimal *bigDecimal) {}

    /**
     * Overload the operator * to multiply two BigDecimal
     * @param bigDecimal BigDecimal to multiply
     *
     * Rounding rule:
     *  Banker's rounding to 2nd decimal place
     */
    BigDecimal *operator*(BigDecimal *bigDecimal) {}

    /**
     * Overload the operator / to divide two BigDecimal
     * @param bigDecimal BigDecimal to divide
     *
     * Rounding rule:
     *  Banker's rounding to 2nd decimal place
     */
    BigDecimal *operator/(BigDecimal *bigDecimal) {}

    /**
     * Overload the operator ^ to power two BigDecimal
     * @param bigDecimal BigDecimal to power
     *
     * Rounding rule:
     *  Banker's rounding to 2nd decimal place
     */
    BigDecimal *operator^(BigDecimal *bigDecimal) {}

    bool operator>(BigDecimal *bigDecimal)
    {
        if (!this->getNegative() && bigDecimal->getNegative())
            return true;
        else if (this->getNegative() && !bigDecimal->getNegative())
            return false;

        vector<char> integer1 = this->getInteger();
        vector<char> integer2 = bigDecimal->getInteger();
        vector<char> decimal1 = this->getDecimal();
        vector<char> decimal2 = bigDecimal->getDecimal();

        if (integer1.size() > integer2.size())
        {
            return true;
        }
        else if (integer1.size() < decimal2.size())
        {
            return false;
        }
        else
        {
            for (int i = 0; i < integer1.size(); i++)
            {
                if (integer1[i] > integer2[i])
                    return true;
                else if (integer1[i] < integer2[i])
                    return false;
            }

            int size = decimal1.size() < decimal2.size() ? decimal1.size() : decimal2.size();
            for (int i = 0; i < size; i++)
            {
                if (decimal1[i] > decimal2[i])
                    return true;
                else if (decimal1[i] < decimal2[i])
                    return false;
            }
            return decimal1.size() > decimal2.size();
        }
    }
    /**
     * Overload the operator << to print out the BigDecimal to 2nd decimal place
     *
     * Output example:
     * If your integer is 1234 and decimal is 56 then you should print out "1234.56" with the content in quotes but without the quotes and a \n at the end
     */
    friend std::ostream &operator<<(std::ostream &out, BigDecimal *bigDecimal)
    {
        if (bigDecimal->getNegative())
            out << "-";

        // bigDecimal->rounding();
        vector<char> integer = bigDecimal->getInteger();
        vector<char> decimal = bigDecimal->getDecimal();

        for (int i = 0; i < integer.size(); i++)
            out << integer[i];

        for (int i = 0; i < decimal.size(); i++)
            out << decimal[i];
        return out;
    }

    friend std::istream &operator>>(std::istream &in, BigDecimal *data)
    {
        string tmp;
        in >> tmp;

        int cur;
        if (tmp[0] == '-')
        {
            data->setNegative(true);
            cur = 1;
        }
        else
        {
            data->setNegative(false);
            cur = 0;
        }

        vector<char> integer;
        vector<char> decimal;

        while (tmp[cur] != '.' && cur < tmp.length())
        {
            integer.push_back(tmp[cur]);
            cur++;
        }

        while (cur < tmp.length())
        {
            decimal.push_back(tmp[cur]);
            cur++;
        }

        data->setInteger(integer);
        data->setDecimal(decimal);
        return in;
    }

public:
    vector<char> getInteger()
    {
        return this->integer;
    }

    vector<char> getDecimal()
    {
        return this->decimal;
    }

    bool getNegative()
    {
        return this->negative;
    }

    void setInteger(vector<char> vec)
    {
        this->integer = vec;
    }

    void setDecimal(vector<char> vec)
    {
        this->decimal = vec;
    }

    void setNegative(bool boolean)
    {
        this->negative = boolean;
    }
};

int main()
{
    BigDecimal *bigDecimal1 = new BigDecimal();
    BigDecimal *bigDecimal2 = new BigDecimal();
    char operation;
    while (cin >> bigDecimal1 >> operation >> bigDecimal2)
    {
        cout << (*bigDecimal1 > bigDecimal2) << endl;

        // switch (operation)
        // {
        // case '+':
        //     cout << (*bigDecimal1 + bigDecimal2);
        //     break;
        // case '-':
        //     cout << (*bigDecimal1 - bigDecimal2);
        //     break;
        // case '*':
        //     cout << (*bigDecimal1 * bigDecimal2);
        //     break;
        // case '/':
        //     cout << (*bigDecimal1 / bigDecimal2);
        //     break;
        // case '^':
        //     cout << (*bigDecimal1 ^ bigDecimal2);
        //     break;
        // default:
        //     break;
        // }
    }
    return 0;
}
