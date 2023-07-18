# Makefiles

[GNU make](https://www.gnu.org/software/make/manual/make.html)

### makefile Processing Work Flow

- > By default, make starts with the first target (not targets whose names start with ‘.’). This is called the _default goal_.
- References:
  - ["2.3 How make Processes a Makefile"](https://www.gnu.org/software/make/manual/make.html#How-Make-Works)

### Variables

- Define a variable: `objects = main.o kbd.o command.o display.o insert.o search.o files.o utils.o`
- Use a variable: `$(objects)` or `${objects}`.
  - `$x` also dereferences the variable `x` but this use is highly discouraged.
- Two flavours:
  - `x = y`: recursively expanded variable
  - `x := y` or `x ::= y`: simply expanded variables
  - `x ?= y`: conditional variable assignment operator: `x` is assigned with `y` if `x` is currently not defined yet.
- References:
  - [2.4 Variables Make Makefiles Simpler](https://www.gnu.org/software/make/manual/make.html#Variables-Simplify)
  - [6 How to Use Variables](https://www.gnu.org/software/make/manual/make.html#Using-Variables)

### 5.5 Errors in Recipes

- > To ignore errors in a recipe line, write a ‘-’ at the beginning of the line’s text (after the initial tab).
- > For example,
```
clean:
        -rm -f *.o
```
- > When you run make with the ‘-i’ or ‘--ignore-errors’ flag, errors are ignored in all recipes of all rules. A rule in the makefile for the special target .IGNORE has the same effect, if there are no prerequisites. These ways of ignoring errors are obsolete because ‘-’ is more flexible.
- > Usually when a recipe line fails, if it has changed the target file at all, the file is corrupted and cannot be used—or at least it is not completely updated. Yet the file’s time stamp says that it is now up to date, so the next time make runs, it will not try to update that file. ... So generally the right thing to do is to delete the target file if the recipe fails after beginning to change the file. make will do this if .DELETE_ON_ERROR appears as a target. This is almost always what you want make to do, but it is not historical practice; so for compatibility, you must explicitly request it.
