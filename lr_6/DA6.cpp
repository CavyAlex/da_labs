#include <vector>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <ctime>
const int R = 4;
const int BASE = pow(10, R);

class TBigInt {
public:
    TBigInt() { }
    TBigInt(size_t n) { mData.resize(n); }
    TBigInt(const std::string & str);
    ~TBigInt() {}

    inline size_t size() const { return mData.size(); }
    inline int get(size_t i) const { return mData[i]; }
    void set(size_t i, int value) { mData[i] = value; }
    TBigInt(std::vector<int> & data);
    friend std::ostream& operator<< (std::ostream& stream, const TBigInt & number);

    TBigInt operator+(const TBigInt & other);
    TBigInt operator-(const TBigInt & other);
    TBigInt operator/(const TBigInt & other);
    TBigInt operator/(const int & other);
    TBigInt operator*(const TBigInt & other);
    TBigInt operator^(const int & other);
    bool operator<(const TBigInt & other);
    bool operator<=(const TBigInt & other);
    bool operator==(const TBigInt & other);
protected:
    std::vector<int> mData;
private:
    void DeleteZeros();
};

void TBigInt::DeleteZeros() {
    while (mData.size() > 1 && mData.back() == 0) { mData.pop_back(); }
}

std::ostream & operator<< (std::ostream& stream, const TBigInt & number) {
    if (number.size() == 0)
    {
        return stream;
    }

    stream << number.mData[number.size() - 1];
    for (int i = number.size() - 2; i >= 0; --i)
    {
        stream << std::setfill('0') << std::setw(R) << number.mData[i];
    }
    return stream;
}

TBigInt::TBigInt(const std::string & str) {
    int i = str.size() - 1, tmp = 0, ind = 1, k = 0;
    while (i >= 0) {
        while (i >= 0 && k < 4) {

            tmp += (str[i] - '0') * ind;
            ind *= 10;
            k++;
            i--;
        }
        ind = 1;
        mData.push_back(tmp);
        k = 0;
        tmp = 0;
    }
    DeleteZeros();
}

bool TBigInt::operator<(const TBigInt & other) {
    if (other.size() != this->size())
    {
        return this->size() < other.size();
    }

    for (int i = other.size() - 1; i >= 0; --i)
    {
        if (other.get(i) != this->get(i))
        {
            return this->get(i) < other.get(i);
        }
    }
    return false;
}

bool TBigInt::operator<=(const TBigInt & other) {
    if (other.size() != this->size())
    {
        return this->size() < other.size();
    }

    for (int i = other.size() - 1; i >= 0; --i)
    {
        if (other.get(i) != this->get(i))
        {
            return this->get(i) < other.get(i);
        }
    }
    return true;
}

bool TBigInt::operator==(const TBigInt & other) {
    if (other.size() != this->size())
    {
        return false;
    }

    for (int i = other.size() - 1; i >= 0; --i)
    {
        if (other.get(i) != this->get(i))
        {
            return false;
        }
    }
    return true;
}

TBigInt TBigInt::operator+(const TBigInt & other) {
    size_t n = std::max(this->size(), other.size());
    TBigInt result(n + 1);
    int r = 0;
    int k = 0;
    for (size_t i = 0; i < n; ++i)
    {
        if (mData.size() <= i)
        {
            k = other.get(i);
        }
        else if (other.size() <= i)
        {
            k = mData[i];
        }
        else
        {
            k = mData[i] + other.get(i);
        }
        k += r;
        result.set(i, k % BASE);
        r = k / BASE;
    }
    if (r != 0)
    {
        result.set(n, r);
    }
    result.DeleteZeros();
    return result;
}

TBigInt TBigInt::operator-(const TBigInt & other) {
    size_t n = std::max(this->size(), other.size());
    TBigInt result(n);
    int r = 0;
    int k = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (other.size() <= i) {
            r = (mData[i] + k) % BASE;
            k = floor((double)(mData[i] + k) / (double)BASE);

        }
        else {
            r = (mData[i] - other.get(i) + k) % BASE;
            k = floor((double)(mData[i] - other.get(i) + k) / (double)BASE);
        }
        if (r < 0)
            r += BASE;
        result.set(i, r);
    }
    result.DeleteZeros();
    return result;
}

TBigInt TBigInt::operator*(const TBigInt & other) {
    size_t n = this->size() + other.size();
    TBigInt result(n);
    int k = 0, t;
    for (int i = 0; i < other.size(); i++) {
        if (other.get(i) != 0) {
            k = 0;
            for (int j = 0; j < this->size(); j++) {
                t = mData[j] * other.get(i) + result.get(i + j) + k;
                result.set(i + j, t % BASE);
                k = floor((double)t / (double)BASE);
            }
            result.set(i + this->size(), k);
        }
    }
    result.DeleteZeros();
    return result;
}

