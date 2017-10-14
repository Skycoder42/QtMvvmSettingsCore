#include "xmlsettingssetuploader.h"

#include <QDebug>

#define hasValue(key) attributes().hasAttribute(QStringLiteral(key))
#define stringValue(key) attributes().value(QStringLiteral(key)).toString()
#define boolValue(key) attributes().value(QStringLiteral(key)) == QStringLiteral("true")

QUrl XmlSettingsSetupLoader::defaultIcon(QStringLiteral("qrc:/qtmvvm/icons/settings.svg"));

SettingsSetup XmlSettingsSetupLoader::loadSetup(const QByteArray &platform, QIODevice *device, QIODevice *extraPropertyDevice)
{
	QXmlStreamReader reader(device);

	QVariantHash extraProperties;
	if(extraPropertyDevice)
		extraProperties = readExtraProperties(extraPropertyDevice);

	SettingsSetup setup;
	if(!reader.readNextStartElement() || reader.name() != QStringLiteral("SettingsConfig"))
		throwError(reader);
	setup.allowSearch = reader.boolValue("allowSearch");
	setup.allowRestore = reader.boolValue("allowRestore");

	if(reader.readNextStartElement()) {
		if(reader.name() == QStringLiteral("Category")) {
			do
				setup.categories.append(readCategory(reader, platform, extraProperties));
			while(reader.readNextStartElement());
		} else
			setup.categories.append(readDefaultCategory(reader, platform, extraProperties));
	}

	return setup;
}

void XmlSettingsSetupLoader::overwriteDefaultIcon(const QUrl &defaultIcon)
{
	XmlSettingsSetupLoader::defaultIcon = defaultIcon;
}

void XmlSettingsSetupLoader::throwError(QXmlStreamReader &reader, const QByteArray &customError)
{
	throw QStringLiteral("Failed to read settings with error %1 at %2:%3")
			.arg(customError.isNull() ? reader.errorString() : QString::fromUtf8(customError))
			.arg(reader.lineNumber())
			.arg(reader.columnNumber());
}

SettingsCategory XmlSettingsSetupLoader::readCategory(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties)
{
	if(reader.name() != QStringLiteral("Category"))
		throwError(reader, "Child Elements must be of the same type (Here: Category)");

	SettingsCategory category;
	if(reader.boolValue("default"))
		category = createDefaultCategory();

	if(reader.hasValue("title"))
		category.title = reader.stringValue("title");
	if(reader.hasValue("icon"))
		category.icon = reader.stringValue("icon");
	if(reader.hasValue("tooltip"))
		category.tooltip = reader.stringValue("tooltip");

	if(reader.readNextStartElement()) {
		if(reader.name() == QStringLiteral("Section")) {
			do
				category.sections.append(readSection(reader, platform, extraProperties));
			while(reader.readNextStartElement());
		} else
			category.sections.append(readDefaultSection(reader, platform, extraProperties));
	}

	return category;
}

SettingsCategory XmlSettingsSetupLoader::readDefaultCategory(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties)
{
	SettingsCategory category = createDefaultCategory();

	if(reader.name() == QStringLiteral("Section")) {
		do
			category.sections.append(readSection(reader, platform, extraProperties));
		while(reader.readNextStartElement());
	} else
		category.sections.append(readDefaultSection(reader, platform, extraProperties));

	return category;
}

SettingsSection XmlSettingsSetupLoader::readSection(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties)
{
	if(reader.name() != QStringLiteral("Section"))
		throwError(reader, "Child Elements must be of the same type (Here: Section)");

	SettingsSection section;
	if(reader.boolValue("default"))
		section = createDefaultSection();

	if(reader.hasValue("title"))
		section.title = reader.stringValue("title");
	if(reader.hasValue("icon"))
		section.icon = reader.stringValue("icon");
	if(reader.hasValue("tooltip"))
		section.tooltip = reader.stringValue("tooltip");

	if(reader.readNextStartElement()) {
		if(reader.name() == QStringLiteral("Group")) {
			do
				section.groups.append(readGroup(reader, platform, extraProperties));
			while(reader.readNextStartElement());
		} else
			section.groups.append(readDefaultGroup(reader, platform, extraProperties));
	}

	return section;
}

SettingsSection XmlSettingsSetupLoader::readDefaultSection(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties)
{
	SettingsSection section = createDefaultSection();

	if(reader.name() == QStringLiteral("Group")) {
		do
			section.groups.append(readGroup(reader, platform, extraProperties));
		while(reader.readNextStartElement());
	} else
		section.groups.append(readDefaultGroup(reader, platform, extraProperties));

	return section;
}

