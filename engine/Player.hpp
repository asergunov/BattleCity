#pragma once

#include "Tank.hpp"

namespace battlecity {
namespace engine {

class Player : public Tank
{
public:
    Player(field_vector_type respawn_cell, Scene* scene = nullptr);

    void respawn();

    // Tank interface
public:
    void fire() override;
    velocity_type speed() const override;
    bool processHit(Bullet *bullet) override;
    uint_fast8_t level() const { return _level; }
    bool isUndestroyable() const { return _undestroyable_time > 0; }
    bool isKilled() const { return _killed; }
    void process(time_type dt) override;
    void turn(const direction_type& direction);
    void setUndestroyableFor(time_type period);
    void levelUp();
    void addLife();
    const uint_fast8_t& lifeCount() const;

protected:
    void occupiedCellsChanged(const field_rect_type&) override;

private:
    uint_fast8_t _star_level = 0;
    uint_fast8_t _level = 0;
    uint_fast8_t _life = 3;
    time_type _friendly_fire_freeze = 0;

    time_type _undestroyable_time = 0;
    field_vector_type _respawn_cell;
    direction_type _respawn_direction;
    bool _killed = false;
};

}}
