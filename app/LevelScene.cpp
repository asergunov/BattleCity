#include "LevelScene.hpp"

#include "Field.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "Bonus.hpp"

#include "typeConvertion_p.hpp"

#include <QTimer>
#include <Scene.hpp>
#include <QDebug>

#include <cassert>

using namespace battlecity;


class LevelScene::SceneListener : public engine::Scene::Listener {
    LevelScene* q_ptr;
public:
    SceneListener(LevelScene* q)
        : q_ptr(q) {
    }

    // Listener interface
public:
    void tankExploded(engine::Tank *) override;
    void bulletExploded(engine::Bullet *) override;
    void scoreForEnemy(engine::Player *, engine::Enemy *) override;
    void enemySpawned(engine::Enemy *) override;
    void enemyErased(engine::Enemy *) override;
    void bulletSpawned(engine::Bullet *) override;
    void scoreForBonus(engine::Player *, engine::Bonus *) override;
    void bonusCreated(const engine::Bonus *) override;
    void bonusRemoved(const engine::Bonus *) override;
    void bonusTaken(const engine::Player *, const engine::Bonus*) override;
    void gameOver() override;
    void levelCompleted() override;
};

LevelScene::LevelScene(QObject *parent)
    : QObject(parent)
    , m_listener(new SceneListener(this))
    , m_scene(new engine::Scene(m_listener.get()))
    , m_players(new PlayerList(this))
    , m_field(new Field(m_scene->field(), this))
    , m_bullets(new ActorList(this))
    , m_enemies(new EnemyList(this))
    , m_bonuses(new BonusList(this))
    , m_eagle(new Eagle(&m_scene->eagle(), this))
    , m_enemiesRemain(m_scene->enemiesRemain())
{
    auto timer = new QTimer(this);
    timer->setTimerType(Qt::PreciseTimer);
    timer->start(1000/60);

    connect(timer, &QTimer::timeout, this, [this]{
        if(m_paused)
            return;

        m_scene->process(1);
        emit sceneProcessed();
        update();
    });

    m_field->reset();
}

LevelScene::~LevelScene()
{

}

int LevelScene::cellSize() const
{
    return m_scene->fieldCellSize();
}

int LevelScene::levelCount() const
{
    return m_scene->levelCount();
}

void LevelScene::startLevel(int level, int players)
{
    m_players->clear();

    clenupWrappers();
    m_scene->restart (level, players);
    fillupWrappers();

    for(const auto& player : m_scene->players())
        m_players->add(player);
}

void LevelScene::startNextLevel()
{
    clenupWrappers();
    m_scene->nextLevel();
    fillupWrappers();
}

void LevelScene::setPaused(bool paused)
{
    if (m_paused == paused)
        return;

    m_paused = paused;
    emit pausedChanged(m_paused);
}

void LevelScene::update()
{
    m_players->updated();
    m_bullets->updated();
    m_enemies->updated();
    m_bonuses->updated();
    m_field->reduce();
    m_eagle->update();

    {
        const int value = static_cast<int>(m_scene->enemiesRemain());
        if(value != m_enemiesRemain) {
            m_enemiesRemain = value;
            emit enemiesRemainChanged(value);
        }
    }

    {
        const int level = m_scene->level() + 1;
        if(level != m_level) {
            m_level = level;
            emit levelChanged(level);
        }
    }
}

void LevelScene::clenupWrappers()
{
    m_bonuses->clear();
    m_bullets->clear();
    m_enemies->clear();
}

void LevelScene::fillupWrappers()
{
    m_field->reset();
    update();
}

void LevelScene::SceneListener::tankExploded(engine::Tank *tank) {
    emit q_ptr->tankExplosion( toPoint( tank->position() ) );
}

void LevelScene::SceneListener::bulletExploded(engine::Bullet *bullet)
{
    q_ptr->m_bullets->remove(bullet);
    emit q_ptr->bulletExplosion( toPoint( bullet->position() ) );
}

void LevelScene::SceneListener::scoreForEnemy(engine::Player *, engine::Enemy *enemy)
{
    emit q_ptr->score( toPoint(enemy->position()), enemy->scoreForKill() );
}

void LevelScene::SceneListener::enemySpawned(engine::Enemy *enemy)
{
    q_ptr->m_enemies->add(enemy);
}

void LevelScene::SceneListener::enemyErased(engine::Enemy *enemy)
{
    q_ptr->m_enemies->remove(enemy);
}

void LevelScene::SceneListener::bulletSpawned(engine::Bullet *bullet)
{
    q_ptr->m_bullets->add(bullet);
}

void LevelScene::SceneListener::scoreForBonus(engine::Player *, engine::Bonus *)
{

}

void LevelScene::SceneListener::bonusCreated(const engine::Bonus *bonus)
{
    q_ptr->m_bonuses->add(bonus);
}

void LevelScene::SceneListener::bonusRemoved(const engine::Bonus *bonus)
{
    q_ptr->m_bonuses->remove(bonus);
}

void LevelScene::SceneListener::bonusTaken(const engine::Player *, const engine::Bonus *bonus)
{
    emit q_ptr->score(toPoint(bonus->position()), bonus->scoreToTake());
}

void LevelScene::SceneListener::gameOver()
{
    emit q_ptr->gameOver();
}

void LevelScene::SceneListener::levelCompleted()
{
    emit q_ptr->levelCompleted();
}
