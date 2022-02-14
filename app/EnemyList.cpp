#include "EnemyList.hpp"

#include "typeConvertion_p.hpp"

#include <engine/Enemy.hpp>

EnemyList::EnemyList(QObject *parent)
    : TankList(parent)
{

}

void EnemyList::add(const battlecity::engine::Enemy *enemy)
{
    TankList::add(enemy);
}

void EnemyList::remove(const battlecity::engine::Enemy *enemy)
{
    TankList::remove(enemy);
}

const battlecity::engine::Enemy *EnemyList::at(int row) const
{
    return static_cast<const battlecity::engine::Enemy *>(TankList::at(row));
}

QHash<int, QByteArray> EnemyList::roleNames() const
{
    auto names = TankList::roleNames();
    names.insert(TypeRole, "type");
    names.insert(BonusRole, "bonus");
    names.insert(HealthRole, "health");
    names.insert(SpawningRole, "spawning");
    return names;
}

QVariant EnemyList::data(const QModelIndex &index, int role) const
{
    const auto& enemy = at(index.row());
    switch (role) {
    case TypeRole:
        return fromEngine(enemy->type());
    case BonusRole:
        return enemy->isBonus();
    case HealthRole:
        return static_cast<int>(enemy->health());
    case SpawningRole:
        return enemy->isSpawning();
    }

    return TankList::data(index, role);
}
