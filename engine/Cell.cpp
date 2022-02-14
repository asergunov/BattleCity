#include "Cell.hpp"

using namespace battlecity::engine;
using namespace battlecity::math;

Cell::Cell(Type type)
    : _value(static_cast<data_type>(type))
{

}

bool Cell::isBuisyForTank() const {
    const auto& type = this->type();
    switch(type) {
    case Type::Stone:
    case Type::Water:
    case Type::Brick:
    case Type::undestroyableWall:
        return true;
    case Type::Empty:
    case Type::Bush:
    case Type::Ice:
        break;
    }
    return false;
}

bool Cell::isBuisyForBullet() const {
    const auto& type = this->type();
    switch(type) {
    case Type::Stone:
    case Type::Brick:
    case Type::undestroyableWall:
        return true;
    case Type::Water:
    case Type::Empty:
    case Type::Bush:
    case Type::Ice:
        break;
    }
    return false;
}

bool Cell::processHit(const battlecity::math::Direction &direction)
{
    switch (direction.type()) {
    case Direction::Type::Down:
        return processHit(Hit::Up);
    case Direction::Type::Up:
        return processHit(Hit::Down);
    case Direction::Type::Left:
        return processHit(Hit::Right);
    case Direction::Type::Right:
        return processHit(Hit::Left);
    }
    return false;
}

/**
 * @brief process hit into the wall
 * @param hit directino to process
 * @return \a true if cell was changed by the hit
 */
bool Cell::processHit(Hit hit) {
    if(type() != Type::Brick)
        return isBuisyForBullet();
    const auto& ui_hit = static_cast<data_type>(hit);

    _value = (0 == (_value & ui_hit) && 0 == (_value & static_cast<data_type>(oposite(hit)))) // it was not a hit from this side or from oposite
            ? _value | ui_hit       // add a flag
            : static_cast<data_type>(Type::Empty);   // it's second hit. Destroy.
    return true;
}

Cell::Hit Cell::oposite(Cell::Hit hit)
{
    switch (hit) {
    case Hit::Down: return Hit::Up;
    case Hit::Left: return Hit::Right;
    case Hit::Up: return Hit::Down;
    case Hit::Right: return Hit::Left;
    default:
        break;
    }
    return Hit::Mask;
}
