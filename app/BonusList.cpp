#include "BonusList.hpp"

#include "typeConvertion_p.hpp"

#include <engine/Bonus.hpp>

BonusList::BonusList(QObject *parent)
    : SceneObjectList(parent)
{

}

void BonusList::add(const battlecity::engine::Bonus *bonus)
{
    SceneObjectList::add(bonus);
}

void BonusList::remove(const battlecity::engine::Bonus *bonus)
{
    SceneObjectList::remove(bonus);
}

const battlecity::engine::Bonus *BonusList::at(int row) const
{
    return static_cast<const battlecity::engine::Bonus*>(SceneObjectList::at(row));
}

QVariant BonusList::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());
    const auto& bonus = at(index.row());
    switch(role) {
    case TypeRole:
        return fromEngine(bonus->type());
    }

    return SceneObjectList::data(index, role);
}

QHash<int, QByteArray> BonusList::roleNames() const
{
    auto names = SceneObjectList::roleNames();
    names.insert(TypeRole, "type");
    return names;
}
