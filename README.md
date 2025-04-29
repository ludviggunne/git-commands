# My git commands

## Installation
```console
$ sudo make install PREFIX=...
```

## Commands

### git rev-pick
Opens `fzf` to let you pick a revision interactively, and points a (new) ref `PICK` to it.

#### Example:
```console
$ git rev-pick
$ git checkout PICK
```
