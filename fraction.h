#ifndef FRACTION_H
#define FRACTION_H

#include<numeric>
#include<ostream>
#include<cassert>
#include<cstdlib>

struct fraction{
   fraction(int x=0, int y=1) noexcept: a(x), b(y){
      assert(b != 0);
      if(b < 0){
         a = -a, b = -b;
      }
      if(a == 0){
         b = 1; return;
      }
      int d = std::gcd(abs(a), b);
      a /= d; b /= d;
   }
   int numerator() const noexcept{
      return a;
   }
   int denominator() const noexcept{
      return b;
   }
   fraction operator+() const noexcept{
      return *this;
   }
   fraction operator-() const noexcept{
      return {-a, b};
   }
   fraction operator+(fraction const &rhs) const noexcept{
      return {a*rhs.b + b*rhs.a, b*rhs.b};
   }
   fraction &operator+=(fraction const &rhs) noexcept{
      return *this = *this+rhs;
   }
   fraction operator-(fraction const &rhs) const noexcept{
      return *this + (-rhs);
   }
   fraction &operator-=(fraction const &rhs){
      return *this = *this-rhs;
   }
   fraction operator*(fraction const &rhs) const{
      return {a*rhs.a, b*rhs.b};
   }
   fraction &operator*=(fraction const &rhs){
      return *this = *this*rhs;
   }
   fraction operator/(fraction const &rhs) const{
      return {a*rhs.b, b*rhs.a};
   }
   fraction &operator/=(fraction const &rhs){
      return *this = *this/rhs;
   }
   bool operator==(fraction const &rhs) const{
      return a==rhs.a && b==rhs.b;
   }
   bool operator!=(fraction const &rhs) const{
      return !(*this == rhs);
   }
   bool operator<(fraction const &rhs) const{
      return a*rhs.b < b*rhs.a;
   }
   bool operator>(fraction const &rhs) const{
      return a*rhs.b > b*rhs.a;
   }
   bool operator<=(fraction const &rhs) const{
      return !(*this > rhs);
   }
   bool operator>=(fraction const &rhs) const{
      return !(*this < rhs);
   }
private:
   int a, b;
};
inline fraction operator+(int n, const fraction &f){
    return f + n;
}
inline fraction operator-(int n, const fraction &f){
    return (-f) + n;
}
inline fraction operator*(int n, const fraction &f){
    return f * n;
}
inline fraction operator/(int n, const fraction &f){
    return fraction(n) / f;
}
inline std::ostream &operator<<(std::ostream &out, fraction const &f){
    if(f.denominator() == 1) return out << f.numerator();
    return out << f.numerator() << "/" << f.denominator();
}

#endif
