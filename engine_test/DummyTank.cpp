#include "DummyTank.hpp"
#include "DummyBullet.hpp"

DummyTank::DummyTank(battlecity::engine::Scene *scene)
    : battlecity::engine::Tank(scene)
{

}

std::shared_ptr<DummyBullet> DummyTank::bullet() const
{
    return _bullet.lock();
}

void DummyTank::fire()
{
    if(!_bullet.expired())
        return;

    const auto bullet = std::make_shared<DummyBullet>(this);
    _bullet = bullet;
    addBullet(bullet);
}

battlecity::engine::velocity_type DummyTank::speed() const
{
    return 1;
}

bool DummyTank::processHit(battlecity::engine::Bullet *)
{
    return false;
}
