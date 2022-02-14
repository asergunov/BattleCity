#include "DummyBullet.hpp"

#include "DummyTank.hpp"

#include <memory>

DummyBullet::DummyBullet(DummyTank *tank)
    : battlecity::engine::Bullet(tank)
{

}

battlecity::engine::health_type DummyBullet::power() const
{
    return 1;
}

battlecity::engine::Player *DummyBullet::player() const
{
    return nullptr;
}

battlecity::engine::velocity_type DummyBullet::speed() const
{
    return 1;
}
