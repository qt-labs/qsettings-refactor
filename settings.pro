TEMPLATE = app
TARGET = settings
INCLUDEPATH += . include
QT = core

# Input
HEADERS += include/settings.h
SOURCES += main.cpp

include(backend/backend.pri)
