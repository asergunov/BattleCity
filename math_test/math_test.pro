TEMPLATE = app

include(../math/link_math.pri)

QT += testlib
QT += gui

CONFIG += testcase link_prl

SOURCES += \
    tst_math.cpp

