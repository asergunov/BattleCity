TEMPLATE = subdirs  # workaround: header only library
TARGET = engine

CONFIG += c++11 create_prl
CONFIG -= qt

SOURCES +=

HEADERS += \
    Direction.hpp \
    Vector.hpp \
    FixedPoint.hpp \
    Rect.hpp \
    MovementProcessor.hpp


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    link_math.pri
