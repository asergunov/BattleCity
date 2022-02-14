#pragma once

#include <QObject>

#include "ActorList.hpp"

namespace battlecity {
namespace engine {
class Player;
}}

class PlayerController : public QObject
{
    Q_OBJECT
public:
    using Direction = ActorList::Direction;

    explicit PlayerController(battlecity::engine::Player *player,  QObject *parent = nullptr);

public slots:
    void turn(int direction);   // Workaround: type Direction should be here but metasystem can't recognize it. Have no idea why.
    void fire();
    void move();
    void stop();

private:
    battlecity::engine::Player *m_player;
};
