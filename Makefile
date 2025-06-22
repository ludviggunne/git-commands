PREFIX=.

all: git-browse

install: \
	install-git-rev-pick \
	install-git-note \
	install-git-browse

install-%: %
	install -Dm755 $(<) $(PREFIX)/bin/$(<)

git-browse: git-browse.c
	$(CC) -lgit2 -o $(@) $(^)

clean:
	rm -rf git-browse
