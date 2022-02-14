#pragma once

#include "Tank.hpp"

namespace battlecity {
namespace engine {

class Enemy : public Tank
{
public:
    enum class Type {
        Basic, Fast, Power, Armor
    };

    explicit Enemy(Type type, bool bonus, Scene *scene);

    // Tank interface
public:
    void fire() override;
    velocity_type speed() const override;
    void process(time_type dt) override;
    bool processHit(Bullet *bullet) override;
    field_rect_type occupiedCells() const override;

    score_type scoreForKill() const;
    const Type& type() const { return _type; }
    const health_type& health() const { return _health; }
    bool isBonus() const { return _isBonus; }
    bool isSpawning() const { return _respawn_timeout != 0; }

protected:
    void arrivedToCell(const field_vector_type &newCell) override;
    void hitObstacle() override;

private:
    bool turnRandomDirection();
    void changeDirection();

    void resetMoveTarget();
    void setMoveTarget(const field_vector_type& target);

    direction_type directionToGetTo(const field_vector_type& target) const;

private:
    Type _type;
    health_type _health;
    bool _isBonus;
    int _id;

    bool _targetIsSet = false;
    field_vector_type _target;
    time_type _respawn_timeout = 0;

};

}}
