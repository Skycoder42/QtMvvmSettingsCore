#!/usr/bin/env python3
# Usage: lupdate_xml.py xml_source ts_file [locale]
# Example: lupdate_xml.py settings.xml settings_de.ts de_DE

import sys
from xml.etree.ElementTree import ElementTree, Element, SubElement, parse, tostring
from xml.dom import minidom

src = sys.argv[1]
dest = sys.argv[2]
locale = ""
if len(sys.argv) > 3:
	locale = sys.argv[3]

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
root.attrib["language"] = locale

context = SubElement(root, "context")
name = SubElement(context, "name")
name.text = "qtmvvm_settings_xml"

for str in trstrings:
	message = SubElement(context, "message")
	source = SubElement(message, "source")
	source.text = str
	translation = SubElement(message, "translation")
	translation.attrib["type"] = "unfinished"

outFile = open(dest, "w")
tree = ElementTree(root)
tree.write(outFile, encoding="unicode", xml_declaration=True, short_empty_elements=False)
outFile.close()
