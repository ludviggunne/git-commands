PREFIX=.

all:
	@echo "Nothing to do here. Use make install PREFIX=... instead."

install: \
	install-git-rev-pick \
	install-git-note \
	install-git-browse \
	install-git-vibranch \

install-%: %
	install -Dm755 $(<) $(PREFIX)/bin/$(<)
