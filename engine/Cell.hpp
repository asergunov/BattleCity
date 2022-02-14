#pragma once

#include <Direction.hpp>

#include <cstdint>


namespace battlecity {
namespace engine {


class Cell
{
public:
    using data_type = uint8_t;

    enum class Type : data_type {
        Empty    = 0x00,
        Stone   = 0x01,
        Water   = 0x02,
        Bush    = 0x03,
        Ice     = 0x04,
        Brick   = 0x05,
        undestroyableWall = 0x06,
    };

    enum class Hit : data_type {
        Mask     = 0xf0,
        Up       = 0x10,
        Down     = 0x20,
        Left     = 0x40,
        Right    = 0x80,
    };

private:
    data_type _value;     ///< the cell value. Stores the state of the cell. @see Type, Hit

public:
    Cell(Type type = Type::Empty);

    bool operator==(const Type& type) const {
        return this->type() == type;
    }

    bool operator==(const Cell& cell) const {
        return _value == cell._value;
    }

    bool operator!=(const Cell& cell) const {
        return _value != cell._value;
    }

    Type type() const {
        return static_cast<Type>(_value & 0x0F);
    }

    Hit hit() const {
        return static_cast<Hit>(_value & 0xF0);
    }

    bool isBuisyForTank() const;
    bool isBuisyForBullet() const;
    bool processHit(const math::Direction& direction);
    bool processHit(Hit hit);

    static Hit oposite(Hit hit);
};

}}
