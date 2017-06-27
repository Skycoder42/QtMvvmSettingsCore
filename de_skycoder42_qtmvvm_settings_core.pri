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

!isEmpty(QTMVVM_SETTINGS_FILE) {
	settingsts.target = lupdate-settings
	settingsts.commands = $$shell_quote($$shell_path($$PWD/lupdate_xml.py)) $$QTMVVM_SETTINGS_TS $$QTMVVM_SETTINGS_LOCALES
	lupdate.depends += lupdate-settings

	QMAKE_EXTRA_TARGETS += lupdate settingsts
}
