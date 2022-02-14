TEMPLATE = app
TARGET = engine_test

include(../engine/link_engine.pri)

QT += testlib
QT += gui

CONFIG += testcase link_prl

SOURCES += \
    tst_engine.cpp \
    DummyTank.cpp \
    DummyBullet.cpp

HEADERS += \
    DummyTank.hpp \
    DummyBullet.hpp

