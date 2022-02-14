#include "Bonus.hpp"

#include "Scene.hpp"

#include <cassert>

using namespace battlecity::engine;

Bonus::Bonus(Scene *scene)
    : FieldObject(scene)
    , _type(Type::Invalid)
{

}

Bonus::Bonus(Scene *scene, Type type, const field_vector_type& cell)
    : FieldObject(scene)
    , _cell(cell)
    , _type(type)
{

}

field_rect_type Bonus::fieldRect() const
{
    return field_rect_type(_cell, {2,2});
}

score_type Bonus::scoreToTake() const
{
    return 500;
}

Bonus Bonus::createRandom(Scene* scene, const std::vector<field_vector_type>& cells)
{
    // showel is not implemented
    return {scene, static_cast<Type>(rand()%5+1), cells.at(rand() % cells.size())};
}

coordinate_rect_type Bonus::rect() const
{
    assert(scene());
    const auto& cellSize = scene()->fieldCellSize();
    return {
        coordinate_vector_type(_cell)*cellSize,
        {cellSize, cellSize}
    };
}

