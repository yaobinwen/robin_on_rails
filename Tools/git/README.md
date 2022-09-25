# Git

## 1. Overview

This folder is all about [Git](https://git-scm.com/), the distributed version control system.

The script `concepts.py` creates a graph to show the relations between the git "concepts". By "concepts" I mean the terms defined in the [Git Glossary](https://git-scm.com/docs/gitglossary). Whenever I start to learn a new technology, I find it usually hard to get the big picture of all the concepts of this technology and, more importantly, their relations. The idea is: If I can get the big picture, I can quickly figure out which batch of concepts are the more fundamental ones that other concepts are built upon. I can then start with those fundamental ones and climb up the tree, layer by layer, until I get to the top eventually.

The references that are used in this document include:

- [1] [Pro Git: The official Git book](https://git-scm.com/book/en/v2)
- [2] [Git Hooks](https://githooks.com/): This website has a lot of resources about using Git hooks to improve one's productivity.
- [3] [Learn Git Branching](https://learngitbranching.js.org/): A good online tool to learn git branching

## 2. General Usage

### 2.1 Use `vim` as the editor**

`git config --global core.editor "vim"`

### 2.2 Convert a normal Git repository to a bare one

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

## 3. Branching

### 3.1 How to clean up the non-existing remote branches (the `origin` ones)?

When the remote branches (shown as the `origin` ones in your local clone) no longer exist on the remote repository, `git branch --all` still shows them. How to delete these `origin` branches from the local clone? [This answer](https://stackoverflow.com/a/3184742/630364) gives three methods.

First, if you know which branch you want to delete, run `git branch -r -d origin/branch-name` where `-r` is short for `--remotes` and `-d` for `--delete`.

Alternatively, if you want to prune all the non-existing branches, run `git remote prune origin` or `git fetch origin --prune`.

## 4. Merging

### 4.1 Us vs Them

See [Git Merge: Us vs Them](./git-merge-us-vs-them.md).

## 5. Signing

### 5.1 How to use the specified GPG key to sign the commits?

`git config --global user.signingKey "0xA788F5525815CBC6DF91A36E851F38D2609E665D"  # use key's fingerprint`

Then run `git commit -S ...` to sign the commit.

### 5.2 How to GPG sign old commits (automatically)?

[Can you GPG sign old commits?](https://superuser.com/a/1123928/224429)

`git rebase --exec 'git commit --amend --no-edit -n -S' -i <base-commit>`

But also note [a comment in the answer](https://superuser.com/questions/397149/can-you-gpg-sign-old-commits#comment2343221_1123928) says that, technically, the command above does not sign the "old" commits, but just creates new commits that contain the same contents with additional signatures.

## 6. Revisions

### 6.1 How to list different commits between two branches?

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

### 6.2 How to check if the branch contains a specific commit?

`git branch --contains <SHA-1>`

### 6.3 Double-dot Selection

See the "Example history for range selection":

![Example history for range selection](https://git-scm.com/book/en/v2/images/double-dot.png)

```
$ git log master..experiment
D
C
$ git log experiment..master
F
E
```

The general form is: `git log <common-ancestor-of-from-and-to-branches>..<to-branch>`. One confusion of `master..experiment` is it makes you think the output should include all the commits that traverse from `master` through the common ancestor to `experiment`, but in fact the `master` here actually denotes the "common ancestor of `master` and `experiment`".
