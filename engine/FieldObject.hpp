#pragma once

#include "SceneObject.hpp"

namespace battlecity {
namespace engine {

class FieldObject : public SceneObject
{
public:
    FieldObject(Scene* scene);
    virtual field_rect_type fieldRect() const = 0;
};

}}
