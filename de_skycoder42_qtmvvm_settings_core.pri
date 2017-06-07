HEADERS += \
	$$PWD/settingscontrol.h \
	$$PWD/settingssetuploader.h \
	$$PWD/xmlsettingssetuploader.h

SOURCES += \
	$$PWD/settingscontrol.cpp \
	$$PWD/xmlsettingssetuploader.cpp

INCLUDEPATH += $$PWD

DISTFILES += \
    $$PWD/lupdate_xml.py