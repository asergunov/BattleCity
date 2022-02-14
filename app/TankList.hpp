#pragma once

#include "ActorList.hpp"

namespace battlecity {
namespace engine {
class Tank;
}}


class TankList : public ActorList
{
    Q_OBJECT
public:
    enum Roles {
        MovingRole = ActorList::UserRole,
        UserRole
    };

public:

    explicit TankList(QObject *parent = nullptr);

    void add(const battlecity::engine::Tank* tank);
    void remove(const battlecity::engine::Tank* tank);
    const battlecity::engine::Tank* at(int row)  const;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
signals:

public slots:
};

