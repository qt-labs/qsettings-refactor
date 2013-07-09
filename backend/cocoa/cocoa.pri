INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

QT += core-private
LIBS += -framework Cocoa -framework Foundation

OBJECTIVE_SOURCES += \
    $$PWD/cocoasettings.mm

HEADERS  += \
    $$PWD/cocoasettings.h
