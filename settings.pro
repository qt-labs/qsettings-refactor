TEMPLATE = app
TARGET = settings
INCLUDEPATH += . include
QT = core

HEADERS += \
    include/settings.h

SOURCES += \
    main.cpp \
    src/settings.cpp

include(backend/backend.pri)
