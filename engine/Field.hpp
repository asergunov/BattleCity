#pragma once

#include "types.hpp"
#include "Cell.hpp"

#include <Vector.hpp>

#include <array>
#include <vector>

namespace battlecity {
namespace engine {

class Field
{
public:
    using size_type = field_coordinate_type;  ///< type for field size. Original size is 26x26 so byte should be enough there
    using coordinate_vector_type = field_vector_type;

    template<typename underlying_iterator>
    class base_iterator : public underlying_iterator
    {
        const Field * _field;
    public:
        coordinate_vector_type cell() const {
            const auto distance = std::distance(_field->begin(), *this);
            return coordinate_vector_type(distance % _field->_width, distance / _field->_width);
        }
    protected:
        base_iterator(const Field* field, underlying_iterator i) : underlying_iterator(i), _field(field) {}
        friend class Field;
    };

    using iterator = base_iterator<std::vector<Cell>::iterator>;
    using const_iterator = base_iterator<std::vector<Cell>::const_iterator>;

public:
    Field(size_type width = 0, size_type height = 0, const Cell& v = Cell());
    Field(std::initializer_list<std::vector<Cell> >);

    bool operator==(const Field& other) const {
        return _width == other._width && _height == other._height && _cells == other._cells;
    }

    const size_type& width() const {
        return _width;
    }

    const size_type& height() const {
        return _height;
    }

    /// @{ \defgroup Iteration
    iterator begin() { return {this, _cells.begin()}; }
    iterator end()   { return {this, _cells.end()}; }

    const_iterator begin() const { return {this, _cells.cbegin()}; }
    const_iterator end()   const { return {this, _cells.cend()}; }

    const_iterator cbegin() const { return {this, _cells.cbegin()}; }
    const_iterator cend()   const { return {this, _cells.cend()}; }
    /// @}

    Cell& at(size_type x, size_type y);
    Cell& at(const coordinate_vector_type& p) { return at(p.x, p.y); }

    const Cell& at(size_type x, size_type y) const;
    const Cell& at(const coordinate_vector_type& p) const { return at(p.x, p.y); }
    static Field fromClassicCoding(const std::array<uint64_t, 13>& data);

    bool outOfRange(const coordinate_vector_type& p) const;

    Field part(int x, int y, size_type w, size_type h, const Cell& v = Cell()) const;
    Field resized(size_type w, size_type h, const Cell& v = Cell()) const;


private:
    size_type _width;
    size_type _height;
    std::vector<Cell> _cells;   ///< the horizontal lines of the field from top-left corner to bottom-right
};

}}
