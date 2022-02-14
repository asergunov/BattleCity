#include "SceneObjectList.hpp"

#include <engine/SceneObject.hpp>

#include "typeConvertion_p.hpp"

SceneObjectList::SceneObjectList(QObject *parent)
    : QAbstractListModel(parent)
{

}

void SceneObjectList::add(const battlecity::engine::SceneObject *o)
{
    Q_CHECK_PTR(o);
    if(m_list.contains(o))
        return;
    const int row = m_list.size();
    beginInsertRows({}, row, row);
    m_list.push_back(o);
    endInsertRows();
}

void SceneObjectList::remove(const battlecity::engine::SceneObject *o)
{
    for(auto i = m_list.begin(); i != m_list.end(); ) {
        if(*i == o) {
            const int row = std::distance(m_list.begin(), i);
            beginRemoveRows({}, row, row);
            i = m_list.erase(i);
            endRemoveRows();
        } else {
            ++i;
        }
    }
}

const battlecity::engine::SceneObject *SceneObjectList::at(int row) const
{
    Q_ASSERT(row >=0 && row < m_list.size());
    return m_list.at(row);
}

void SceneObjectList::clear()
{
    beginResetModel();
    m_list.clear();
    endResetModel();
}

void SceneObjectList::updated()
{
    if(m_list.empty())
        return;
    emit dataChanged(index(0, 0), index(m_list.size()-1, 0));
}

int SceneObjectList::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_list.size();
}

QVariant SceneObjectList::data(const QModelIndex &index, int role) const
{
    Q_ASSERT(index.isValid());
    const auto& object = at(index.row());
    switch (role) {
    case PostitionRole:
        return toPoint(object->position());
    case RectRole:
        return fromEngine(object->rect());
    }
    return {};
}

QHash<int, QByteArray> SceneObjectList::roleNames() const
{
    const static QHash<int, QByteArray> names = {
        {PostitionRole, "position"},
        {RectRole, "rect"}
    };
    return names;
}
