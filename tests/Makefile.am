
EXTRA_DIST = entities.html entities.html utf-8-sampler.html utf-8-sampler.output

tests: entities utf8

ENTITIES = "tsetstset "
entities:
	../src/vilistextum 2>/dev/null -m entities.html entities.output.test ; \
	diff -q entities.output entities.output.test && echo "$(ENTITIES) passed" || echo "$(ENTITIES) failed"

UTF8 = "utf8"
utf8:
	../src/vilistextum 2>/dev/null -m utf-8-sampler.html utf-8-sampler.output.test ; \
	diff -q utf-8-sampler.output utf-8-sampler.output.test && echo "$(UTF8) passed" || echo "$(UTF8) failed"

