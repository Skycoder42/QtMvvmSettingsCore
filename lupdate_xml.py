#!/usr/bin/env python3
# Usage: lupdate_xml.py xml_source locales
# Example: lupdate_xml.py settings.xml de template

import sys
from xml.etree.ElementTree import ElementTree, Element, SubElement, parse, tostring
from xml.dom import minidom

src = sys.argv[1]
locales = sys.argv[2:]

trstrings = set()

tree = parse(src)
for elem in tree.iter():
	if elem.tag == "SearchKey":
		trstrings.add(elem.text)
	else:
		if "title" in elem.attrib:
			trstrings.add(elem.attrib["title"])
		if "tooltip" in elem.attrib:
			trstrings.add(elem.attrib["tooltip"])

	root = Element("TS")
	root.attrib["version"] = "2.1"

	context = SubElement(root, "context")
	name = SubElement(context, "name")
	name.text = "qtmvvm_settings_xml"

	for str in trstrings:
		message = SubElement(context, "message")
		source = SubElement(message, "source")
		source.text = str
		translation = SubElement(message, "translation")
		translation.attrib["type"] = "unfinished"

for locale in locales:
	root.attrib["language"] = locale

	outFile = open("qtmvvm_settings_xml" + locale + ".ts", "w")
	tree = ElementTree(root)
	tree.write(outFile, encoding="unicode", xml_declaration=True, short_empty_elements=False)
	outFile.close()
