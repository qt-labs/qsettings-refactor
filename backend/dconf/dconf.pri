INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

CONFIG += no_keywords link_pkgconfig
PKGCONFIG += dconf

SOURCES += \
    $$PWD/dconfsettings.cpp

HEADERS  += \
    $$PWD/dconfsettings.h
