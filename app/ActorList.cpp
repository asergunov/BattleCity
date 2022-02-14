#include "ActorList.hpp"

#include "typeConvertion_p.hpp"

#include <engine/Actor.hpp>

ActorList::ActorList(QObject *parent)
    : SceneObjectList(parent)
{

}

void ActorList::add(const battlecity::engine::Actor *actor)
{
    SceneObjectList::add(actor);
}

void ActorList::remove(const battlecity::engine::Actor *actor)
{
    SceneObjectList::remove(actor);
}

const battlecity::engine::Actor *ActorList::at(int row) const
{
    return static_cast<const battlecity::engine::Actor*>(SceneObjectList::at(row));
}

QHash<int, QByteArray> ActorList::roleNames() const
{
    auto names = SceneObjectList::roleNames();
    names.insert(DirectionRole, "direction");
    return names;
}

QVariant ActorList::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());
    switch (role) {
    case DirectionRole:
        return fromEngine(at(index.row())->direction());
    }
    return SceneObjectList::data(index, role);
}
