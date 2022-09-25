# repo

## Overview

[Repo Command Reference](https://source.android.com/setup/develop/repo)

[repo Manifest Format](https://gerrit.googlesource.com/git-repo/+/master/docs/manifest-format.md)

## Manifest File

For basic use, the following elements are enough:

- `manifest`: The root element.
- `default`: Define default values.
- `remote`: Define the remote base Git URL as the prefix for `projects` (aka the Git repositories). This doesn't refer to any specific Git repository.
- `project`: Define a specific Git repository.
  - `project` can be nested to create a Git-submodule.

Additional remotes and projects may be added through local manifest files stored in `$TOP_DIR/.repo/local_manifests/*.xml`.

See the comments in the example for more details:

```xml
<manifest>
    <default remote="boostorg" revision="master" />

    <!-- All the remotes -->

    <remote
     name="boostorg"
     fetch="https://github.com/boostorg" />

    <remote
     name="gcc"
     fetch="git://gcc.gnu.org/git" />

    <!-- C++ Boost projects -->

    <!-- `groups`:
        - Whitespace or comma separated.
        - All projects belong to the group "all".
        - Each project automatically belongs to a group of its name: `name` and
          path: `path`. See the documentation for more details.
    -->

    <!-- `revision`:
        - If obsent, use the revision defined in `default` element.
        - Can be a branch name.
        - Can be a tag or SHA-1 (but this feature is not extensively tested).
    -->

    <project
     name="atomic"
     groups="concurrent_programming"
     revision="8e387475a56f4d0977cf68da62217346afe1f9ff" />

    <project
     name="beast"
     groups="concurrent_programming,input_output"
     revision="develop" />

    <project
     name="lexical_cast"
     groups="miscellaneous string_text_processing" />

    <project
     name="statechart"
     groups="state_machines" />

    <!-- GCC projects -->

    <project
     name="gcc"
     remote="gcc"
     path="path/to/local/gcc/relative/to/top/repo/directory"
     revision="refs/tags/releases/gcc-9.3.0" />

</manifest>
```
