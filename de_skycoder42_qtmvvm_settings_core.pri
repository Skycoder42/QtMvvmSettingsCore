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

!isEmpty(QTMVVM_SETTINGS_TS_FILES) {
	settingsts.target = lupdate-settings
	settingsts.commands = $$shell_quote($$shell_path($$PWD/lupdate_xml.py)) $$shell_quote($$[QT_INSTALL_BINS]) $$shell_quote($$_PRO_FILE_PWD_) $$shell_quote($$QTMVVM_SETTINGS_TS_LOCALES) $$QTMVVM_SETTINGS_TS_FILES
	lupdate.depends += lupdate-settings

	QMAKE_EXTRA_TARGETS += lupdate settingsts

	for(locale, QTMVVM_SETTINGS_TS_LOCALES) {
		QPM_TRANSLATIONS += $$_PRO_FILE_PWD_/qtmvvm_settings_xml_$${locale}.ts
	}
}
