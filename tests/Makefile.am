TESTS =  check_tags check_entities check_latin2 check_utf8 check_fallback_entities check_utf8_to_latin1 check_html5 check_bufferoverflow

EXTRA_DIST = README \
						testcases/*.html \
						testcases/*.output \
						$(TESTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

DISTCLEANFILES = testcases/*.output.test

.PHONEY: test

test: check

all: check

