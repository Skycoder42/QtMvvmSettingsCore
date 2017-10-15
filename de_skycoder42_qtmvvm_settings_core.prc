DISTFILES += \
	$$PWD/lupdate_xml.py

!isEmpty(QTMVVM_SETTINGS_FILES): {	
	QTMVV_SETTINGS_TS_COMMANDS = $$shell_quote($$[QT_INSTALL_BINS]) $$shell_quote($$_PRO_FILE_PWD_) $$QTMVVM_SETTINGS_FILES
	linux: QTMVV_SETTINGS_TS_COMMANDS = $$shell_quote($$shell_path($$PWD/lupdate_xml.py)) $$QTMVV_SETTINGS_TS_COMMANDS
	else:win32: QTMVV_SETTINGS_TS_COMMANDS = python $$shell_quote($$shell_path($$PWD/lupdate_xml.py)) $$QTMVV_SETTINGS_TS_COMMANDS
	else:mac: QTMVV_SETTINGS_TS_COMMANDS = /usr/local/bin/python3 $$shell_quote($$shell_path($$PWD/lupdate_xml.py)) $$QTMVV_SETTINGS_TS_COMMANDS
	
	!system($$QTMVV_SETTINGS_TS_COMMANDS):warning(Failed to generate dummy cpp file for settings translations)

	!no_settings_ts_warn {
		message(Please add \"never_true_lupdate_only: SOURCES += .qtmvvm_settings_xml_ts.cppdummy\" to your pro file)
		message(You can add \"CONFIG += no_settings_ts_warn\" to remove this notification)
	}
	
	DISTFILES += $$QTMVVM_SETTINGS_FILES
}
