AUTOMAKE_OPTIONS = foreign

SUBDIRS = src doc tests
EXTRA_DIST = CHANGES COPYING INSTALL README

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
	cp $(distdir).tar.bz2 old

documentation:
	rm -f CHANGES README
	$(PACKAGE) -l doc/README.xhtml README
	echo >> README " "
	echo >> README "PS: This document has been automatically created with vilistextum"
	echo >> README "    from README.html."
	$(PACKAGE) -l doc/htmlmail.xhtml doc/htmlmail
	$(PACKAGE) doc/changes.xhtml - | sed -e "1,2D" -e "s/^  \*/  -/" >CHANGES

test: check

