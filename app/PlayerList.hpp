#pragma once

#include "TankList.hpp"

#include <QHash>

namespace battlecity {
namespace engine {
class Player;
}}

class PlayerController;

class PlayerList : public TankList
{
    Q_OBJECT
public:
    enum Roles {
        LevelRole = TankList::UserRole,
        UndestroyableRole,
        LifeCountRole,
        ControllerRole,
        KilledRole,
        UserRole
    };

public:
    explicit PlayerList(QObject *parent = nullptr);

    void add(battlecity::engine::Player* player);
    void remove(const battlecity::engine::Player* player);
    const battlecity::engine::Player* at(int row)  const;
    void clear();

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
signals:

public slots:
private:
    QHash<const battlecity::engine::Player*, PlayerController*> m_controllers;
};
