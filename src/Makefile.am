bin_PROGRAMS = vilistextum 
vilistextum_SOURCES = debug.h charset.c charset.h \
											html.c html.h latin1.c latin1.h html_tag.c html_tag.h \
											lists.c lists.h microsoft.c microsoft.h \
											text.c text.h fileio.c fileio.h \
											main.c main.h util.c util.h \
											multibyte.h

# profiling
# CFLAGS = @CFLAGS@ -pg

#CFLAGS = @CFLAGS@ -Wall 
AM_CFLAGS = -Wall

EXTRA_DIST = kilistextum.kaptn

kilistextum:
	rm -f kilistextum
	if test $(KAPTAIN) != no; then \
		{ touch kilistextum ; \
		echo \#\!$(KAPTAIN) >> kilistextum ; \
		cat kilistextum.kaptn >> kilistextum ; \
		chmod 744 kilistextum ; } ; \
	fi

