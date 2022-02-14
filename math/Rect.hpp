#pragma once

#include "Vector.hpp"

namespace battlecity {
namespace math {

template <typename T>
class Rect
{
public:
    using value_type = T;
    using vector_type = Vector<T>;

    Rect() : _topLeft(0,0), _size(0,0) {}
    Rect(const Rect&) = default;
    Rect(const vector_type& topLeft, const vector_type& size)
        :_topLeft(topLeft), _size(size) {}

    Rect& adjust(const value_type& left, const value_type& top, const value_type& right, const value_type& bottom) {
        _topLeft.x += left;
        _topLeft.y += top;
        _size.x += right - left;
        _size.y += bottom - top;
        return *this;
    }

    const vector_type& topLeft() const {
        return _topLeft;
    }

    const vector_type& size() const {
        return _size;
    }

    const value_type& left() const {
        return _topLeft.x;
    }

    const value_type& top() const {
        return _topLeft.y;
    }

    const value_type& width() const {
        return _size.x;
    }

    const value_type& height() const {
        return _size.y;
    }

    bool contains(const vector_type& point) const {
        return _topLeft.x <= point.x && _topLeft.y <= point.y &&
                point.x < _topLeft.x + _size.x && point.y < _topLeft.y + _size.y;
    }

    Rect translated(const vector_type& delta) const {
        return {_topLeft+delta, _size};
    }

    bool isNull() const {
        return _size.x == 0 || _size.y == 0;
    }

    bool intersects(const Rect& r) const {
        auto valueInRange = [](const value_type& value, const value_type& begin, const value_type& end) {
            return (value >= begin) && (value < end);
        };

        const auto& xOverlap =
                valueInRange(left(), r.left(), r.left() + r.width()) ||
                valueInRange(r.left(), left(), left() + width());

        const auto& yOverlap =
                valueInRange(top(), r.top(), r.top() + r.height()) ||
                valueInRange(r.top(), top(), top() + height());

        return xOverlap && yOverlap;
    }

private:
    vector_type _topLeft;
    vector_type _size;
};

}}