SettingsGroup XmlSettingsSetupLoader::readGroup(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties)
{
	if(reader.name() != QStringLiteral("Group"))
		throwError(reader, "Child Elements must be of the same type (Here: Group)");

	SettingsGroup group;
	if(reader.hasValue("title"))
		group.title = reader.stringValue("title");

	if(reader.readNextStartElement()) {
		if(reader.name() == QStringLiteral("Entry")) {
			do {
				auto skip = false;
				auto entry = readEntry(reader, platform, skip, extraProperties);
				if(!skip)
				   group.entries.append(entry);
			} while(reader.readNextStartElement());
		} else
			throwError(reader, "Expected Element of type Entry");
	}

	return group;
}

SettingsGroup XmlSettingsSetupLoader::readDefaultGroup(QXmlStreamReader &reader, const QByteArray &platform, const QVariantHash &extraProperties)
{
	SettingsGroup group;

	if(reader.name() == QStringLiteral("Entry")) {
	   do {
		   auto skip = false;
		   auto entry = readEntry(reader, platform, skip, extraProperties);
		   if(!skip)
			  group.entries.append(entry);
	   } while(reader.readNextStartElement());
	} else
		throwError(reader, "Expected Element of type Entry");

	return group;
}

SettingsEntry XmlSettingsSetupLoader::readEntry(QXmlStreamReader &reader, const QByteArray &platform, bool &skip, const QVariantHash &extraProperties)
{
	SettingsEntry entry;

	auto plt = reader.stringValue("platform");
	skip = !plt.isEmpty() && !plt.contains(QString::fromUtf8(platform));

	if(!reader.hasValue("key"))
		throwError(reader, "key attribute is required!");
	entry.key = reader.stringValue("key");
	if(!reader.hasValue("type"))
		throwError(reader, "type attribute is required!");
	entry.type = reader.stringValue("type").toLatin1();
	if(!reader.hasValue("title"))
		throwError(reader, "title attribute is required!");
	entry.title = reader.stringValue("title");
	entry.tooltip = reader.stringValue("tooltip");
	entry.defaultValue = reader.stringValue("default");

	while(reader.readNextStartElement()) {
		if(reader.name() == QStringLiteral("SearchKey"))
			entry.searchKeys.append(reader.readElementText());
		else if(reader.name() == QStringLiteral("Property")) {
			auto prop = readProperty(reader);
			entry.properties.insert(prop.first, prop.second);
		} else
			throwError(reader, "Unexpected Element type");
	}

	auto extras = extraProperties.value(entry.key).toMap();
	for(auto it = extras.constBegin(); it != extras.constEnd(); it++)
		entry.properties.insert(it.key(), it.value());

	return entry;
}

QVariantHash XmlSettingsSetupLoader::readExtraProperties(QIODevice *device)
{
	QXmlStreamReader reader(device);
	QVariantHash extraProperties;

	if(!reader.readNextStartElement() || reader.name() != QStringLiteral("SettingsProperties"))
		throwError(reader);

	while(reader.readNextStartElement()) {
		if(reader.name() != QStringLiteral("Entry"))
			throwError(reader, "Expected Element of type Entry");

		auto property = readProperty(reader, QStringLiteral("object"));
		extraProperties.insert(property.first, property.second);
	}

	return extraProperties;
}

QPair<QString, QVariant> XmlSettingsSetupLoader::readProperty(QXmlStreamReader &reader, const QString &overwriteType)
{
	QPair<QString, QVariant> property;
	if(!reader.hasValue("key"))
		throwError(reader, "key attribute is required!");
	property.first = reader.stringValue("key");
	property.second = readElement(reader, overwriteType);
	return property;
}

QVariant XmlSettingsSetupLoader::readElement(QXmlStreamReader &reader, const QString &overwriteType)
{
	QString type;
	if(overwriteType.isNull())
		type = reader.stringValue("type");
	else
		type = overwriteType;

	if(type == QStringLiteral("string") || type.isEmpty())
		return reader.readElementText();
	else if(type == QStringLiteral("number"))
		return reader.readElementText().toDouble();
	else if(type == QStringLiteral("bool"))
		return (reader.readElementText() == QStringLiteral("true"));
	else if(type == QStringLiteral("list")) {
		QVariantList list;
		while (reader.readNextStartElement()) {
			if(reader.name() != QStringLiteral("Element"))
				throwError(reader, "Expected Element of type Element");
			list.append(readElement(reader));
		}
		return list;
	} else if(type == QStringLiteral("object")) {
		QVariantMap map;
		while (reader.readNextStartElement()) {
			if(reader.name() != QStringLiteral("Property"))
				throwError(reader, "Expected Element of type Property");
			auto property = readProperty(reader);
			map.insert(property.first, property.second);
		}
		return map;
	} else
		throwError(reader, "Unable to handle unknown type");

	return {};
}

SettingsCategory XmlSettingsSetupLoader::createDefaultCategory()
{
	SettingsCategory category;
	category.title = tr("General Settings");
	category.icon = defaultIcon;
	return category;
}

SettingsSection XmlSettingsSetupLoader::createDefaultSection()
{
	SettingsSection section;
	section.title = tr("General");
	return section;
}
