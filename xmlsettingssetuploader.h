#ifndef XMLSETTINGSSETUPLOADER_H
#define XMLSETTINGSSETUPLOADER_H

#include <QXmlStreamReader>
#include <QCoreApplication>
#include "settingssetuploader.h"

class XmlSettingsSetupLoader : public SettingsSetupLoader
{
	Q_DECLARE_TR_FUNCTIONS(XmlSettingsSetupLoader)

public:
	SettingsSetup loadSetup(const QByteArray &platform, QIODevice *device, QIODevice *extraPropertyDevice) override;

	static void overwriteDefaultIcon(const QUrl &defaultIcon);

private:
	static QUrl defaultIcon;

	void throwError(QXmlStreamReader &reader, const QByteArray &customError = {});

	SettingsCategory readCategory(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsCategory readDefaultCategory(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsSection readSection(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsSection readDefaultSection(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsGroup readGroup(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsGroup readDefaultGroup(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties);
	SettingsEntry readEntry(QXmlStreamReader &reader, const QByteArray &platform, bool &skip, const QVariantHash &extraProperties);

	QVariantHash readExtraProperties(QIODevice *device);

	QPair<QString, QVariant> readProperty(QXmlStreamReader &reader, const QString &overwriteType = {});
	QVariant readElement(QXmlStreamReader &reader, const QString &overwriteType = {});

	SettingsCategory createDefaultCategory();
	SettingsSection createDefaultSection();
};

#endif // XMLSETTINGSSETUPLOADER_H
