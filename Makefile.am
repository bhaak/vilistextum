AUTOMAKE_OPTIONS = foreign

SUBDIRS = src doc
EXTRA_DIST = CHANGES COPYING INSTALL

mydist: vilistextum documentation dist bzip2dist

vilistextum:
	cd src && $(MAKE)

static:
	rm -f src/vilistextum
	cd src && env STATIC=-static $(MAKE)

bzip2dist:
	gunzip $(distdir).tar.gz
	bzip2 -f -k $(distdir).tar
	gzip -f $(distdir).tar
	cp $(distdir).tar.gz old

documentation:
	$(PACKAGE) -l doc/README.xhtml doc/README
	echo >>doc/README " "
	echo >>doc/README "PS: This document has been automatically created with vilistextum"
	echo >>doc/README "    from README.xhtml."
	$(PACKAGE) -l doc/htmlmail.xhtml doc/htmlmail
	rm -f CHANGES
	$(PACKAGE) doc/changes.xhtml - | sed -e "1,2D" -e "s/^  \*/  -/" >CHANGES


