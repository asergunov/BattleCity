#pragma once

#include <Vector.hpp>

#include <cstdint>
#include <functional> // for the hash
#include <type_traits>

namespace battlecity {
namespace math {


class Direction {
    using numeric_type = uint_fast8_t;
public:
    enum Type : numeric_type {
        Up, Left, Down, Right
    };

    Direction(Type type= Up) : _type(type) {

    }

    bool operator==(const Direction& other) const {
        return _type == other._type;
    }

    bool operator!=(const Direction& other) const {
        return _type != other._type;
    }

    const Type& type() const {
        return _type;
    }

    Direction operator+(const Direction& other) const {
        return {static_cast<Type>((static_cast<numeric_type>(other._type) + static_cast<numeric_type>(_type))%4)};
    }

    Direction turnedCV() const {
        switch (_type) {
            case Direction::Up: return Direction::Right;
            case Direction::Right: return Direction::Down;
            case Direction::Down: return Direction::Left;
            case Direction::Left: return Direction::Up;
        }
        return {};
    }

    inline Direction turnedCCV() const {
        switch (_type) {
            case Direction::Up: return Direction::Left;
            case Direction::Right: return Direction::Up;
            case Direction::Down: return Direction::Right;
            case Direction::Left: return Direction::Down;
        }
        return {};
    }

    inline Direction inversed() const {
        switch (_type) {
            case Direction::Up: return Direction::Down;
            case Direction::Right: return Direction::Left;
            case Direction::Down: return Direction::Up;
            case Direction::Left: return Direction::Right;
        }
        return {};
    }

    template<typename T>
    inline Vector<T> toVector() const {
        switch (_type) {
            case Direction::Up: return Vector<T>(0, -1);
            case Direction::Right: return Vector<T>(1, 0);
            case Direction::Down: return Vector<T>(0, 1);
            case Direction::Left: return Vector<T>(-1, 0);
        }
        return {};
    }

private:
    Type _type;
};


}}

namespace std {
template<> struct hash<battlecity::math::Direction::Type> {

    std::hash<std::underlying_type<battlecity::math::Direction::Type>::type> _hash;
    size_t operator() (const battlecity::math::Direction& arg) const {
        return _hash(arg.type());
    }
};
template<> struct hash<battlecity::math::Direction> {
    std::hash<battlecity::math::Direction::Type> _hash;
    size_t operator() (const battlecity::math::Direction& arg) const {
        return _hash(arg.type());
    }
};
}
