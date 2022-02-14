#include "Tank.hpp"

#include "Scene.hpp"
#include "Bullet.hpp"
#include "Field.hpp"
#include "Cell.hpp"

#include <cassert>
#include <algorithm>
#include <unordered_map>

using namespace battlecity::engine;

Tank::Tank(Scene* scene)
    : Actor(scene)
    , _movement(_coordinate)
{

}

void Tank::process(time_type dt)
{
    // remove expired bullets
    _bullets.erase(std::remove_if(begin(_bullets), end(_bullets), [](const std::weak_ptr<Bullet>& bullet){
        return bullet.expired();
    }), _bullets.end());

    processMovement(dt);
}

coordinate_rect_type Tank::rect() const
{
    return scene()->mapFromField({_coordinate._cell, {2,2}}).translated(
                _coordinate._direction.toVector<coordinate_type>()*_coordinate._move_stage
    );
}

direction_type Tank::direction() const
{
    return _coordinate._direction;
}

field_vector_type Tank::fieldCell()
{
    return _coordinate._cell;
}

void Tank::respawn(const field_vector_type &cell, const direction_type &direction)
{
    _coordinate._cell = cell;
    _coordinate._direction = direction;
    _coordinate._move_stage = 0;

    _next_direction = direction;
    _movement.reset();
}

void Tank::turn(const direction_type &direction)
{
    _next_direction=direction;

    if(_coordinate._direction == direction) {
        return;
    } else if(_coordinate._direction == direction.inversed()) {
        turnAround();
    } else if(!_isMoving && std::abs(_coordinate._move_stage) < scene()->fieldCellSize()/4) {
        turnAround();
    }
}

/**
 * @brief turn tank around
 *
 * It does not change next tank direction
 */
void Tank::turnAround()
{
    _coordinate._direction = _coordinate._direction.inversed();
    if(inTransition()) {
        _coordinate._move_stage = scene()->fieldCellSize() - _coordinate._move_stage;
        _coordinate._cell -= _coordinate._direction.toVector<field_coordinate_type>();
    }
    directionChanged(_coordinate._direction);
}

void Tank::move()
{
    if(_isMoving)
        return;
    _isMoving = true;
}

void Tank::stop()
{
    _isMoving = false;
}

size_t Tank::bulletsCount() const
{
    return std::count_if(begin(_bullets), end(_bullets), [](const std::weak_ptr<Bullet>& bullet){
        return !bullet.expired();
    });
}

bool Tank::inTransition() const
{
    return _coordinate._move_stage != 0;
}

const field_vector_type &Tank::cell() const
{
    return _coordinate._cell;
}

field_rect_type Tank::occupiedCells() const
{
    field_rect_type result(_coordinate._cell, {2,2});
    if(inTransition()) {
        const auto& direction = _coordinate._move_stage >0
                ? _coordinate._direction
                : _coordinate._direction.inversed();
        switch (direction.type()) {
        case direction_type::Up:
            result.adjust(0, -1, 0, 0);
            break;
        case direction_type::Left:
            result.adjust(-1, 0, 0, 0);
            break;
        case direction_type::Down:
            result.adjust(0, 0, 0, 1);
            break;
        case direction_type::Right:
            result.adjust(0, 0, 1, 0);
            break;
        }
    }
    return result;
}

bool Tank::canMove(const direction_type &direction) const
{
    if(inTransition()) {
        return direction == _coordinate._direction || direction == _coordinate._direction.inversed();
    }

    static const std::unordered_map<direction_type, field_rect_type> cellsAhead = {
        {direction_type::Left, {{-1,0},{1,2}}},
        {direction_type::Right, {{2,0},{1,2}}},
        {direction_type::Up, {{0,-1},{2,1}}},
        {direction_type::Down, {{0,2},{2,1}}}
    };

    scene()->cellsAreFreeForTank(cellsAhead.at(direction).translated(_coordinate._cell), this);
}

bool Tank::isMoving() const
{
    return _isMoving;
}

/**
 * @brief process movement
 *
 * If turn allowed it applied
 */
void Tank::processMovement(time_type dt)
{
    if(!_isMoving) {
        _movement.reset();
        return;
    }

    _movement.move(dt, speed());

    for(;_movement.haveSomethingToApply();) {
        if(0 == _coordinate._move_stage) {
            if(_next_direction != direction()) {
                _coordinate._direction = _next_direction;
                directionChanged(_coordinate._direction);
            }

            if(!canMove(_coordinate._direction)) {
                hitObstacle();
                if(!canMove(_coordinate._direction)) {
                    _movement.reset();
                    return;
                }
            }

            _movement.apply(1);
            occupiedCellsChanged(occupiedCells());
        }

        assert(_coordinate._move_stage >= 0);
        if(_coordinate._move_stage > 0) {
            _movement.apply(scene()->fieldCellSize() - _coordinate._move_stage);
            if(_coordinate._move_stage == scene()->fieldCellSize()) {
                _coordinate._cell += _coordinate._direction.toVector<field_coordinate_type>();
                _coordinate._move_stage = 0;
                arrivedToCell(_coordinate._cell);
                occupiedCellsChanged(occupiedCells());
            }
        }
    }
}

void Tank::addBullet(std::shared_ptr<Bullet> bullet)
{
    _bullets.push_back(bullet);
    scene()->addBullet(bullet, this);
}

/**
 * @brief direction of tank is changed
 */
void Tank::directionChanged(const direction_type &/*newDirection*/)
{

}

/**
 * @brief tank stopped
 */
void Tank::stopped()
{

}

void Tank::occupiedCellsChanged(const field_rect_type &)
{

}

/**
 * @brief arrived to cell
 *
 * Tank arrived to cell. It's not in transition.
 */
void Tank::arrivedToCell(const field_vector_type &/*newCell*/)
{

}

/**
 * @brief tank hit obstacle
 *
 * It's a chace to change direction
 */
void Tank::hitObstacle()
{

}


Tank::coordinate_vector &Tank::coordinate_vector::operator+=(const coordinate_type &other) {
    _move_stage+=other;
    return *this;
}
