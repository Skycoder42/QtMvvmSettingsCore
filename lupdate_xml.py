#!/usr/bin/env python3
# Usage: lupdate_xml.py bindir srcdir locales(space seperated) xml_sources...

import sys
import os
import subprocess
from xml.etree.ElementTree import Element, parse

bindir = sys.argv[1]
srcdir = sys.argv[2]
locales = sys.argv[3].split(" ")
srces = sys.argv[4:]

trstrings = set()

for src in srces:
	tree = parse(os.path.join(srcdir, src))
	root = Element("TS")
	for elem in tree.iter():
		if elem.tag == "SearchKey":
			trstrings.add(elem.text)
		else:
			if "title" in elem.attrib:
				trstrings.add(elem.attrib["title"])
			if "tooltip" in elem.attrib:
				trstrings.add(elem.attrib["tooltip"])

outfile = open(".qtmvvm_settings_xml_ts_dummy.cpp", "w")
outfile.write("#include <QCoreApplication>\n\n")
outfile.write("void dummyfn() {\n")
for str in trstrings:
	outfile.write("\tQCoreApplication::translate(\"qtmvvm_settings_xml\", \"{}\");\n".format(str))
outfile.write("}\n")
outfile.close()

args = [
	os.path.join(bindir, "lupdate"),
	"-locations",
	"relative",
	outfile.name,
	"-ts"
]
for locale in locales:
	args.append(os.path.join(srcdir, "qtmvvm_settings_xml_" + locale + ".ts"))
subprocess.run(args)
