# My git commands

## Installation
```console
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
