#include "Field.hpp"

#include <engine/types.hpp>
#include <engine/Cell.hpp>
#include <engine/Field.hpp>

#include <QPoint>
#include <QSize>

struct Field::CellInfo {
    battlecity::engine::field_vector_type pos;
    battlecity::engine::Cell cell;
};

Field::Field(const battlecity::engine::Field &field, QObject *parent)
    : QAbstractListModel(parent)
    , m_field(field)
    , m_size(m_field.width(), m_field.height())
{

}

Field::~Field()
{

}

void Field::reset()
{
    {
        const QSize size(m_field.width(), m_field.height());
        if(size != m_size) {
            m_size = size;
            emit sizeChanged(size);
        }
    }

    beginResetModel();
    m_non_empty_cells.clear();
    const auto end = m_field.end();
    for(auto i = m_field.begin(); i != end; ++i) {
        m_non_empty_cells.push_back({i.cell(), *i});
    }
    endResetModel();
}

void Field::reduce()
{
    using namespace battlecity::engine;
    const auto& begin = m_non_empty_cells.begin();
    for(auto i = begin; i != m_non_empty_cells.end(); ) {
        const int row = static_cast<int>( std::distance(begin, i) );
        const auto& cell = m_field.at(i->pos);
        if(cell.type() == Cell::Type::Empty) {
            beginRemoveRows({}, row, row);
            i = m_non_empty_cells.erase(i);
            endRemoveRows();
        } else {
            if(i->cell != cell) {
                i->cell = cell;
                const auto index = this->index(row, 0);
                emit dataChanged(index, index);
            }
            ++i;
        }
    }
}

int Field::rowCount(const QModelIndex &parent) const
{
    return parent.isValid()
            ? 0
            : m_non_empty_cells.size();
}

QVariant Field::data(const QModelIndex &index, int role) const
{
    using namespace battlecity::engine;

    if(!index.isValid())
        return {};

    const auto& cell = m_non_empty_cells[index.row()];
    switch (role) {
    case CellRole:
        return QPoint(cell.pos.x, cell.pos.y);
    case TypeRole:
        return static_cast<int>( cell.cell.type() );
    case DestructionRole:
        return static_cast<int>( cell.cell.hit() );
    }
    return {};
}

QHash<int, QByteArray> Field::roleNames() const
{
    static const auto names = [](){
        QHash<int, QByteArray> names;
        names.insert(CellRole, "cell");
        names.insert(TypeRole, "type");
        names.insert(DestructionRole, "destruction");
        return names;
    }();

    return names;
}

