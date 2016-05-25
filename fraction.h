#include<cassert>
#include<algorithm>

using namespace std;

class fraction{
private:
    int a, b; // a/b
public:
    fraction(const int &a = 0, const int &b = 1): a(a), b(b){
        assert(b != 0);
        if(this->b < 0){
            this->a = -this->a; this->b = -this->b;
        }
        if(this->a == 0){
            this->b = 1; return;
        }
        int d = __gcd(abs(this->a), this->b);
        this->a /= d; this->b /= d;
    };
    fraction operator +(const fraction &another) const{
        return fraction(a * another.b + b * another.a, b * another.b);
    }
    fraction operator -() const{
        return fraction(-a, b);
    }
    fraction operator -(const fraction &another) const{
        return *this + (-another);
    }
    fraction operator *(const fraction &another) const{
        return fraction(a * another.a, b * another.b);
    }
    fraction operator /(const fraction &another) const{
        assert(another.a != 0);
        return *this * fraction(another.b, another.a);
    }
    fraction operator +=(const fraction &another){
        return *this = *this + another;
    }
    fraction operator -=(const fraction &another){
        return *this = *this - another;
    }
    fraction operator *=(const fraction &another){
        return *this = *this * another;
    }
    fraction operator /=(const fraction &another){
        return *this = *this / another;
    }
    bool operator ==(const fraction &another) const{
        return a == another.a && b == another.b;
    }
    bool operator !=(const fraction &another) const{
        return !(*this == another);
    }
    bool operator <(const fraction &another) const{
        return a * another.b < b * another.a;
    }
    bool operator >(const fraction &another) const{
        return a * another.b > b * another.a;
    }
    bool operator <=(const fraction &another) const{
        return !(*this > another);
    }
    bool operator >=(const fraction &another) const{
        return !(*this < another);
    }
    friend ostream& operator << (ostream&, const fraction&);
};
fraction operator +(const int &n, const fraction &f){
    return f + n;
}
fraction operator -(const int &n, const fraction &f){
    return (-f) + n;
}
fraction operator *(const int &n, const fraction &f){
    return f * n;
}
fraction operator /(const int &n, const fraction &f){
    return fraction(n) / f;
}
ostream &operator <<(ostream& out, const fraction &f){
    if(f.b == 1) return out << f.a;
    return out << f.a << "/" << f.b;
}

