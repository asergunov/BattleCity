#pragma once

#include <QAbstractListModel>

namespace battlecity {
namespace engine {
class SceneObject;
}}

class SceneObjectList : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Roles {
        PostitionRole = Qt::UserRole,
        RectRole,
        UserRole
    };
public:
    explicit SceneObjectList(QObject *parent = nullptr);

    void add(const battlecity::engine::SceneObject*);
    void remove(const battlecity::engine::SceneObject*);
    const battlecity::engine::SceneObject* at(int row)  const;

    void clear();
    void updated();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:

public slots:

private:
    QList<const battlecity::engine::SceneObject*> m_list;

};
