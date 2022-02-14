#pragma once

#include <QObject>

#include "Eagle.hpp"
#include "Field.hpp"
#include "PlayerList.hpp"
#include "EnemyList.hpp"
#include "ActorList.hpp"
#include "BonusList.hpp"

#include <QObjectList>
#include <QPoint>
#include <QQmlListProperty>

#include <memory>

namespace battlecity {
namespace engine {
class Scene;
}}

class Field;
class Player;
class Enemy;
class Actor;
class Bonus;

/**
 * @brief The LevelScene class
 *
 * Qt wrapper for battlecity::engine::Scene
 */
class LevelScene : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PlayerList* players READ players CONSTANT)
    Q_PROPERTY(EnemyList* enemies READ enemies CONSTANT)
    Q_PROPERTY(ActorList* bullets READ bullets CONSTANT)
    Q_PROPERTY(BonusList* bonuses READ bonuses CONSTANT)

    Q_PROPERTY(Field* field READ field CONSTANT)
    Q_PROPERTY(Eagle* eagle READ eagle CONSTANT)

    Q_PROPERTY(int cellSize READ cellSize CONSTANT)
    Q_PROPERTY(int enemiesRemain READ enemiesRemain NOTIFY enemiesRemainChanged)
    Q_PROPERTY(int level READ level NOTIFY levelChanged)
    Q_PROPERTY(int levelCount READ levelCount CONSTANT)
    Q_PROPERTY(bool paused READ paused WRITE setPaused NOTIFY pausedChanged)
public:
    explicit LevelScene(QObject *parent = nullptr);
    ~LevelScene();

    PlayerList* players() const { return m_players; }
    ActorList* bullets() const { return m_bullets; }
    EnemyList* enemies() const { return m_enemies; }
    BonusList* bonuses() const { return m_bonuses; }
    Field* field() const { return m_field; }
    int cellSize() const;
    bool paused() const { return m_paused; }
    const int& enemiesRemain() const { return m_enemiesRemain; }
    const int& level() const { return m_level; }
    Eagle* eagle() const { return m_eagle; }

    int levelCount() const;

signals:
    void sceneProcessed();
    void enemiesRemainChanged(int enemiesRemain);
    void levelChanged(int level);
    void levelCompleted();
    void gameOver();

    void tankExplosion(QPoint position);
    void bulletExplosion(QPoint position);
    void score(QPoint position, int score);


    void pausedChanged(bool paused);

public slots:
    void startLevel(int level, int players);
    void startNextLevel();
    void setPaused(bool paused);

protected:
    void update();
    void clenupWrappers();
    void fillupWrappers();

private:
    class SceneListener;
    friend class SceneListener;
    std::unique_ptr<SceneListener> m_listener;
    std::unique_ptr<battlecity::engine::Scene> m_scene;

    PlayerList* m_players;
    Field* m_field;
    ActorList* m_bullets;
    EnemyList* m_enemies;
    BonusList* m_bonuses;
    Eagle* m_eagle;
    int m_enemiesRemain = 0;
    int m_level = 0;
    bool m_paused = false;
};

