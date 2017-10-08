HEADERS += \
	$$PWD/settingscontrol.h \
	$$PWD/settingssetuploader.h \
	$$PWD/xmlsettingssetuploader.h

SOURCES += \
	$$PWD/settingscontrol.cpp \
	$$PWD/xmlsettingssetuploader.cpp

DISTFILES += \
	$$PWD/lupdate_xml.py

TRANSLATIONS += $$PWD/qtmvvm_settings_core_de.ts \
	$$PWD/qtmvvm_settings_core_template.ts

INCLUDEPATH += $$PWD

!qpmx_static: include($$PWD/de_skycoder42_qtmvvm_settings_core.prc)
