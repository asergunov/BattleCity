#pragma once

#include "SceneObject.hpp"

namespace battlecity {
namespace engine {

class Scene;
class Bullet;

class Actor : public SceneObject
{
public:

public:
    explicit Actor(Scene* scene);
    virtual ~Actor() {}

    virtual void process(time_type dt) = 0;
    virtual direction_type direction() const = 0;
};

}}
