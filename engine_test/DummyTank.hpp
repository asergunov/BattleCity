#pragma once

#include <Tank.hpp>

class DummyBullet;

class DummyTank : public battlecity::engine::Tank {
public:
    DummyTank(battlecity::engine::Scene* scene);
    std::shared_ptr<DummyBullet> bullet() const;

    // Tank interface
public:
    void fire() override;
    battlecity::engine::velocity_type speed() const override;
    bool processHit(battlecity::engine::Bullet *) override;
private:
    std::weak_ptr<DummyBullet> _bullet;
};
