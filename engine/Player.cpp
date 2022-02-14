#include "Player.hpp"

#include "Scene.hpp"

#include <cassert>

using namespace battlecity::engine;

class PlayerBullet : public Bullet {
    Player *_player;
public:
    PlayerBullet(Player* player)
        : Bullet(player)
        , _player(player) {

    }

    Player *player() const override {
        return _player;
    }

    health_type power() const override {
        const auto& level = player()->level();
        if(level >= 3)
            return 4;
        if(level >= 2)
            return 2;
        return 1;
    }

    velocity_type speed() const override {
            return player()->level() < 2
                    ? velocity_type(2,1)
                    : velocity_type(4,1);
    }
};

Player::Player(field_vector_type respawn_cell, Scene *scene)
    : Tank(scene)
    , _respawn_cell(respawn_cell)
    , _respawn_direction(direction_type::Type::Up)
{
    Tank::respawn(_respawn_cell, _respawn_direction);
    _undestroyable_time = 0;
}

void Player::respawn()
{
    if(_killed && _life > 0) {
        --_life;
        _killed = false;
        _level = 0;
        setUndestroyableFor(60*5);
    }

    if(!_killed)
        Tank::respawn(_respawn_cell, _respawn_direction);
}

void Player::fire()
{
    assert(scene());
    if(scene()->gameOver())
        return;
    const auto bulletsCount = this->bulletsCount();

    if((_level < 2 && bulletsCount >= 1) || bulletsCount >= 2)
        return;

    addBullet(std::make_shared<PlayerBullet>(this));
}

velocity_type Player::speed() const
{
    return velocity_type(3, 4);
}

bool Player::processHit(Bullet *bullet)
{
    if(isUndestroyable())
        return true;

    assert(this != bullet->player());
    if(bullet->isPlayer()) {
        // friendly fire
        _friendly_fire_freeze = 192;
        scene()->tankFriendlyFireFreeze(this);
    } else {
        scene()->tankExplode(this);
        _killed = true;
        scene()->killPlayer(this);
    }
    return true;
}

void Player::process(time_type dt)
{
    assert(scene());
    if(scene()->gameOver())
        return;

    _undestroyable_time -= std::min(dt, _undestroyable_time);
    if(_friendly_fire_freeze) {
        const auto min = std::min(dt, _friendly_fire_freeze);
        _friendly_fire_freeze -= min;
        dt -= min;
        if(!dt)
            return;
    }

    Tank::process(dt);
}

void Player::turn(const direction_type &direction)
{
    assert(scene());
    if(scene()->gameOver())
        return;

    Tank::turn(direction);
}

void Player::setUndestroyableFor(time_type period)
{
    _undestroyable_time = std::max(period, _undestroyable_time);
}

void Player::levelUp()
{
    _level = std::min(3, _level+1);
}

void Player::addLife()
{
    ++_life;
}

const uint_fast8_t &Player::lifeCount() const {
    return _life;
}

void Player::occupiedCellsChanged(const field_rect_type & cells)
{
    if(auto bonus = scene()->bonusAt(cells)) {
        scene()->applyBonus(this, bonus);
    }
}

