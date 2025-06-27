# My git commands

## Installation
```console
$ make
$ sudo make install PREFIX=...
```

## Commands

### git rev-pick
Opens `fzf` to let you pick a revision interactively, and points a (new) ref `PICK` to it.
`git rev-pick` accepts the same arguments as `git log`.

#### Example:
```console
$ git rev-pick HEAD~3..
$ git checkout PICK
```

### git note
Opens a note file specific to the current branch in the default editor.
The note file is located at `$GIT_DIR/info/notes/<branch name>`.

### git browse
Pages through diff hunks for a specified commit and lets you open an
editor at the current location.

### git vibranch
Like moreutils' vidir but for git branches. Let's you batch rename/delete branches using a text editor.
