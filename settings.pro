TEMPLATE = app
TARGET = settings
INCLUDEPATH += . include
QT = core

HEADERS += \
    include/settings.h

SOURCES += \
    main.cpp \
    src/settings.cpp

CONFIG(debug, debug|release) {
    OBJECTS_DIR = .debug
    MOC_DIR = .debug
} else {
    OBJECTS_DIR = .release
    MOC_DIR = .release
}

include(backend/backend.pri)
