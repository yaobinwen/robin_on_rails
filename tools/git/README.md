# Git

## Overview

This folder is all about [Git](https://git-scm.com/), the distributed version control system.

## List

- `concepts.py`: This script creates a graph to show the relations between the git "concepts". By "concepts" I mean the terms defined in the [Git Glossary](https://git-scm.com/docs/gitglossary). Whenever I start to learn a new technology, I find it usually hard to get the big picture of all the concepts of this technology and, more importantly, their relations. The idea is: If I can get the big picture, I can quickly figure out which batch of concepts are the more fundamental ones that other concepts are built upon. I can then start with those fundamental ones and climb up the tree, layer by layer, until I get to the top eventually.

## HOWTOs

---

**How to clean up the non-existing remote branches (the `origin` ones)?**

When the remote branches (shown as the `origin` ones in your local clone) no longer exist on the remote repository, `git branch --all` still shows them. How to delete these `origin` branches from the local clone? [This answer](https://stackoverflow.com/a/3184742/630364) gives three methods.

First, if you know which branch you want to delete, run `git branch -r -d origin/branch-name` where `-r` is short for `--remotes` and `-d` for `--delete`.

Alternatively, if you want to prune all the non-existing branches, run `git remote prune origin` or `git fetch origin --prune`.

## References

- [Pro Git: The official Git book](https://git-scm.com/book/en/v2)
- [Git Hooks](https://githooks.com/): This website has a lot of resources about using Git hooks to improve one's productivity.
