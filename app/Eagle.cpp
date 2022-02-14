#include "Eagle.hpp"

#include <engine/Eagle.hpp>

using namespace battlecity;

Eagle::Eagle(const engine::Eagle *eagle, QObject *parent)
    : SceneObject(eagle, parent)
{

}

const engine::Eagle *Eagle::engineObject() const
{
    return static_cast<const engine::Eagle*>(SceneObject::engineObject());
}

void Eagle::update()
{
    SceneObject::update();

    {
        const bool killed = engineObject()->killed();
        if(killed != m_killed) {
            m_killed = killed;
            emit killedChanged(killed);
        }
    }

}
