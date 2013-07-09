INCLUDEPATH += $$PWD/dconf
DEPENDPATH += $$PWD/dconf

CONFIG += no_keywords link_pkgconfig
PKGCONFIG += dconf

SOURCES += \
    $$PWD/dconf/dconfsettings.cpp

HEADERS  += \
    $$PWD/dconf/dconfsettings.h
