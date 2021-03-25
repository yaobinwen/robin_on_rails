# Git

## Overview

This folder is all about [Git](https://git-scm.com/), the distributed version control system.

A good online tool to learn git branching: https://learngitbranching.js.org/

## List

- `concepts.py`: This script creates a graph to show the relations between the git "concepts". By "concepts" I mean the terms defined in the [Git Glossary](https://git-scm.com/docs/gitglossary). Whenever I start to learn a new technology, I find it usually hard to get the big picture of all the concepts of this technology and, more importantly, their relations. The idea is: If I can get the big picture, I can quickly figure out which batch of concepts are the more fundamental ones that other concepts are built upon. I can then start with those fundamental ones and climb up the tree, layer by layer, until I get to the top eventually.

## HOWTOs

---

**How to clean up the non-existing remote branches (the `origin` ones)?**

When the remote branches (shown as the `origin` ones in your local clone) no longer exist on the remote repository, `git branch --all` still shows them. How to delete these `origin` branches from the local clone? [This answer](https://stackoverflow.com/a/3184742/630364) gives three methods.

First, if you know which branch you want to delete, run `git branch -r -d origin/branch-name` where `-r` is short for `--remotes` and `-d` for `--delete`.

Alternatively, if you want to prune all the non-existing branches, run `git remote prune origin` or `git fetch origin --prune`.

---

**Convert a normal Git repository to a bare one**

[How to convert a normal Git repository to a bare one?](https://stackoverflow.com/q/2199897/630364) has two top answers.

The first one does this:

```
cd repo
mv .git ../repo.git # renaming just for clarity
cd ..
rm -fr repo
cd repo.git
git config --bool core.bare true
```

The second one does this:

```
git clone --bare /path/to/repo
```

But they are not equivalent. The comment says the `git clone --bare` way doesn't preserve config options which "can be critical to proper operation such as if you use git-p4". Read the comments to see more discussions on them.

---

**How to list different commits between two branches?**

[List Different Commits Between Two Branches](https://til.hashrocket.com/posts/18139f4f20-list-different-commits-between-two-branches)

```
$ git log --left-right --graph --cherry-pick --oneline feature...master
```

> This lists commits with the first line of their messages. It also includes either a `<` or `>` arrow at the front of each commit indicating whether the commit is on the left (`feature`) or right (`master`) branch, respectively.

Some example output:

```
> 10d0f13 Add notes about X Window system
> bc00096 Add notes about Docker
>   3d894fa Merge branch 'master' of github.com:yaobinwen/robin_on_rails
|\  
| > e514cde gpg: Signature made Mon 18 Mar 2019 01:03:28 PM EDT
|   gpg:                using RSA key 4AEE18F83AFDEB23
|   gpg: Can't check signature: No public key
|   Add a new git HOWTO: Convert a normal Git repository to a bare one
> 1a4c784 Add notes about power management
< 13c579b (origin/dev/oauth-2.0-demo) Show a message when authorization request is denied
< e566926 Implement server's grant and deny operations
< 3f0a883 Fix PEP8 errors
< 424e665 Use Location to redirect the browser to server
```

You can replace `--online` with `--stat` to show the details (including the modified files) of each commit.

---

**How to check if the branch contains a specific commit?**

`git branch --contains <SHA-1>`

## References

- [Pro Git: The official Git book](https://git-scm.com/book/en/v2)
- [Git Hooks](https://githooks.com/): This website has a lot of resources about using Git hooks to improve one's productivity.
