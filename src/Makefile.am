bin_PROGRAMS = vilistextum 
vilistextum_SOURCES = debug.h charset.c charset.h \
											html.c html.h latin1.c latin1.h html_tag.c html_tag.h \
											lists.c lists.h microsoft.c microsoft.h \
											text.c text.h fileio.c fileio.h \
											main.c main.h util.c util.h \
											multibyte.h unicode_entities.c unicode_entities.h

# profiling
#AM_CFLAGS = -Wall -pg
AM_CFLAGS = -O2 -Wall #-ansi #-pedantic-error -funsigned-char
AM_LDFLAGS = $(STATIC)

EXTRA_DIST = kilistextum.kaptn

kilistextum: kilistextum.kaptn
	rm -f kilistextum
	if test $(KAPTAIN) != no; then \
		{ touch kilistextum ; \
		echo \#\!$(KAPTAIN) >> kilistextum ; \
		cat kilistextum.kaptn >> kilistextum ; \
		chmod 744 kilistextum ; } ; \
	fi

