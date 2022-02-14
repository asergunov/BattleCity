#include "Field.hpp"

#include <algorithm>
#include <cassert>

using namespace battlecity::engine;

Field::Field(Field::size_type width, Field::size_type height, const Cell& v)
    : _width(width), _height(height)
    , _cells(height*width, v)
{

}

Field::Field(std::initializer_list<std::vector<Cell> > cells)
    : _width(0), _height(cells.size())
{
    if(_height > 0) {
        auto i_max= std::max_element(cells.begin(), cells.end(), [](const std::vector<Cell>& line1, const std::vector<Cell>& line2){
            return line1.size() < line2.size();
        });
        _width = i_max->size();
        _cells.resize(_height*_width);

        size_type y = 0;
        for(const auto& line : cells) {
            size_type x = 0;
            for(const auto& cell : line) {
                at(x, y) = cell;
                ++x;
            }
            ++y;
        }
    }
}

Cell &Field::at(Field::size_type x, Field::size_type y) {
    assert(!outOfRange({x,y}));
    return _cells.at(x+y*_width);
}

const Cell &Field::at(Field::size_type x, Field::size_type y) const
{
    if(outOfRange({x,y})) {
        static const Cell outOfField(Cell::Type::undestroyableWall);
        return outOfField;
    }
    return _cells.at(x+y*_width);
}


/**
 * @brief make a field by classic data
 * @param data is the array of 91 bytes
 * @return created field
 *
 *  The data is the of the lines of the level. Each letter in hex notation represens the block 2x2.
 *
 * Codes are
 * Bricks:
 * 0 for .B
 *       .B
 *
 * 1 for ..
 *       BB
 *
 * 2 for B.
 *       B.
 *
 * 3 for BB
 *       ..
 *
 * 4 for BB
 *       BB
 *
 * Stone:
 * 5 for .S
 *       .S
 *
 * 6 for ..
 *       SS
 *
 * 7 for S.
 *       S.
 *
 * 8 for SS
 *       ..
 *
 * 9 for SS
 *       SS
 *
 * Water:
 * A for ~~
 *       ~~
 *
 * Brush
 * B for ##
 *       ##
 *
 * Ice
 * C for II Ice
 *       II
 *
 * D, E, F for ..
 *             ..
 *
 * Usage fromClassicCoding( { 0xFFFFFFFFFFFFF,
 *                            0xF4F4F4F4F4F4F,
 *                            0xF4F4F4F4F4F4F,
 *                            0xF4F4F494F4F4F,
 *                            0xF4F4F3F3F4F4F,
 *                            0xF3F3F1F1F3F3F,
 *                            0x1F11F3F3F11F1,
 *                            0x8F33F1F1F33F8,
 *                            0xF1F1F444F1F1F,
 *                            0xF4F4F4F4F4F4F,
 *                            0xF4F4F3F3F4F4F,
 *                            0xF4F4FFFFF4F4F,
 *                            0xFFFFFFFFFFFFF, } ) to load level1
 *
 * Bricks added around the base
 *
 */
Field Field::fromClassicCoding(const std::array<uint64_t, 13> &data)
{
    Field result(13*2, data.size()*2);
    size_type y = 0;

    // define cells
    const static std::vector<Field> blocks = {
        // 0
        {{Cell::Type::Empty, Cell::Type::Brick},
         {Cell::Type::Empty, Cell::Type::Brick}},
        // 1
        {{Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Brick, Cell::Type::Brick}},
        // 2
        {{Cell::Type::Brick, Cell::Type::Empty},
         {Cell::Type::Brick, Cell::Type::Empty}},
        // 3
        {{Cell::Type::Brick, Cell::Type::Brick},
         {Cell::Type::Empty, Cell::Type::Empty}},
        // 4
        {{Cell::Type::Brick, Cell::Type::Brick},
         {Cell::Type::Brick, Cell::Type::Brick}},
        // 5
        {{Cell::Type::Empty, Cell::Type::Stone},
         {Cell::Type::Empty, Cell::Type::Stone}},
        // 6
        {{Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Stone, Cell::Type::Stone}},
        // 7
        {{Cell::Type::Stone, Cell::Type::Empty},
         {Cell::Type::Stone, Cell::Type::Empty}},
        // 8
        {{Cell::Type::Stone, Cell::Type::Stone},
         {Cell::Type::Empty, Cell::Type::Empty}},
        // 9
        {{Cell::Type::Stone, Cell::Type::Stone},
         {Cell::Type::Stone, Cell::Type::Stone}},
        // A
        {{Cell::Type::Water, Cell::Type::Water},
         {Cell::Type::Water, Cell::Type::Water}},
        // B
        {{Cell::Type::Bush, Cell::Type::Bush},
         {Cell::Type::Bush, Cell::Type::Bush}},
        // C
        {{Cell::Type::Ice, Cell::Type::Ice},
         {Cell::Type::Ice, Cell::Type::Ice}},
        // D
        {{Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Empty, Cell::Type::Empty}},
        // E
        {{Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Empty, Cell::Type::Empty}},
        // F
        {{Cell::Type::Empty, Cell::Type::Empty},
         {Cell::Type::Empty, Cell::Type::Empty}}
    };

    // load blocks
    for(auto line : data) {
        for(int x = result.width()/2-1; x>=0; --x) {
            assert((line & 0xF) < blocks.size());
            const auto& block = blocks[line & 0xF];

            for(int i=0; i<2; ++i) for(int j=0; j<2; ++j)
                result.at(x*2+i, y*2+j) = block.at(i,j);

            line >>= 4;
        }
        ++y;
    }

    // build a wall around the base
    result.at(11, result.height()-2) = result.at(11, result.height()-1) = result.at(11, result.height()-3)
            = result.at(12, result.height()-3) = result.at(13, result.height()-3)
            = result.at(14, result.height()-1) = result.at(14, result.height()-2) = result.at(14, result.height()-3) = Cell::Type::Brick;

    // free the base cells
    result.at(12,result.height()-2) = result.at(12,result.height()-1) = result.at(13,result.height()-2) = result.at(13,result.height()-1) = Cell::Type::Empty;

    return result;
}

bool Field::outOfRange(const Field::coordinate_vector_type &p) const
{
    return p.x <0 || p.x >= _width || p.y < 0 || p.y >= _height;
}

Field Field::part(int sx, int sy, Field::size_type w, Field::size_type h, const Cell &v) const
{
    Field result(w, h, v);

    if(sx >= width() || sy >= height())
        return result;

    const auto mw = std::min(w, size_type(width()-sx));
    const auto mh = std::min(h, size_type(height()-sy));

    for(size_type x = 0; x < mw; ++x) for(size_type y = 0; y < mh; ++y) {
        if(x >= -sx && y >= -sy)
            result.at(x,y) = at(sx+x,sy+y);
    }

    return result;
}

Field Field::resized(Field::size_type w, Field::size_type h, const Cell &v) const
{
    return part(0,0, w,h, v);
}

