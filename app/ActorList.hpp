#pragma once

#include "SceneObjectList.hpp"

namespace battlecity {
namespace engine {
class Actor;
}}

class ActorList : public SceneObjectList
{
    Q_OBJECT
public:
    enum Roles {
        DirectionRole = SceneObjectList::UserRole,
        UserRole
    };

    enum Direction {
        Up, Left, Down, Right
    };
    Q_ENUM(Direction)

public:
    explicit ActorList(QObject *parent = nullptr);

    void add(const battlecity::engine::Actor* actor);
    void remove(const battlecity::engine::Actor* actor);
    const battlecity::engine::Actor* at(int row)  const;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;
signals:

public slots:
};
