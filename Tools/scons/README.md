# SCons

## Overview

This folder has the notes for [SCons](https://scons.org/).

I'm reading [SCons User Guide 2.3.0](https://scons.org/doc/2.3.0/HTML/scons-user.html)(refered to as `DOC` in this file) because I'm using this version.

The notation `DOC-X.Y` means you can find more details in Chapter X, Section Y.

## Notes

SCon's build configuration files are actually **Python scripts**.

`builder_method`[DOC-2.1] is "a Python call that tells SCons that you want to build an executable program", such as:
- [`Program`](https://scons.org/doc/2.3.0/HTML/scons-user/a8588.html#b-Program)[DOC-2.1]: Build an executable.
- [`Object`](https://scons.org/doc/2.3.0/HTML/scons-user/a8588.html#b-Object)[DOC-2.2]: Build an object file.
- [`Library`](https://scons.org/doc/2.3.0/HTML/scons-user/a8588.html#b-Library)[DOC-4.1]: Build a **static** library.
  - [`StaticLibrary`](https://scons.org/doc/2.3.0/HTML/scons-user/a8588.html#b-StaticLibrary): Synonym of `Library`, build a **static** library, too.
- [`SharedLibrary`](https://scons.org/doc/2.3.0/HTML/scons-user/a8588.html#b-SharedLibrary)[DOC-4.1]: Build a **shared** library.
- `Install`[DOC-11]: "...installing a file is still considered a type of file 'build'."

`scons -c` (short for `--clean`) removes the appropriate built files.

`SConstruct` file is "the SCons equivalent of a `Makefile`" [DOC-2.5].
- SCons functions are order-**independent**: "the order in which the SCons functions are called in the SConstruct file does not affect the order in which SCons actually builds the programs and object files you want it to build".
- In other words, the time the function is called may **NOT** be the time the target is built. It's up to SCons to decide the appropriate time to actually build the target.

Tips:
- `Program('hello.c')` vs `Program('hello', 'hello.c')`: Specify the name of the output executable.
- `Program('program', ['prog.c', 'file1.c', 'file2.c'])`: Specify a list of input source files.
- `Program('program', Glob('*.c'))`: Use all files that match a certain pattern.
- `Program('hello', 'hello.c')` vs `Program(target='hello', source='hello.c')`

Linking with libraries[DOC-4.2]:
- Use [`LIBS`](https://scons.org/doc/2.3.0/HTML/scons-user/a5025.html#cv-LIBS) to specify the libraries to link against.
- Use [`LIBPATH`](https://scons.org/doc/2.3.0/HTML/scons-user/a5025.html#cv-LIBPATH)[DOC-4.3] to specify the directories to find the libraries.
- Example: `Program('prog.c', LIBS=['foo', 'bar'], LIBPATH='.')`

`Nodes`: "Internally, SCons represents all of the files and directories it knows about as _Nodes_."
- Builder methods return lists of target nodes. This helps write portable SCons code. See the example in DOC-5.1.
- A file Node (defined by `File`) is different from a directory Node (defined by `Dir`). `Entry` returns a Node that can represent either a file or a directory.

### Dependencies

`Decider` specifies the method to use to detect if an input file has changed.
- `Decider('MD5')` or `Decider('content')`: Build when the file's MD5 signature has changed.
- `Decider('timestamp-newer')` or `Decider('make')`: Build when the file's modification time is newer than the target.
- `Decider('timestamp-match')`: Build whenever the file's modification time is different from that that is recorded by SCons.
- `Decider('MD5-timestamp')`: Use MD5 checksums of file contents but to read those contents only when the file's timestamp has changed.
  - Use caution when using this decider with continuous integration tools because when the source file is modified within one second of the last time SCons built the file, the target will not rebuilt. See DOC-6.1.3 for more details.

The **implicit dependencies**:
- [`CPPPATH`](https://scons.org/doc/2.3.0/HTML/scons-user/a5025.html#cv-CPPPATH):
  - This causes an `-I` to be added to the build command line.
  - "The list of directories that the C preprocessor will search for include directories."
  - "The C/C++ implicit dependency scanner will search these directories for include files."
  - "**Don't explicitly put include directory arguments in CCFLAGS or CXXFLAGS** because the result will be non-portable and the directories will not be searched by the dependency scanner."

The **explicit dependencies**:
- `Depends(file, dependent_files)`[DOC-6.5] can explicitly specify the files that one file depends on. Note both `file` and `dependent_files` can be Nodes.
- `ParseDepends`[DOC-6.6] can parse the contents of the compiler output in the style of Make, and explicitly establish all of the listed dependencies. It usually needs `SideEffect`[DOC-6.6]. See the example in DOC-6.6.
- `Ignore`[DOC-6.7]:
- `Requires`[DOC-6.8]: Order-only dependencies.
- `AlwaysBuild`[DOC-6.9]: The target specified by `AlwaysBuild` is always built when it appears in the dependency tree of the target specified on the SCons command line.

### Environments

The types:
- `external environment`[DOC-7.1]: The same as Python's `os.environ`.
- `construction environment`[DOC-7.2]: Contains values that affect SCons' behaviors (not the external compiler's or linker's).
- `execution environment`[DOC-7.3]: Contains values that SCons sets when executing an external command (such as a compiler or linker) to build one or more targets.

### Installing Files

- `Install`[DOC-11]
  - Installing a file is still considered a type of file "build." This is important when you remember that the default behavior of SCons is to build files in or below the current directory.
  - Examples:
    - Install one: `env.Install('/usr/bin', hello)`
    - Install multiple: `env.Install('/usr/bin', [hello, goodbye])`
- `InstallAs`[DOC-11.2]: Install a file under a different name.
  - Examples:
    - Install one: `env.InstallAs('/usr/bin/hello-new', hello)`.
    - Install multiple using lists of the same length: `env.InstallAs(['/usr/bin/hello-new', '/usr/bin/goodbye-new'], [hello, goodbye])`.
- `Alias`[DOC-11]: Define a pseudo-target of a real target. Example: `env.Alias('install', '/usr/bin')`.

### Hierarchical Builds

In `SConstruct`, use the `SConscript`[DOC-14.1] function to include subsidary build scripts:

```
SConscript(['drivers/display/SConscript',
            'drivers/mouse/SConscript',
            'parser/SConscript',
            'utilities/SConscript'])
```

You can either list all the subsidary SConscripts in the top-level SConstruct, or arrange the SConscripts recurssively.

In the `SConscript` file, path names are relative to the SConscript directory.

When building, SCons stays in the top-level directory where the `SConstruct` file lives and issues commands that use the path names from the top-level directory to the target and source files within the hierarchy.

Use `#`[DOC-14.3] to specify the top-level `SConstruct` folder. Or simply start a path with `/`[DOC-14.4] to specify an absolute path.

Use `Export`[DOC-14.5.1] to export a _variable_ from a parent SConscript file to its subsidiary SConscript files.
- The _variable_ is in the sense of Python. For example, a construction environment is an _environment_ in the sense of SCons, but when it is defined in a SCons script, it is defined as a Python variable: `env = Environment()`. "Export a variable" means making this vairable visible to the subsidary SCons script.
- The variables can be exported in several ways:
  - `Export('env', 'debug')`: Export multiple variables.
  - `SConscript('src/SConscript', 'env')`: Specify the variable to export as the second argument of `SConscript` function.
    - `SConscript('src/SConscript', exports='env')`
    - `SConscript(['src1/SConscript', 'src2/SConscript'], exports='env')`

Use `Import`[DOC-14.5.2] to import the variable that are exported by other SConscript files.

Use `Return`[DOC-14.5.3] to return variables to the calling SConscript files.

## TODO

- What is a `construction variable`? It is mentioned the first time in DOC-4.2. The `Appendix A: Construction Variables` has all the construction variables.
