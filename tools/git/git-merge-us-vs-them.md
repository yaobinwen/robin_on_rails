# Git Merge: Us vs Them

## Question

After a Git rebase, and in other circumstances, you can find some files marked as deleted by us in the git status report. Who is us according to Git and why?

Is it referring to me sitting on this branch and it working for me? Or is it referring to itself and the folks working on the branch I am rebasing against?

## Answer

Link: https://stackoverflow.com/a/63911630/630364

<!-- language-all: lang-bash -->

(This also answers the question: "How does a git rebase work and what exactly is happening with it?")

## In all cases: 

1. **"us"** (or **"ours"**) = the currently-checked out commit (`HEAD`) _at the moment git does the action_ which causes the conflict (more on this later), and:
1. **"them"** (or **"theirs"**) = the other commit, NOT checked-out by git _at the moment git does the action_ which causes the conflict (more on this later).

_IMPORTANT: `HEAD` at the moment it does the action which causes the conflict is NOT necessarily the `HEAD` at the moment you type the git command. This is essential to understand. Git may do some checkouts and change the `HEAD` (which commit is checked-out) before running the action which causes the conflict, thereby causing "us" and "them" to appear swapped or backwards to the untrained eye._

## The 4 cases, in detail: merge, cherry-pick, rebase, revert:

1. `git merge` (intuitive):
    1. Sample command: 
        ```
        git checkout master
        git merge feature_branch  # merge feature_branch into master
        ```
    1. **"us"/"ours"** = `HEAD`, which is `master`, because you were on branch `master` at the time you ran `git merge feature_branch`.
    1. **"them"/"theirs"** = `feature_branch`, which is the branch you're merging into `master`.
1. `git cherry-pick` (intuitive):
    1. Sample command: 
        ```
        git checkout feature_branch
        git cherry-pick some_commit  # apply some_commit to feature_branch
        ```
    1. **"us"/"ours"** = `HEAD`, which is `feature_branch`, because you were on branch `feature_branch` at the time you ran `git cherry-pick some_commit`.
    1. **"them"/"theirs"** = `some_commit`, which is the commit you're cherry-picking onto `feature_branch`.
