win32:include(win32/win32.pri)
else:mac:include(cocoa/cocoa.pri)
else:unix:include(dconf/dconf.pri)
else:error(Not implemented for $$first(QMAKE_PLATFORM))
