#include "PlayerList.hpp"

#include "PlayerController.hpp"

#include <engine/Player.hpp>

PlayerList::PlayerList(QObject *parent)
    : TankList(parent)
{

}

void PlayerList::add(battlecity::engine::Player *player)
{
    if(!m_controllers.contains(player)) {
        auto controller = new PlayerController(player, this);
        m_controllers.insert(player, controller);
    }
    TankList::add(player);
}

void PlayerList::remove(const battlecity::engine::Player *player)
{
    TankList::remove(player);
    auto found = m_controllers.find(player);
    if(found != m_controllers.end()) {
        delete found.value();
        m_controllers.erase(found);
    }
}

const battlecity::engine::Player *PlayerList::at(int row) const
{
    return static_cast<const battlecity::engine::Player *>(TankList::at(row));
}

void PlayerList::clear()
{
    TankList::clear();
    qDeleteAll(m_controllers);
    m_controllers.clear();
}

QHash<int, QByteArray> PlayerList::roleNames() const
{
    auto names = TankList::roleNames();
    names.insert(LevelRole, "level");
    names.insert(UndestroyableRole, "undestroyable");
    names.insert(LifeCountRole, "lifeCount");
    names.insert(ControllerRole, "controller");
    names.insert(KilledRole, "killed");
    return names;
}

QVariant PlayerList::data(const QModelIndex &index, int role) const
{
    const auto& player = at(index.row());
    switch(role) {
    case LevelRole:
        return static_cast<int>(player->level());
    case UndestroyableRole:
        return player->isUndestroyable();
    case LifeCountRole:
        return static_cast<int>(player->lifeCount());
    case ControllerRole: {
        const auto found = m_controllers.find(player);
        if(found == m_controllers.end())
            return {};
        return QVariant::fromValue<PlayerController*>(found.value());
    }
    case KilledRole:
        return player->isKilled();
    }

    return TankList::data(index, role);
}