1. `git rebase` (counter-intuitive, but totally makes sense once you understand the mechanics of how it works):
    1. Sample command: 
        ```
        git checkout feature_branch
        git rebase master  # rebase feature_branch onto latest master
        ```
    1. Diagram of this (drawn at https://asciiflow.com), with the _latest_ or _newest_ commits on top and/or to the right:
        ```
        #             Prior to rebase: feature_branch
        #             received new commits while
        #             master did too
        # 
        #                           master
        #                           x
        #                           |          feature_branch
        #                           x          y
        #                           |          |
        #                           x          y
        #                           |         /
        # git merge-base      ────► x--y--y--y
        # master feature_branch     |
        #                           x
        # 
        # 
        #             After rebase: feature_branch has
        #             been completely cherry-picked onto
        #             the end of master
        # 
        #                                   feature_branch
        #                                   y'
        #                                   |
        #                                   y'
        #                                  /
        #                         y'--y'--y'
        #                         |
        #                  master x
        #                         |
        #                         x
        #                         |
        #                         x
        #                         |
        #                         x
        #                         |
        #                         x
        ```
    1. **"us"/"ours"** = `HEAD`, which is **the upstream branch: initially the last `x` commit on `master`, and then thereafter, some NEW commit, `y'`, `cherry-picked` on top of that** (this one's tricky!). This is because when you typed `git rebase master`, _git first checks out `master` as the starting point to start cherry-picking your `feature_branch` commits onto_, then it determines which commits from `feature_branch` to cherry-pick (ie: which of your `feature_branch` commits are not already on `master`). It does this by finding the `merge-base` (the commit which is common to both `feature_branch` and `master` and which can be found with `git merge-base master feature_branch`), and THEN it starts cherry-picking commits from the first one _after_ this `merge-base` and onward, working one-at-a-time, towards the last commit on `feature_branch`, onto the tip of `master`, thereby "rebasing" all "new" `y` commits you added to `feature_branch` onto the latest `master`, as new `y'` commits. Therefore, **"us"/"ours"** = `HEAD`, but since git did a new checkout behind-the-scenes to perform this rebase, `HEAD` is NOT the branch you were on when you typed `git rebase master`. Instead, **us**, or `HEAD`, is either the last `x` commit on `master` if the conflict occurs during the first `cherry-pick`, or it is whatever NEW commit, `y'`, was last successfully cherry-picked onto `master` if the conflict occurs during any later cherry-pick. **Them** is therefore the other commit, which is some `y` commit from `feature_branch` which is being applied to this new `HEAD` via a cherry-pick, in order, FROM the first `y` commit on `feature_branch` which is immediately _after_ `git merge-base master feature_branch` all the way TO the last `y` commit on `feature_branch`. _See explanation for "them" also, just below._
    1. **"them"/"theirs"** = some `y` commit from `feature_branch` which is being applied to a newly-checked-out `HEAD`, where `HEAD` is either the last `x` commit on `master` for the first cherry-pick operation during the rebase, OR one of these newly-created `y'` commits on top of `master` as `feature_branch` is "rebased", or cherry-picked one-commit-at-a-time (along your string of new commits from `git merge-base master feature_branch` to the last commit on `feature_branch`) onto `master`. _See explanation for "us" also, just above._
1. `git revert` (sort of intuitive):
    1. Sample command: 
        ```
        git checkout feature_branch
        # create a new commit to undo the changes from some_previous_commit
        # within feature_branch
        git revert some_previous_commit  
        ```
    1. _For some of the detailed, low-level mechanics of this one, see my "Results and Conclusions" section at the bottom of [my other answer here](https://stackoverflow.com/a/63911358/4561887), as well as [this very long and detailed answer by @torek here][1]._
    1. **"us"/"ours"** = `HEAD`, which is `feature_branch`, because you were on branch `feature_branch` at the time you ran `git revert some_previous_commit`. More specifically, **"us"/"ours"** contains the changes expressed by `git diff some_previous_commit..HEAD`, which are the changes from the commit being reverted to the commit we are on now.
    1. **"them"/"theirs"** = (the inverse or opposite of) `some_previous_commit`, which is the commit whose changes you're reverting (undoing, by creating a new commit on top of `feature_branch`). In other words, **"them"/"theirs"** contains the changes expressed by `git diff some_previous_commit..some_previous_commit~`, where `some_previous_commit~` is the _parent commit_ of `some_previous_commit`. This means that **"them"/"theirs"** is the _opposite_ of `some_previous_commit`, as it contains the opposite of its changes, in order to undo `some_previous_commit`'s changes.

## Example use-cases:

### Merge conflict resolution examples:
```bash
# 1. Merge `feature_branch` into `master`, accepting ALL of 
# `master`'s (`ours`) changes in the event of 
# any merge conflicts! 
git checkout master
git merge -X ours feature_branch

# 2. Merge `feature_branch` into `master`, accepting ALL of 
# `feature_branch`'s (`theirs`) changes in the event of 
# any merge conflicts! 
git checkout master
git merge -X theirs feature_branch
```

Here are some more. **These are my most-commonly-used techniques**, rather than the 2 examples just above.

```bash
# 3. Assuming this merge attempt results in merge conflicts in
# these 3 files, but we know all of the changes on the `master`
# branch side are the ones we wish to keep, check out these 3 
# files from `master` (`--ours`) to overwrite the conflicted
# files. Then, add these now-fixed files to stage them for 
# committing, and continue (finish) the merge. 
git checkout master
git merge feature_branch
git checkout --ours -- path/to/somefile1.c path/to/somefile2.c path/to/somefile3.c
git add path/to/somefile1.c path/to/somefile2.c path/to/somefile3.c
git status 
git merge --continue

# 4. Assuming this merge attempt results in merge conflicts in
# these 3 files, but we know all of the changes on the `feature_branch`
# side are the ones we wish to keep, check out these 3 
# files from `feature_branch` (`--theirs`) to overwrite the conflicted
# files. Then, add these now-fixed files to stage them for 
# committing, and continue (finish) the merge. 
git checkout master
git merge feature_branch
git checkout --theirs -- path/to/somefile1.c path/to/somefile2.c path/to/somefile3.c
git add path/to/somefile1.c path/to/somefile2.c path/to/somefile3.c
git status 
git merge --continue
```

VERY USEFUL: If an _entire folder_ of conflicts exists, you can also specify to accept all conflicting changes from the `--ours` or `--theirs` branch _for the entire folder at once_, like this!:

**\*\*BEST MERGE CONFLICT RESOLUTION EXAMPLE:\*\***
```bash
# 5. [BEST EXAMPLE] Assuming this merge attempt results in merge conflicts in
# a bunch of files, some of which are inside `path/to/some/dir`, I can
# choose to accept the changes from one side or the other **for 
# all conflicts within files inside this directory**, like this!:
git checkout master
git merge feature_branch

# Keep `--theirs` for all conflicts within files inside this dir
git checkout --theirs -- path/to/some/dir
# OR: keep `--ours` for all conflicts within files inside this dir
git checkout --ours -- path/to/some/dir

# Add (stage for committing) all changes within files inside this dir 
# all at once
git add path/to/some/dir 
git status 
git merge --continue 
```

**DEALING WITH `path does not have our version` or `path does not have their version` ERRORS:**

If you ever run something like this:
```
git checkout --ours -- path/to/some/dir
```

...and it didn't work! It didn't do anything. Instead, it output these errors:
> ```
> error: path 'path/to/some/dir/file1.cpp' does not have our version
> error: path 'path/to/some/dir/file2.cpp' does not have our version
> error: path 'path/to/some/dir/file3.cpp' does not have our version
> ```

The problem is that these errored files are _deleted_ files on the `our` side, so we must `git rm` each of them manually BEFORE running `git checkout --ours -- path/to/some/dir`.

```
git rm path/to/some/dir/file1.cpp path/to/some/dir/file2.cpp \
path/to/some/dir/file3.cpp

# then try again
git checkout --ours -- path/to/some/dir
```

You can also just do this instead to automate the process:
```
git checkout --ours -- path/to/some/dir \
|& gawk '{ print $3 }' | xargs git rm

git checkout --ours -- path/to/some/dir
```

See my answer here for a detailed explanation of the commands above: https://stackoverflow.com/questions/43551979/git-checkout-ours-when-file-spec-includes-deleted-file/66665151#66665151.


### WARNING WARNING WARNING!

For a more-detailed example of the problem described below, [see my other answer here][2].

Do NOT do `git checkout -- path/to/some/dir`, nor `git checkout some_branch -- path/to/some/dir` in the middle of a conflict resolution (such as during a `merge` conflict like in the examples above), UNLESS YOU INTEND TO CHECK OUT **ALL** FILES FROM `HEAD`, or from `some_branch`, respectively, in directory `path/to/some/dir`, AND OVERWRITE THE LOCAL FILES WITH THOSE FILES, thereby _not_ just _accepting the conflicted changes from one side or the other._

In other words, **during the middle of a conflict resolution**, this:

**GOOD:**
```bash
# GOOD :)
# Accept all conflicts from one side or the other (while still 
# merging changes from both sides into one, in the event of being
# in the middle of a `git merge`).

git checkout --ours -- path/to/some/dir
# OR
git checkout --ours -- path/to/some/file
```
will accept _just the changes_ from side `--ours`, which is always good and safe if that's what we want, whereas this:

**BAD:**
```bash
# BAD :(
# OVERWRITE all files with these files from `some_branch` instead,
# thereby _losing_ any changes and/or files contained in the other
# side but which are not in `some_branch`.

git checkout some_branch -- path/to/some/dir 
# OR
git checkout some_branch -- path/to/some/file
```
will fully _check out and overwrite_ the ENTIRE DIRECTORY or ENTIRE FILE, as specified, rather than _only the conflicting changes themselves._ This means you may be inadvertently deleting changes from one side or the other by doing a full checkout with `git checkout some_branch` rather than a conflict resolution with `git checkout --ours` or `git checkout --theirs`. **FOR THIS REASON, IT IS RECOMMENDED TO USE `git checkout --ours -- file_or_dir_paths` or `git checkout --theirs -- file_or_dir_paths`, NOT `git checkout some_branch -- file_or_dir_paths` whenever you are in the middle of a conflict resolution** such as for `git merge`, `git cherry-pick`, `git rebase`, or `git revert`.

HOWEVER, if you DO run `git checkout some_branch -- file_or_dir_paths` because you understand this behavior and that's what you _want_, then you need to be aware of this too: checking out an entire directory like that does NOT delete local files in that dir which do not exist at `some_branch`, like you'd expect it would. Instead, it leaves them alone in your local file system if they exist locally but not at `some_branch`. _So, you must MANUALLY remove all of those files instead._ Keep that in mind or else it will leave you very very confused in the end. Read more about this in my other answers [here (this answer has a good solution to that too)][3] and [here][4]. 

### Going further / additional notes and tips:
1. The above examples of file conflict resolution can apply in the event of conflicts in any of the various types of operations (`git merge`, `git cherry-pick`, `git rebase`, `git revert`, etc.), except you need to keep in mind what `theirs` and `ours` means for each type of conflict resolution, as explained above.
1. You can also just use branch names such as `master` or `feature_branch` in place of `-X ours`/`-X theirs` and `--ours` and `--theirs`. WARNING: Passing branch names may seem easier and clearer, but CAN BE DANGEROUS to your changes, as doing it this way does a **FULL FILE OR DIRECTORY REPLACEMENT, RATHER THAN A CONFLICT RESOLUTION FOR JUST THE CONFLICTS WITHIN THE FILES**. See the "WARNING WARNING WARNING" section above. If you DO want to do a full file replacement, however, rather than just accepting conflicting changes from one side or the other, here's how:
    ```
    # See "WARNING WARNING WARNING" section above.
    git checkout feature_branch -- path/to/somefile1.c path/to/somefile2.c path/to/somefile3.c
    ```
1. You can also check out **entire directories** rather than specifying files individually! See [this answer here](https://stackoverflow.com/a/63624168/4561887) and [my answer here](https://stackoverflow.com/questions/2364147/how-to-get-just-one-file-from-another-branch/65258783#65258783) and [my other answer here][2]. I just tested it. This works too. Again, however, pay attention to the "WARNING WARNING WARNING" section above. This does a full directory replacement, rather than just accepting conflicting changes from one side or the other for an entire folder: 
    ```
    # Check out ALL files from feature_branch which are in
    # directory "path/to/dir". See "WARNING WARNING WARNING"
    # section above.
    git checkout feature_branch -- path/to/dir
    ``` 
1. Remember, if you do intentionally use `git checkout feature_branch -- path/to/dir`, expecting/hoping it will delete local files in directory `path/to/dir` which do NOT exist at `feature_branch`, it will NOT. You must remove those files manually in your local file system before or after running the `checkout` command. Read more in my answers here:
    1. [All about checking out files or directories in git][4]
    1. [How to do a `--soft` or `--hard` git reset by path][3]


## References:
1. [my answer, which I reference all the time!] **"All about checking out files or directories in git": https://stackoverflow.com/questions/2364147/how-to-get-just-one-file-from-another-branch/65258783#65258783**
1. [my answer] https://stackoverflow.com/questions/11200839/why-git-cant-do-hard-soft-resets-by-path/66539777#66539777 --> see especially the _git terminology and definitions_ in the **"Background knowledge"** section at the end!
1. [my own answer on what "them" and "us" mean during `git revert`] https://stackoverflow.com/questions/63908012/who-is-them-and-us-in-a-git-revert/63911358#63911358
1. [@LeGEC's answer which points out that "ours/us" is always `HEAD`, and "them/theirs" is always the other branch or commit] https://stackoverflow.com/questions/63908012/who-is-them-and-us-in-a-git-revert/63908496#63908496
1. [the pre-existing answer which I cross-checked against regarding my understandings of `git merge` and `git rebase`] https://stackoverflow.com/questions/21025314/who-is-us-and-who-is-them-according-to-git/21025695#21025695
1. From `man git checkout`: "**Note that during git rebase and git pull --rebase, ours and theirs may appear swapped**":
    <!-- language: lang-none -->
    >     --ours, --theirs
    >         When checking out paths from the index, check out stage #2 (ours) or #3 (theirs) for
    >         unmerged paths.
    >    
    >         Note that during git rebase and git pull --rebase, ours and theirs may appear swapped;
    >         --ours gives the version from the branch the changes are rebased onto, while --theirs
    >         gives the version from the branch that holds your work that is being rebased.
    >    
    >         This is because rebase is used in a workflow that treats the history at the remote as
    >         the shared canonical one, and treats the work done on the branch you are rebasing as the
    >         third-party work to be integrated, and you are temporarily assuming the role of the
    >         keeper of the canonical history during the rebase. As the keeper of the canonical
    >         history, you need to view the history from the remote as ours (i.e. "our shared
    >         canonical history"), while what you did on your side branch as theirs (i.e. "one
    >         contributor’s work on top of it").
1. [answer to my question]: you can pass directory paths to git too to check out all files from entire directories, rather than having to specify files individually: https://stackoverflow.com/questions/63623581/how-do-i-accept-git-merge-conflicts-from-their-branch-for-only-a-certain-direc/63624168#63624168
1. [my answer] https://stackoverflow.com/questions/43551979/git-checkout-ours-when-file-spec-includes-deleted-file/66665151#66665151
1. For drawing pretty ASCII pictures or diagrams to place in code: https://asciiflow.com/.

## Additional Study:
1. [<s>I NEED TO STUDY THIS ANSWER MYSELF STILL!</s>--done; I've figured it out and updated my answer here now as of 7 Jan. 2020] https://stackoverflow.com/questions/63908012/who-is-them-and-us-in-a-git-revert/63915268#63915268
1. [I NEED TO STUDY AND READ THIS STILL] https://stackoverflow.com/questions/39438168/git-checkout-ours-does-not-remove-files-from-unmerged-files-list/39439137#39439137


  [1]: https://stackoverflow.com/a/63915268/4561887
  [2]: https://stackoverflow.com/a/66466786/4561887
  [3]: https://stackoverflow.com/a/66539777/4561887
  [4]: https://stackoverflow.com/a/65258783/4561887
