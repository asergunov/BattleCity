#pragma once

#include "types.hpp"

namespace battlecity {
namespace engine {

class Scene;

class SceneObject
{
public:
    SceneObject(Scene* scene);
protected:
    ~SceneObject();

public:
    Scene* scene() const { return _scene; }
    void setScene(Scene* scene) { _scene = scene; }

    coordinate_vector_type position() const { return rect().topLeft(); }
    virtual coordinate_rect_type rect() const = 0;
private:
    Scene* _scene = nullptr;
};

}}
