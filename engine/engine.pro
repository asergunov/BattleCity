TEMPLATE = lib
TARGET = engine

CONFIG += c++11 create_prl
CONFIG -= qt

include(../math/link_math.pri)

SOURCES += \
    Cell.cpp \
    Enemy.cpp \
    Bullet.cpp \
    Tank.cpp \
    Actor.cpp \
    Field.cpp \
    Scene.cpp \
    Player.cpp \
    Bonus.cpp \
    SceneObject.cpp \
    FieldObject.cpp \
    Eagle.cpp


HEADERS += \
    Field.hpp \
    Cell.hpp \
    Enemy.hpp \
    Bullet.hpp \
    Tank.hpp \
    Actor.hpp \
    Scene.hpp \
    types.hpp \
    Player.hpp \
    Bonus.hpp \
    SceneObject.hpp \
    FieldObject.hpp \
    Eagle.hpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    link_engine.pri

