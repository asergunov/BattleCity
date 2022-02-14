#pragma once

#include "Actor.hpp"

#include <MovementProcessor.hpp>

#include <list>
#include <memory>
#include <unordered_set>

namespace battlecity {
namespace engine {

class Bullet;
class Cell;

class Tank : public Actor
{
public:
    Tank(Scene* scene = nullptr);

    virtual void fire() = 0;
    virtual velocity_type speed() const = 0;
    virtual bool processHit(Bullet*) = 0;

    void process(time_type dt) override;

    coordinate_rect_type rect() const override final;
    direction_type direction() const override final;

    field_vector_type fieldCell();

    void respawn(const field_vector_type &, const direction_type& direction);
    void turn(const direction_type& direction);
    void move();
    void stop();

    size_t bulletsCount() const;
    bool inTransition() const;

    const field_vector_type& cell() const;
    field_vector_type bulletSpawnCell() const;
    virtual field_rect_type occupiedCells() const;
    bool canMove(const direction_type& direction) const;

    bool isMoving() const;

protected:
    void addBullet(std::shared_ptr<Bullet> bullet);

    virtual void directionChanged(const direction_type& newDirection);
    virtual void stopped();
    virtual void occupiedCellsChanged(const field_rect_type&);
    virtual void arrivedToCell(const field_vector_type& newCell);
    virtual void hitObstacle();

private:
    void processMovement(time_type dt);
    void turnAround();

private:
    struct coordinate_vector{
        field_vector_type _cell; ///< tank ocupied 4 cels startion from this one
        coordinate_type _move_stage;        ///< the current movement stage within the cell
        direction_type _direction;
        coordinate_vector& operator+=(const coordinate_type& other);
    } _coordinate;
    direction_type _next_direction;
    math::MovementProcessor<time_type, coordinate_vector, velocity_type, math::IdentityFunction> _movement;
    bool _isMoving = false;
    std::list<std::weak_ptr<Bullet>> _bullets;
};

}}
