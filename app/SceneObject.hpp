#pragma once

#include <QObject>
#include <QPoint>

namespace battlecity {
namespace engine {
class SceneObject;
}}


class SceneObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
public:
    explicit SceneObject(const battlecity::engine::SceneObject* object, QObject *parent = nullptr);

    const battlecity::engine::SceneObject* engineObject() const {
        return m_engineObject;
    }

    const QPoint& position() const { return m_position; }

    virtual void update();
signals:
    void positionChanged(QPoint position);
public slots:
private:
    const battlecity::engine::SceneObject* m_engineObject;
    QPoint m_position;
};
