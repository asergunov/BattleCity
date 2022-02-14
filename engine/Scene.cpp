#include "Scene.hpp"
#include "Enemy.hpp"

#include <sstream>
#include <algorithm>
#include <cassert>
#include <memory>

using namespace battlecity::engine;

namespace {

struct ClassicLevel {
    Field field;
    std::vector<std::pair<Enemy::Type, size_t>> enemies;
};

const std::vector<ClassicLevel> levels {
    { Field::fromClassicCoding(
        {
            {0xFFFFFFFFFFFFF,
             0xF4F4F4F4F4F4F,
             0xF4F4F4F4F4F4F,
             0xF4F4F494F4F4F,
             0xF4F4F3F3F4F4F,
             0xF3F3F1F1F3F3F,
             0x1F11F3F3F11F1,
             0x8F33F1F1F33F8,
             0xF1F1F444F1F1F,
             0xF4F4F4F4F4F4F,
             0xF4F4F3F3F4F4F,
             0xF4F4FFFFF4F4F,
             0xFFFFFFFFFFFFF}} ),
        {
            {Enemy::Type::Basic, 18},
            {Enemy::Type::Fast, 2},
        }
    },{ Field::fromClassicCoding(
        {
            {0xfff9fff9fffff,
             0xf4f9fff4f4f4f,
             0xf4ffff44f494f,
             0xfff4fffff9fff,
             0xbff4fffff4b49,
             0xbbfff4ff9fbff,
             0xf444bbb9ffb4f,
             0xfff9b4f4f4f4f,
             0x94f9f4f4fff4f,
             0xf4f4f444f494f,
             0xf4f4f444fffff,
             0xf4fffffff4f4f,
             0xf4f4fffff444f}} ),
        {
            {Enemy::Type::Armor, 2},
            {Enemy::Type::Fast, 4},
            {Enemy::Type::Basic, 14}
        }
    },{ Field::fromClassicCoding(
        {
            {0xffff4fff4ffff,
             0xfbbb4fffff666,
             0x4bbbfffffffff,
             0xbbbbfff4f4442,
             0xbbbb4443f4f0f,
             0xbbbbff4ffff0f,
             0xfbffff999ffbf,
             0xf1f1fffffbbbb,
             0x420420333bbbb,
             0xfffff4f11bbbb,
             0x4ff7fff33bbbf,
             0x44f7fffffbbbf,
             0x944ffffff4fff}} ),
        {
            {Enemy::Type::Basic, 14},
            {Enemy::Type::Fast, 4},
            {Enemy::Type::Armor, 2}
        }
    },
    { Field::fromClassicCoding(
        {
            {0xfbbffffffffbf,
             0xbbff14411fffb,
             0xbff04444441f8,
             0x8ff444444442f,
             0xff03fff344f2f,
             0xaf0f7f7f42fff,
             0xff4f11ff42faa,
             0xff44444444fff,
             0xf0444444442ff,
             0xf3344444433ff,
             0xf4413443144fb,
             0xbf33ffff33fbb,
             0x9bffffffffbb9}} ),
        {
            {Enemy::Type::Power, 10},
            {Enemy::Type::Fast, 5},
            {Enemy::Type::Basic, 2},
            {Enemy::Type::Armor, 3}
        }
    },
    { Field::fromClassicCoding(
        {
            {0xffff44fffffff,
             0x6f1f4fff889ff,
             0x9f4fff4ffffff,
             0x4f444f44faafa,
             0x3fff3ffffafff,
             0xff1faafaaaf44,
             0x44ffa4f42ffff,
             0xffffafffff57f,
             0xaaafaf9f4f5ff,
             0xfff11fffff544,
             0xffff433341fff,
             0x443ffffff34ff,
             0x3ffffffffffff}} ),
        {
            {Enemy::Type::Power, 5},
            {Enemy::Type::Armor, 2},
            {Enemy::Type::Basic, 8},
            {Enemy::Type::Fast, 5}
        }
    }
//    { Field::fromClassicCoding(
//        {
//            {0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff,
//             0xfffffffffffff}} ),
//        {
//            {Enemy::Type::Basic, 14},
//            {Enemy::Type::Fast, 4},
//            {Enemy::Type::Armor, 2}
//        }
//    }
};

}

Scene::Scene(Scene::Listener *listener)
 : _listener(listener)
 , _bonus(this)
 , _eagle(this)
{

}

