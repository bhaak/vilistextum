TESTS =  check_entities check_utf8

EXTRA_DIST = entities.html entities.html utf-8-sampler.html utf-8-sampler.output

TESTS_ENVIRONMENT = top_builddir=$(top_builddir)

DISTCLEANFILES = \
        index.swish-e.prop \
        index.swish-e

.PHONEY: test

test: check

