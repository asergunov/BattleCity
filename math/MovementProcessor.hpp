#pragma once

#include "Direction.hpp"
#include "FixedPoint.hpp"
#include "Vector.hpp"

#include <type_traits>
#include <limits>


namespace battlecity {
namespace math {

/**
 * @brief The MovementProcessor class
 */
template <typename _Time, typename _Position, typename _Velocity, typename _ScalarToPositionFunctor>
class  MovementProcessor {
    using time_type = _Time;
    using position_type = _Position;
    using velocity_type = _Velocity;

    velocity_type _reminder = 0;
    position_type& _pos;
public:
    _ScalarToPositionFunctor scalarToDelta;
public:
    MovementProcessor(position_type& position, _ScalarToPositionFunctor scalarToDelta = {})
        : _pos(position)
        , scalarToDelta(scalarToDelta) {

    }

    void process(time_type dt, const velocity_type& speed) {
        move(dt, speed);
        apply();
    }

    void move(time_type dt, const velocity_type& speed) {
        _reminder += speed*dt;
    }

    /**
     * @brief apply the movement to position
     * @param maxSteps to do
     * @return true if nothing remaind
     */
    bool apply(const typename velocity_type::numeric_type& maxSteps = std::numeric_limits<typename velocity_type::numeric_type>::max()) {
        const auto steps = std::max(0, std::min(_reminder.floor(), maxSteps));
        _reminder -= steps;
        _pos += scalarToDelta(steps);
        return !haveSomethingToApply();
    }

    bool haveSomethingToApply() const {
        return _reminder.numerator() >= _reminder.denumenator();
    }

    void reset() {
        _reminder = 0;
    }
};

template<typename T>
struct DirectionToVector {
    DirectionToVector() {}
    explicit DirectionToVector(const Direction& direction) : direction(direction) {}
    Direction direction;
    Vector<T> operator() (const T& scalar) const {
        return direction.toVector<T>()*scalar;
    }
};


struct IdentityFunction {
    template<typename T>
    T operator() (const T& scalar) const {
        return scalar;
    }
};

}}
