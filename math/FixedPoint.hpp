#pragma once

#include <stdexcept>

namespace battlecity {
namespace math {

/**
 * Fixed point value
 *
 * numerator is stored but denumenator is a part of the type.
 *
 * So we can operate only the values with same denumenator
 */
template <typename T, T _denumenator>
class FixedPoint
{
public:
    using numeric_type = T;

    FixedPoint() {}
    FixedPoint(const numeric_type& value)
        : _numerator(value*_denumenator) {

    }

    FixedPoint(const numeric_type& numerator, const numeric_type& denominator)
        : _numerator(numerator * _denumenator / denominator) {
        if(_denumenator % denominator != 0)
            throw std::runtime_error("wrong denumenator");
    }

    FixedPoint& operator+=(const FixedPoint& other) {
        _numerator += other._numerator;
        return *this;
    }

    FixedPoint& operator-=(const FixedPoint& other) {
        _numerator -= other._numerator;
        return *this;
    }

    bool operator<=(const FixedPoint& other) const {
        return _numerator <= other._numerator;
    }

    bool operator>=(const FixedPoint& other) const {
        return _numerator >= other._numerator;
    }

    FixedPoint& operator*=(const numeric_type& other) {
        _numerator *= other;
        return *this;
    }

    FixedPoint operator*(const numeric_type& other) const {
        return {_numerator * other, _denumenator};
    }

    numeric_type mod() const {
        return _numerator % _denumenator;
    }

    numeric_type floor() const {
        return _numerator / _denumenator;
    }

    const numeric_type& numerator() const {
        return _numerator;
    }

    static numeric_type denumenator() {
        return _denumenator;
    }

private:
    numeric_type _numerator;
};

}}
