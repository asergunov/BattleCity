#pragma once

#include "Field.hpp"
#include "Enemy.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "Bonus.hpp"
#include "Eagle.hpp"

#include <vector>
#include <unordered_set>
#include <queue>
#include <set>

namespace battlecity {
namespace engine {

class Scene
{
public:
    struct worng_player_count : public std::runtime_error {
        worng_player_count(int min, int max, int count);
    };

    class Listener {
        Listener(const Listener&) = delete;
        Listener& operator=(const Listener&) = delete;
    protected:
        Listener() = default;
        ~Listener() = default;
    public:
        virtual void tankExploded(Tank*) = 0;
        virtual void bulletExploded(Bullet*) = 0;

        virtual void scoreForEnemy(Player*, Enemy*) = 0;
        virtual void scoreForBonus(Player*, Bonus*) = 0;

        virtual void enemySpawned(Enemy*) = 0;
        virtual void enemyErased(Enemy*) = 0;

        virtual void bulletSpawned(Bullet*) = 0;

        virtual void bonusCreated(const Bonus*) = 0;
        virtual void bonusRemoved(const Bonus*) = 0;
        virtual void bonusTaken(const Player*, const Bonus*) = 0;

        virtual void gameOver() = 0;
        virtual void levelCompleted() = 0;
    };

    using bullets_container = std::unordered_set<std::shared_ptr<Bullet>>;

public:
    Scene(Listener* listener);
    ~Scene();

    void startLevel(int level);
    void restart(int level, int playerCount);
    void nextLevel();
    void process(time_type dt);

    void tankExplode(Tank*);
    void tankFriendlyFireFreeze(Tank*);
    void bulletExplode(Bullet*);
    void killEnemy(Enemy *, Player *);
    void killPlayer(Player *);
    void killEagle();
    void addBullet(std::shared_ptr<Bullet> bullet, Tank* tank);
    void applyBonus(Player* player, const Bonus* bonus);

    Tank* tankAtCell(field_vector_type cell) const;
    const Bonus* bonusAt(const field_rect_type& rect) const;

    bool cellsAreFreeForTank(const field_rect_type& cells, const Tank *tank) const;

    size_t maxEnemiesOnBoard() const;
    time_type enemyRespawnTime() const;
    time_type roundTime() const;
    int level() const;
    int levelCount() const;
    size_t enemiesRemain() const;
    bool enemiesFreezed() const { return _enemy_freeze_timer > 0; }
    bool gameOver() const { return _gameOver; }

    std::vector<Player*> players() const;
    const bullets_container& bullets() const { return _bullets; }

    Field& field() { return _field; }
    const Field& field() const { return _field; }
    coordinate_type fieldCellSize() const;
    coordinate_rect_type mapFromField(const field_rect_type& rect);
    coordinate_vector_type mapFromField(const field_vector_type& cell);
    const Eagle& eagle() const { return _eagle; }

private:
    void spawnEnemy();
    void eraseEnemy(Enemy *enemy);
    void setGameOver(bool gameOver);
private:
    bool _gameOver = false;
    Listener* _listener;
    Field _field;

    std::unordered_set<std::shared_ptr<Enemy>> _enemies;
    std::vector<std::shared_ptr<Player>>  _players;
    bullets_container _bullets;

    std::vector<field_vector_type> _player_respawn_points;
    std::vector<field_vector_type> _enemy_respawn_points;
    std::vector<field_vector_type> _bonus_respawn_cells;

    time_type _round_time = 0;
    int _level = 0;

    std::queue<Enemy::Type> _enemies_to_spawn;
    std::set<size_t> _enemy_bonus;  ///< bonus tank generated if these number of tanks remains
    time_type _time_to_respawn = 0;
    time_type _enemy_freeze_timer = 0;
    Bonus _bonus;
    Eagle _eagle;
};


}}
