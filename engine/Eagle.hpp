#pragma once

#include "SceneObject.hpp"

namespace battlecity {
namespace engine {

class Eagle : public SceneObject
{
public:
    Eagle(Scene *scene);

public:
    coordinate_rect_type rect() const override;
    const field_rect_type& fieldRect() const { return _field_rect; }
    bool killed() const { return _killed; }

    void respawn(const field_rect_type& fieldRect);
    void kill();

private:
    field_rect_type _field_rect;
    bool _killed = false;
};

}}
