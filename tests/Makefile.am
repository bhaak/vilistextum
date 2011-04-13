TESTS = check_utf8 \
	check_utf8_to_latin1 \
	check_utf8_to_ascii \
	check_xml_default_encoding \
	check_latin2 \
	check_windows-1252 \
	check_html32 \
	check_tags \
	check_lists \
	check_entities \
	check_fallback_entities \
	check_command_line_options \
	check_script \
	check_hr \
	check_ligatures \
	check_links_inline \
	check_html5 \
	check_latex \
	check_bufferoverflow

EXTRA_DIST = README \
						testcases/*.html \
						testcases/*.output \
						$(TESTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

DISTCLEANFILES = testcases/*.output.test

.PHONEY: test

test: check

all: check

