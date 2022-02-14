#pragma once

#include <Bullet.hpp>

class DummyTank;

class DummyBullet : public battlecity::engine::Bullet {
public:
    DummyBullet(DummyTank* tank);

    // Bullet interface
public:
    battlecity::engine::health_type power() const override;
    battlecity::engine::Player *player() const override;
    battlecity::engine::velocity_type speed() const override;
};