TBigInt TBigInt::operator^(const int & other) {
    TBigInt result, tmp;
    if (other == 1)
        return *this;
    if (other == 0) {
        TBigInt ans(1);
        ans.set(0, 1);
        return ans;
    }
    if ((other % 2) == 0) {
        tmp = (*this) ^ (other / 2);
        result = tmp * tmp;
    }
    else {
        result = ((*this) ^ (other - 1)) * (*this);
    }

    result.DeleteZeros();
    return result;
}

TBigInt TBigInt::operator/(const TBigInt & other) {
    size_t n = other.size(), m = this->size() - n;
    int d = floor((double)BASE / (double)(other.get(n - 1) + 1)), q1, r1;
    TBigInt q(m + 1), tmp2, d1(1), other1, this1(m + n + 1), q_tmp(1);
    d1.set(0, d);
    other1 = d1 * other;
    tmp2 = *this * d1;
    for (int i = 0; i < tmp2.size(); i++)
        this1.set(i, tmp2.get(i));
    for (int j = m; j >= 0; j--) {
        q1 = floor((double)(this1.get(j + n) * BASE + this1.get(j + n - 1)) / (double)other1.get(n - 1));
        r1 = (this1.get(j + n) * BASE + this1.get(j + n - 1)) % other1.get(n - 1);
        while (r1 < BASE &&  q1 > 0 && (q1 == BASE || q1 * other1.get(n - 2) >(BASE * r1 + this1.get(j + n - 2)))) {
            q1--;
            r1 += other1.get(n - 1);
        }
        TBigInt tmp(n + 1);
        for (int i = 0; i <= n; i++) {
            tmp.set(i, this1.get(j + i));
        }
        q_tmp.set(0, q1);
        if (tmp < q_tmp * other1) {
            q1--;
        }
        q_tmp.set(0, q1);
        tmp = tmp - (q_tmp * other1);
        for (int i = 0; i <= n; i++) {
            if (tmp.size() > i)
                this1.set(j + i, tmp.get(i));
            else
                this1.set(j + i, 0);
        }
        q.set(j, q1);
    }
    q.DeleteZeros();
    return q;

}

TBigInt TBigInt::operator/(const int & other) {
    size_t n = this->size(), count = 0;
    int j1 = n - 1;
    std::vector<int> ans;
    while (j1 >= 0) {
        count = count * BASE + mData[j1];
        ans.push_back(count / other);
        count = count % other;
        j1--;
    }
    TBigInt answer(ans.size());
    for (int i = ans.size() - 1; i >= 0; i--) {
        answer.set(ans.size() - 1 - i, ans[i]);
    }
    answer.DeleteZeros();
    return answer;
}

TBigInt(vector<int> & data) {
    m_data.swap(data);
}

TBigInt slice1(size_t from, int to = -1) {
    std::vector<int> tmp(m_data.begin() + from, to == -1 ? m_data.end() : m_data.begin() + to);
}

TBigInt TBigInt::karacuba(const TBigInt & other) {
    if (this->size() < 4 || other.size() < 4)
        return (*this) * other;
    TBigInt x0 = this->slice1(0, this->size() / 2);
    TBigInt x1 = this->slice1(this->size() / 2);
    TBigInt y0 = other.slice1(0, other.size() / 2);
    TBigInt y1 = other.slice1(other.size() / 2);
    TBigInt x0y0 = x0.karacuba(y0);
    TBigInt x1y1 = x1.karacuba(y1);
    TBigInt x0x1y0y1 = (x0 + x1).karacuba(y0 + y1);
    return x1y1 * pow(BASE, 2) + BASE * x0x1y0y1 + x0y0;
}

int GetInt(std::string s) {
    int t = 0;
    for (int i = 0; i < s.size(); i++) {
        t *= 10;
        t += s[i] - '0';
    }
    return t;
}

int main() {
    std::string operation;
    while (std::cin >> number1 >> number2 >> operation) {
        TBigInt number1(num1);
        TBigInt number2(num2);
        TBigInt res;

        if (operation == "+") {
            res = number1 + number2;
        }
        else if (operation == "<") {
            res = number1 < number2;
            if (number1 < number2)
                std::cout << "true" << std::endl;
            else
                std::cout << "false" << std::endl;
            continue;
        }
        else if (operation == ">") {
            res = number2 < number1;
            if (number2 < number1)
                std::cout << "true" << std::endl;
            else
                std::cout << "false" << std::endl;
            continue;
        }
        else if (operation == "=") {
            res = number1 == number2;
            if (number1 == number2)
                std::cout << "true" << std::endl;
            else
                std::cout << "false" << std::endl;
            continue;
        }
        else if (operation == "-" && number2 <= number1) {
            res = number1 - number2;
        }
        else if (operation == "*") {
            res = number1 * number2;
        }
        else if (operation == "/") {
            if (number1 < number2) {
                std::cout << 0 << std::endl;
                continue;
            }

                res = number1 / number2;
        }
        else if (operation == "c")
            break;
        if (res.size() != 0)
        {
            std::cout << res << std::endl;
        }
        else {
            std::cout << "Error" << std::endl;
        }
    }
    return 0;
}



