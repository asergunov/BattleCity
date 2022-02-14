#include "Bullet.hpp"

#include "Scene.hpp"

#include <cassert>

using namespace battlecity::engine;

Bullet::Bullet(Tank *tank)
    : Actor(tank->scene())
    , _start_point([&]()-> field_vector_type{
        const auto& cell = tank->cell();
        switch (tank->direction().type()) {
        case math::Direction::Up:
            return cell;
        case math::Direction::Left:
            return {cell.x, cell.y+1};
        case math::Direction::Down:
            return {cell.x+1, cell.y+1};
        case math::Direction::Right:
            return {cell.x+1, cell.y};
        }
    }())
    , _position(tank->scene()->fieldCellSize()/4)
    , _direction(tank->direction())
    , _movement(_position)
{
}

Bullet::~Bullet()
{

}

field_vector_type Bullet::cell() const
{
    return _start_point + _direction.toVector<field_coordinate_type>() * field_coordinate_type(_position / scene()->fieldCellSize());
}

std::unordered_set<field_vector_type> Bullet::cellsToHit()
{
    const auto basicCell = cell();
    return {basicCell, basicCell+(_direction + direction_type::Type::Right).toVector<field_coordinate_type>()};
}

void Bullet::process(time_type dt)
{
    assert(scene());
    _movement.move(dt, speed());
    while(_movement.haveSomethingToApply()) {
        _movement.apply(_position % scene()->fieldCellSize());
        if(_movement.haveSomethingToApply()) {
            _movement.apply(1);

            cellChanged( cellsToHit() );
        }
    }
}

coordinate_rect_type Bullet::rect() const
{
    assert(scene());
    return scene()->mapFromField({_start_point, {1, 1}}).translated(
        _direction.toVector<coordinate_type>()*_position +
       (_direction + direction_type::Type::Right).toVector<coordinate_type>()*(scene()->fieldCellSize()/2)
    );
}

direction_type Bullet::direction() const
{
    return _direction;
}

void Bullet::cellChanged(const std::unordered_set<field_vector_type>& cellsToHit)
{
    assert(scene());
    std::unordered_set<Tank*> tanks;
    bool explode = false;

    // bullet-bullet collision
    const auto bullets = scene()->bullets();
    for(const auto& bullet: bullets) {
        if(bullet.get() == this)
            continue;   // only other bullet

        if(bullet->isPlayer() == this->isPlayer())
            continue;   // only player-enemy collision

        if(bullet->collides(this)) {
            scene()->bulletExplode(this);
            scene()->bulletExplode(bullet.get());
        }
    }


    auto& field = scene()->field();
    for(const auto& cell : cellsToHit) {
        if(field.outOfRange(cell)) {
            explode = true;
        } else {
            if(scene()->eagle().fieldRect().contains(cell)) {
                explode = true;
                scene()->killEagle();
            }

            auto& fieldCell = field.at(cell);
            if(this->power() >= 4) {
                if(fieldCell.type() == Cell::Type::Brick ||
                   fieldCell.type() == Cell::Type::Stone ) {
                    fieldCell = Cell::Type::Empty;
                    explode = true;
                }
            } else if(fieldCell.processHit(_direction)) {
                explode = true;
            }
        }

        if(const auto& tank = scene()->tankAtCell(cell))
            tanks.insert(tank);
    }

    for(const auto& tank : tanks) {
        if(player() == tank)
            continue;

        if(tank->processHit(this))
            explode = true;
    }

    if(explode)
        scene()->bulletExplode(this);
}

/**
 * @brief is this bullet collides with another one
 * @param other
 * @return
 */
bool Bullet::collides(const Bullet *other) const
{
    if(other->_direction != _direction.inversed())
        return false;   // only for oposite directions

    return other->cell() == (cell()
            + (_direction+direction_type::Type::Right).toVector<field_coordinate_type>() // other line
            //+ (_direction).toVector<field_coordinate_type>() // one cell ahead
            );
}
