#pragma once

#include <functional> // for the hash

namespace battlecity {
namespace math {

template <typename T>
class Vector
{
public:
    using value_type = T;
public:
    Vector() = default;
    Vector(const value_type& x, const value_type& y) : x(x), y(y) {}
    Vector(const Vector&) = default;

    template<typename T2>
    explicit Vector(const Vector<T2>& other) : Vector(other.x, other.y) {}

    Vector& operator=(const Vector&) = default;

    Vector& operator+=(const Vector& other) {
        x+=other.x;
        y+=other.y;
        return *this;
    }

    Vector& operator-=(const Vector& other) {
        x-=other.x;
        y-=other.y;
        return *this;
    }

    bool operator==(const Vector& other) const {
        return x == other.x && y == other.y;
    }

public:
    value_type x, y;
};

template<typename T>
Vector<T> operator+(const Vector<T>& a, const Vector<T>& b) {
    return {a.x+b.x, a.y+b.y};
}

template<typename T>
Vector<T> operator-(const Vector<T>& a, const Vector<T>& b) {
    return {a.x-b.x, a.y-b.y};
}

template<typename T>
Vector<T> operator*(const Vector<T>& a, const T& b) {
    return {a.x*b, a.y*b};
}
template<typename T>
Vector<T> operator*(const T& b, const Vector<T>& a) {
    return {a.x*b, a.y*b};
}

}}

namespace std {
template<typename T>
struct hash<battlecity::math::Vector<T>> {
    std::hash<T> _hash;
    size_t operator() (const battlecity::math::Vector<T>& arg) const {
        const auto y_hash = _hash(arg.y);
        return _hash(arg.x) ^ (y_hash << sizeof(size_t)*8/2) ^ (y_hash >> sizeof(size_t)*8/2);
    }
};
}
