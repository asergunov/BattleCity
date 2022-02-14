#include "PlayerController.hpp"

#include "typeConvertion_p.hpp"

#include <engine/Player.hpp>

PlayerController::PlayerController(battlecity::engine::Player *player, QObject *parent)
    : QObject(parent)
    , m_player(player)
{
    Q_CHECK_PTR(player);
}

void PlayerController::turn(int direction)
{
    m_player->turn( toEngine(static_cast<ActorList::Direction>( direction )) );
}

void PlayerController::fire()
{
    m_player->fire();
}

void PlayerController::move()
{
    m_player->move();
}

void PlayerController::stop()
{
    m_player->stop();
}
