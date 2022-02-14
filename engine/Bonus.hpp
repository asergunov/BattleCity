#pragma once

#include "FieldObject.hpp"

#include <vector>

namespace battlecity {
namespace engine {

class Bonus : public FieldObject
{
public:
    enum class Type {
        Invalid,
        Granade,
        Helmet,
        Star,
        Tank,
        Timer,
        Shovel,
    };

public:
    Bonus(Scene* scene);
    Bonus(Scene* scene, Type type, const field_vector_type &cell);
    static Bonus createRandom(Scene *scene, const std::vector<field_vector_type>& cells);

    // Actor interface
public:
    coordinate_rect_type rect() const override;
    const Type& type() const { return _type; }
    field_rect_type fieldRect() const override;
    bool isValid() const { return _type != Type::Invalid; }
    score_type scoreToTake() const;

private:
    field_vector_type _cell;
    Type _type = Type::Invalid;
};

}}
