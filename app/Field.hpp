#pragma once

#include <QAbstractListModel>

#include <QSize>

namespace battlecity {
namespace engine {
class Field;
class Cell;
}}

class Field : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
public:
    enum Roles {
        CellRole = Qt::UserRole,    ///< QSize - cell coordinate
        TypeRole,                   ///< Type  - cell type
        DestructionRole,                ///< Destroy - cell destroy
        UserRole
    };

    enum Type {
        Stone   = 0x01,
        Water   = 0x02,
        Bush    = 0x03,
        Ice     = 0x04,
        Brick   = 0x05
    };
    Q_ENUM(Type)

    enum Destroy {
        Up       = 0x10,
        Down     = 0x20,
        Left     = 0x40,
        Right    = 0x80,
    };
    Q_ENUM(Destroy)


public:
    explicit Field(const battlecity::engine::Field& field, QObject *parent = nullptr);
    ~Field();
    QHash<int, QByteArray> roleNames() const override;
    const QSize& size() const { return m_size; }
signals:

    void sizeChanged(QSize size);

public slots:
    void reset();
    void reduce();

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

private:
    struct CellInfo;
    std::vector<CellInfo> m_non_empty_cells;
    const battlecity::engine::Field& m_field;
    QSize m_size;
};
