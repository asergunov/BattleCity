#pragma once

#include "TankList.hpp"

namespace battlecity {
namespace engine {
class Enemy;
}}


class EnemyList : public TankList
{
    Q_OBJECT
public:
    enum Roles {
        TypeRole = TankList::UserRole,
        BonusRole,
        HealthRole,
        SpawningRole,
        UserRole
    };

    enum Type {
        Basic, Fast, Power, Armor
    };
    Q_ENUM(Type)
public:

    explicit EnemyList(QObject *parent = nullptr);

    void add(const battlecity::engine::Enemy* enemy);
    void remove(const battlecity::engine::Enemy* enemy);
    const battlecity::engine::Enemy* at(int row)  const;


    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
signals:

public slots:
};
