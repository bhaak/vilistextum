TESTS =  check_tags check_entities check_latin2 check_utf8

EXTRA_DIST = README tags.html tags.output entities.html entities.output \
						latin2.html latin2.output \
						utf-8-sampler.html utf-8-sampler.output $(TESTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

DISTCLEANFILES = \
				tags.output.test \
				entities.output.test \
				latin2.output.test \
				utf-8-sampler.output.test

.PHONEY: test

test: check

all: check

