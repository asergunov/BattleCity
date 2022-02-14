#include "Enemy.hpp"

#include "Scene.hpp"
#include "Field.hpp"

using namespace battlecity::engine;

namespace {
direction_type random_direction() {
    return static_cast<direction_type::Type>(std::rand()%4);
}
}

class EnemyBullet : public Bullet {
    Enemy::Type _type;
    // Bullet interface
public:
    EnemyBullet(Enemy* enemy)
        : Bullet(enemy)
        , _type(enemy->type()){

    }

    health_type power() const override {
        return 1; // enough to kill the player
    }
    Player *player() const override {
        return nullptr;
    }
    velocity_type speed() const override {
        return _type != Enemy::Type::Power
                ? velocity_type(2,1)
                : velocity_type(4,1);
    }
};

Enemy::Enemy(Type type, bool bonus, Scene* scene)
    : Tank(scene)
    , _type(type)
    , _health(_type == Type::Armor ? 4 : 1)
    , _isBonus(bonus)
    , _respawn_timeout(60)
{
    move();
}

void Enemy::fire()
{
    if(bulletsCount() == 0)
        addBullet(std::make_shared<EnemyBullet>(this));
}

velocity_type Enemy::speed() const
{
    static const velocity_type speeds[] = {{1,4}, {3,4}, {2,4}, {2,4}};
    return speeds[static_cast<int>(_type)];
}

score_type Enemy::scoreForKill() const
{
    static const score_type points[] = {100, 200, 300, 400};
    return points[static_cast<int>(_type)];
}

void Enemy::process(time_type dt)
{
    // hold on respawn
    if(_respawn_timeout) {
        const auto min = std::min(dt, _respawn_timeout);
        _respawn_timeout -= min;
        dt -= min;
        if(!dt)
            return;
    }

    if(scene()->enemiesFreezed())
        return;

    for(; dt>0; --dt) {
        Tank::process(1);

        // 1/32 to fire
        if(std::rand() % 32 == 0)
            fire();
    }
}

bool Enemy::processHit(Bullet * bullet)
{
    if(isSpawning())
        return false;   // can't hit while spawning

    if(!bullet->isPlayer())
        return false;

    const auto& power = bullet->power();
    if(_health <= power) {
        _health = 0;
        scene()->tankExplode(this);
        scene()->killEnemy(this, bullet->player());
    } else {
        _health -= power;
    }
    return true;
}

bool Enemy::turnRandomDirection()
{
    std::vector<direction_type> availableDirections;
    for(const auto& direction : {direction_type::Left, direction_type::Right, direction_type::Up, direction_type::Down}) {
        if(canMove(direction))
            availableDirections.push_back(direction);
    }

    if(!availableDirections.empty()) {
        turn(availableDirections[rand() % availableDirections.size()]);
        return false;
    }

    return true;
}

field_rect_type Enemy::occupiedCells() const
{
    if(isSpawning())
        return field_rect_type();

    return Tank::occupiedCells();
}

void Enemy::arrivedToCell(const field_vector_type &/*newCell*/)
{
    // AI here
    if(rand()%16 == 0)
        changeDirection();

}

void Enemy::hitObstacle()
{
    turnRandomDirection();
}

void Enemy::changeDirection()
{
    // from https://habrahabr.ru/post/142126/
    const auto periodDuration = scene()->enemyRespawnTime()*60 / 8;

    if (scene()->roundTime() < periodDuration)
    {
        // first period
        turnRandomDirection();
        resetMoveTarget();
    }
    else {
        const auto& players = scene()->players();
        if (scene()->roundTime() < periodDuration * 2 && !players.empty())
        {
            const auto& player = players[_id % players.size()];
            setMoveTarget(player->cell());
        }
        else
        {
            // third period
            setMoveTarget(scene()->eagle().fieldRect().topLeft());
        }
    }

    if(_targetIsSet) {
        const auto nexDirection = directionToGetTo(_target);
        if(canMove(nexDirection))
            turn(nexDirection);
    }
}

void Enemy::resetMoveTarget()
{
    _targetIsSet = false;
}

void Enemy::setMoveTarget(const field_vector_type &target)
{
    _targetIsSet = true;
    _target = target;
}

direction_type Enemy::directionToGetTo(const field_vector_type &target) const
{
    const auto& cell = this->cell();
    const field_vector_type abs_delta(target.x < cell.x
                                            ? cell.x - target.x
                                            : target.x - cell.x,
                                           target.x < cell.y
                                            ? cell.y - target.y
                                            : target.y - cell.y);
    if(abs_delta.x > abs_delta.y) {
        return target.x < cell.x
                ? direction_type::Type::Left
                : direction_type::Type::Right;
    }

    return target.y < cell.y
            ? direction_type::Type::Up
            : direction_type::Type::Down;
}