Scene::~Scene()
{

}

void Scene::startLevel(int level)
{
    _bullets.clear();
    _enemies.clear();
    _bonus = Bonus(this); // clear bonus

    _eagle.respawn({{12, 24}, {2,2}});
    _player_respawn_points = {{9, 24}, {13, 24}};
    _enemy_respawn_points = {{0,0}, {12,0}, {24,0}};

    for(auto& player : _players)
        player->respawn();

    {
        _bonus_respawn_cells.clear();
        for(field_coordinate_type x = 3; x < 20; x+=6) {
            for(field_coordinate_type y = 3; y < 20; y+=6) {
                _bonus_respawn_cells.emplace_back(x,y);
            }
        }
    }

    const auto& classicLevel = levels[level % levels.size()];
    _field = classicLevel.field;

    _enemy_bonus = {17, 10, 3};
    {
        while(!_enemies_to_spawn.empty()) _enemies_to_spawn.pop();

        auto put = [&](size_t count, Enemy::Type type) {
            for(size_t i = 0; i < count; ++i)
                _enemies_to_spawn.push(type);
        };

        for(const auto e: classicLevel.enemies)
            put(e.second, e.first);
    }
    _round_time = 0;

    _level = level;
    _time_to_respawn = enemyRespawnTime();
}

void Scene::restart(int level, int playerCount)
{
    _gameOver = false;

    _players.clear();

    startLevel(level);

    if(playerCount < 1 || playerCount > _player_respawn_points.size()) {
        throw worng_player_count(1, _player_respawn_points.size(), playerCount);
    }

    while(playerCount > _players.size()) {
        _players.push_back(std::make_shared<Player>(
                             _player_respawn_points[_players.size()],
                             this));
    }
}

void Scene::nextLevel()
{
    startLevel(_level+1);
}

void Scene::tankExplode(Tank *tank)
{
    _listener->tankExploded(tank);
}

void Scene::tankFriendlyFireFreeze(Tank *)
{
    // TODO: to implement
}

void Scene::bulletExplode(Bullet *bullet)
{
    for(auto i = _bullets.begin(); i != _bullets.end(); ++i) {
        if(i->get() == bullet) {
            _listener->bulletExploded(bullet);
            _bullets.erase(i);
            break;
        }
    }
}

void Scene::killEnemy(Enemy* enemy, Player *player)
{
    _listener->scoreForEnemy(player, enemy);

    if(enemy->isBonus()) {
        if(_bonus.isValid()) {
            _listener->bonusRemoved(&_bonus);
        }

        _bonus = Bonus::createRandom(this, _bonus_respawn_cells);
        _listener->bonusCreated(&_bonus);
    }

    eraseEnemy(enemy);
}

void Scene::killPlayer(Player *player)
{
    player->respawn();
    if(std::all_of(_players.begin(), _players.end(), [](const std::shared_ptr<Player>& player){
                   return player->isKilled();
    })) {
        setGameOver(true);
    }
}

void Scene::killEagle()
{
    _eagle.kill();
    setGameOver(true);
}

void Scene::eraseEnemy(Enemy *enemy)
{
    auto i_found = std::find_if(_enemies.begin(), _enemies.end(),[&](const std::shared_ptr<Enemy>& _enemy){
        return _enemy.get() == enemy;
    });

    if(i_found == _enemies.end())
        return;

    _listener->enemyErased(enemy);
    _enemies.erase(i_found);

    if(_enemies_to_spawn.empty() && _enemies.empty()) {
        _listener->levelCompleted();
    }
}

void Scene::setGameOver(bool gameOver)
{
    if(_gameOver != gameOver) {
        _gameOver = gameOver;
        if(gameOver)
            _listener->gameOver();
    }
}

void Scene::addBullet(std::shared_ptr<Bullet> bullet, Tank */*tank*/)
{
    _bullets.insert(bullet);
    _listener->bulletSpawned(bullet.get());
}

Tank *Scene::tankAtCell(field_vector_type cell) const
{
    for(auto& tank : _players) {
        const auto& tankCells = tank->occupiedCells();
        if( tankCells.contains(cell) ) {
            return tank.get();
        }
    }

    for(auto& tank : _enemies) {
        const auto& tankCells = tank->occupiedCells();
        if( tankCells.contains(cell) ) {
            return tank.get();
        }
    }
    return nullptr;
}

