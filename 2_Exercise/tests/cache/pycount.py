#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import fileinput

def main(argv=None):
	if argv is None:
		argv = sys.argv

	print("")
	nonEmptyLinesSeen = 0
	sameLinesSeen = 0
	prevLine = None
	for line in fileinput.input():
		line = line.strip()
		if line == prevLine:
			sameLinesSeen += 1
			sys.stdout.write(
				"\r" + str(sameLinesSeen) + " "
				+ str(nonEmptyLinesSeen) + " -> "
				+ "{0:.2f}%".format((sameLinesSeen * 100) / nonEmptyLinesSeen))
			sys.stdout.flush()
		else:
			prevLine = line
			if line:
				nonEmptyLinesSeen += 1
			


if __name__ == "__main__":
	sys.exit(main())
