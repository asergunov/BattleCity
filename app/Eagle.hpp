#pragma once

#include "SceneObject.hpp"

namespace battlecity {
namespace engine {
class Eagle;
}}

class Eagle : public SceneObject
{
    Q_OBJECT
    Q_PROPERTY(bool killed READ killed NOTIFY killedChanged)


public:
    explicit Eagle(const battlecity::engine::Eagle* object, QObject *parent = nullptr);

    const battlecity::engine::Eagle* engineObject() const;

    bool killed() const { return m_killed; }

    // SceneObject interface
public:
    void update() override;

signals:
    void killedChanged(bool killed);

public slots:

private:
    bool m_killed = false;
};

