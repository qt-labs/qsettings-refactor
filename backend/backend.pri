mac:include(cocoa/cocoa.pri)
else:unix:include(dconf/dconf.pri)
else:error(Not implemented for $$first(QMAKE_PLATFORM))
