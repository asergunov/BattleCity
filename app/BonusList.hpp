#pragma once

#include "SceneObjectList.hpp"

namespace battlecity {
namespace engine {
class Bonus;
}}

class BonusList : public SceneObjectList
{
    Q_OBJECT
public:
    enum Roles {
        TypeRole = SceneObjectList::UserRole,
        UserRole
    };

    enum Type {
        Helmet,
        Timer,
        Shovel,
        Star,
        Granade,
        Tank,
    };

    Q_ENUM(Type)
public:
    explicit BonusList(QObject *parent = nullptr);

    void add(const battlecity::engine::Bonus* bonus);
    void remove(const battlecity::engine::Bonus* bonus);
    const battlecity::engine::Bonus* at(int row)  const;

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;


signals:

public slots:
};
