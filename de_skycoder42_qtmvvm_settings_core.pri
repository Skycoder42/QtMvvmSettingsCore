HEADERS += \
	$$PWD/settingscontrol.h \
	$$PWD/settingssetuploader.h \
	$$PWD/xmlsettingssetuploader.h

SOURCES += \
	$$PWD/settingscontrol.cpp \
	$$PWD/xmlsettingssetuploader.cpp

DISTFILES += \
    $$PWD/lupdate_xml.py

QPM_TRANSLATIONS += $$PWD/qtmvvm_settings_core_de.ts \
	$$PWD/qtmvvm_settings_core_template.ts

INCLUDEPATH += $$PWD
