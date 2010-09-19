TESTS =  check_bufferoverflow check_tags check_entities check_latin2 check_utf8 check_fallback_entities

EXTRA_DIST = README tags.html tags.output entities.html entities.output \
						latin2.html latin2.output \
						bufferoverflow.html bufferoverflow.output \
						fallback_entities.html fallback_entities.output \
						utf-8-to-latin1.html utf-8-to-latin1.output \
						utf-8-sampler.html utf-8-sampler.output $(TESTS)

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

DISTCLEANFILES = \
				bufferoverflow.output.test \
				tags.output.test \
				entities.output.test \
				latin2.output.test \
				utf-8-to-latin1.output.test \
				utf-8-sampler.output.test \
				fallback_entities.output.test

.PHONEY: test

test: check

all: check

