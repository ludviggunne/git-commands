PREFIX=.

sources=$(wildcard git-*)
targets=$(sources:%=$(PREFIX)/bin/%)

all:
	@echo Nothing to do here. Use make install PREFIX=... instead.

install: $(targets)

$(PREFIX)/bin/%: %
	install -Dm755 $(<) $(@)
