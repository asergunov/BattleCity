#include "Eagle.hpp"

#include "Scene.hpp"

using namespace battlecity::engine;

Eagle::Eagle(Scene* scene)
    :SceneObject(scene)
{

}

coordinate_rect_type Eagle::rect() const
{
    scene()->mapFromField(_field_rect);
}

void Eagle::respawn(const field_rect_type &fieldRect)
{
    _field_rect = fieldRect;
    _killed = false;
}

void Eagle::kill()
{
    _killed = true;
}
