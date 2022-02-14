#pragma once

#include "types.hpp"
#include "Actor.hpp"

#include <MovementProcessor.hpp>

#include <unordered_set>

namespace battlecity {
namespace engine {

class Player;
class Tank;

class Bullet : public Actor
{
public:
    Bullet(Tank *tank);
    virtual ~Bullet();
    bool isPlayer() const { return player(); }

    field_vector_type cell() const;
    std::unordered_set<field_vector_type> cellsToHit();

    virtual health_type power() const = 0;
    virtual Player* player() const = 0;
    virtual velocity_type speed() const = 0;

    // Actor interface
public:
    void process(time_type dt) override;
    coordinate_rect_type rect() const override final;
    direction_type direction() const override final;
    bool collides(const Bullet* other) const;

private:
    void cellChanged(const std::unordered_set<field_vector_type> &cellsToHit);

private:
    field_vector_type _start_point;
    coordinate_type _position;
    direction_type _direction;
    math::MovementProcessor<time_type, coordinate_type, velocity_type, math::IdentityFunction> _movement;

};

}}
