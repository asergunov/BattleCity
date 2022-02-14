#include "TankList.hpp"

#include <engine/Tank.hpp>

TankList::TankList(QObject *parent)
    : ActorList(parent)
{

}

void TankList::add(const battlecity::engine::Tank *tank)
{
    ActorList::add(tank);
}

void TankList::remove(const battlecity::engine::Tank *tank)
{
    ActorList::remove(tank);
}

const battlecity::engine::Tank *TankList::at(int row) const
{
    return static_cast<const battlecity::engine::Tank *>(ActorList::at(row));
}

QHash<int, QByteArray> TankList::roleNames() const
{
    auto result = ActorList::roleNames();
    result.insert(MovingRole, "moving");
    return result;
}

QVariant TankList::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());
    const auto& tank = at(index.row());
    switch (role) {
    case MovingRole:
        return tank->isMoving();
    }
    return ActorList::data(index, role);
}
