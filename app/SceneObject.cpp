#include "SceneObject.hpp"

#include "typeConvertion_p.hpp"

#include <engine/SceneObject.hpp>

SceneObject::SceneObject(const battlecity::engine::SceneObject *object, QObject *parent)
    : QObject(parent)
    , m_engineObject(object)
    , m_position( toPoint(object->position()) )
{

}

void SceneObject::update()
{
    const auto& position = toPoint(m_engineObject->position());
    if(position != m_position)
    {
        m_position = position;
        emit positionChanged(position);
    }
}