const Bonus *Scene::bonusAt(const field_rect_type &rect) const
{
    if(_bonus.isValid() && _bonus.fieldRect().intersects(rect))
        return &_bonus;

    return nullptr;
}

void Scene::applyBonus(Player *player, const Bonus *bonus)
{
    assert(bonus);
    assert(bonus == &_bonus);

    _listener->bonusRemoved(bonus);
    _listener->bonusTaken(player, bonus);

    switch (bonus->type()) {
    case Bonus::Type::Granade: {
        const auto enemies = _enemies;
        for(const auto& enemy : enemies) {
            _listener->tankExploded(enemy.get());
            eraseEnemy(enemy.get());
        }
    } break;
    case Bonus::Type::Helmet:
        player->setUndestroyableFor(60*10);
        break;
    case Bonus::Type::Shovel:
        break;
    case Bonus::Type::Star:
        player->levelUp();
        break;
    case Bonus::Type::Tank:
        player->addLife();
        break;
    case Bonus::Type::Timer:
        _enemy_freeze_timer = 10*60;
        break;
    default:
        assert(false); // unreachable
    }

    _bonus = Bonus(this); // empty one
}

bool Scene::cellsAreFreeForTank(const field_rect_type &cells, const Tank *tank) const
{
    const auto& x_begin = cells.left();
    const auto& x_end = x_begin + cells.width();

    const auto& y_begin = cells.top();
    const auto& y_end = y_begin + cells.height();

    for(auto x = x_begin; x < x_end; ++x) for(auto y = y_begin; y < y_end; ++y) {
        if(_field.at(x,y).isBuisyForTank())
            return false;

        const auto& otherTank = tankAtCell({x,y});
        if(otherTank && tank != otherTank)
            return false;
    }

    return true;
}

size_t Scene::maxEnemiesOnBoard() const
{
    return 2 + _players.size()*2;
}

time_type Scene::enemyRespawnTime() const
{
    return 190 - level() * 4 - (_players.size() - 1) * 20;
}

time_type Scene::roundTime() const
{
    return _round_time;
}

int Scene::level() const
{
    return _level;
}

int Scene::levelCount() const
{
    return levels.size();
}

size_t Scene::enemiesRemain() const
{
    return _enemies_to_spawn.size();
}

std::vector<Player *> Scene::players() const
{
    std::vector<Player *> result;
    for(const auto& player : _players) {
        result.push_back(player.get());
    }
    return result;
}

void Scene::process(time_type dt)
{
    _round_time += dt;
    _enemy_freeze_timer -= std::min(dt, _enemy_freeze_timer);

    {
        const auto actors = _players;
        for(const auto& actor : actors)
            actor->process(dt);
    }

    {
        const auto actors = _enemies;
        for(const auto& actor : actors)
            actor->process(dt);
    }

    {
        const auto actors = _bullets;
        for(const auto& actor : actors)
            actor->process(dt);
    }

    if(_enemies.size() < maxEnemiesOnBoard() && !_enemies_to_spawn.empty()) {
        _time_to_respawn = std::max(0, _time_to_respawn-dt);
        if(_time_to_respawn == 0) {
            spawnEnemy();
            _time_to_respawn = enemyRespawnTime();
        }
    }
}

void Scene::spawnEnemy()
{
    assert(!_enemies_to_spawn.empty());

    const auto point = _enemy_respawn_points[rand()%_enemy_respawn_points.size()];
    bool bonus = _enemy_bonus.find(_enemies_to_spawn.size()) != _enemy_bonus.end();
    const auto enemy = std::make_shared<Enemy>(_enemies_to_spawn.front(), bonus, this);
    enemy->respawn(point, direction_type::Down);

    _enemies_to_spawn.pop();

    _enemies.insert(enemy);
    _listener->enemySpawned(enemy.get());
}

coordinate_type Scene::fieldCellSize() const
{
    return 8;
}

coordinate_rect_type Scene::mapFromField(const field_rect_type &rect)
{
    return {
        mapFromField(rect.topLeft()),
        mapFromField(rect.size()),
    };
}

coordinate_vector_type Scene::mapFromField(const field_vector_type &cell)
{
    return coordinate_vector_type(cell)*fieldCellSize();
}

Scene::worng_player_count::worng_player_count(int min, int max, int count)
    :std::runtime_error([&]{
    std::stringstream stream;
    stream << "player count should be from " << min << " to " << max << ". " << count << " provided.";
    return stream.str();
}())
{

}
