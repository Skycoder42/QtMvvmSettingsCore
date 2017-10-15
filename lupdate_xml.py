#!/usr/bin/env python3
# Usage: lupdate_xml.py bindir srcdir locales(space seperated) xml_sources...

import sys
import os
import subprocess
from xml.etree.ElementTree import Element, parse

bindir = sys.argv[1]
srcdir = sys.argv[2]
srces = sys.argv[3:]

os.chdir(srcdir)

tsmap = {}
for src in srces:
	trstrings = set()

	tree = parse(src)
	root = Element("TS")
	for elem in tree.iter():
		if elem.tag == "SearchKey":
			trstrings.add(elem.text)
		else:
			if "title" in elem.attrib:
				trstrings.add(elem.attrib["title"])
			if "tooltip" in elem.attrib:
				trstrings.add(elem.attrib["tooltip"])

	tsmap[os.path.basename(src)] = trstrings

outfile = open(".qtmvvm_settings_xml_ts.cppdummy", "w")
outfile.write("#include <QCoreApplication>\n\n")
outfile.write("void dummyfn() {\n")
for src in tsmap:
	for str in tsmap[src]:
		outfile.write("\tQCoreApplication::translate(\"{}\", \"{}\");\n".format(src, str))
outfile.write("}\n")
outfile.close()
