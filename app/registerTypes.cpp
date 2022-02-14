#include "registerTypes.hpp"

#include "Enemy.hpp"
#include "Tank.hpp"
#include "Actor.hpp"
#include "PlayerController.hpp"
#include "LevelScene.hpp"
#include "Player.hpp"
#include "Field.hpp"
#include "Bonus.hpp"
#include "Eagle.hpp"

#include <QQmlEngine>

void registerTypes()
{
    const char* uri = "battlecity.engine";
    const int versionMajor = 1;
    const int versionMinor = 0;
    const QString message = "Can't create engine object: %1";

    qmlRegisterUncreatableType<SceneObject>(uri, versionMajor, versionMinor, "SceneObject", message.arg("SceneObject"));
    qmlRegisterUncreatableType<ActorList>(uri, versionMajor, versionMinor, "ActorList", message.arg("ActorList"));
    qmlRegisterUncreatableType<TankList>(uri, versionMajor, versionMinor, "TankList", message.arg("TankList"));
    qmlRegisterUncreatableType<EnemyList>(uri, versionMajor, versionMinor, "EnemyList", message.arg("EnemyList"));
    qmlRegisterUncreatableType<PlayerList>(uri, versionMajor, versionMinor, "PlayerList", message.arg("PlayerList"));
    qmlRegisterUncreatableType<PlayerController>(uri, versionMajor, versionMinor, "PlayerController", message.arg("PlayerController"));
    qmlRegisterUncreatableType<Field>(uri, versionMajor, versionMinor, "Field", message.arg("Field"));
    qmlRegisterUncreatableType<BonusList>(uri, versionMajor, versionMinor, "BonusList", message.arg("BonusList"));
    qmlRegisterUncreatableType<Eagle>(uri, versionMajor, versionMinor, "Eagle", message.arg("Eagle"));

    qmlRegisterType<LevelScene>(uri, versionMajor, versionMinor, "LevelScene");
}
