# Cheat Sheet (for All)

## Overview

This is my cheat sheet for everything I need to use in my daily work. I don't want to spread them into different cheat sheets because it's inconvenient to find each one when they are separate.

## Git

### Find most recent common ancestor of two branches

See [this Stack Overflow answer](https://stackoverflow.com/a/1549155/630364). Use [`git merge-base`](https://git-scm.com/docs/git-merge-base). For example:

```bash
$ git merge-base branch2 branch3
050dc022f3a65bdc78d97e2b1ac9b595a924c3f2
```

## Linux

### Create `.tar.gz` from a folder

Create: `tar -zcvf tar-archive-name.tar.gz source-folder-name`

Extract: `tar -zxvf tar-archive-name.tar.gz`

### How to know what program is listening on a given port

`sudo lsof -i :8000`

## Vim

### Remove unnecessary indents while keeping the relative indentation

Delete the text line and then paste with `]p` with your cursor from an empty line:

```
dG]p
```

`]p` will paste the text, but will adjust the indent to the current line.

Also look at ( vim-unimpaired)[https://github.com/tpope/vim-unimpaired].

### Move to the end of file

Press Shift+G.

## Patch a folder

`cd` into the folder you want to patch and run:

```
patch --dry-run -p1 < /path/to/patch/file
```
