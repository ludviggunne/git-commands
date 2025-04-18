PREFIX=.

all:
	@echo "Nothing to do here. Use make install PREFIX=... instead."

install: install-git-rev-pick

install-%: %
	install -Dm755 $(<) $(PREFIX)/bin/$(<)
