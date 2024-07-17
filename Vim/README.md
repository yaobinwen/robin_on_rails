# Vim

## Overview

After using various IDEs (e.g., Visual Studio Code) for so many years, I finally decided to pick up command-line editors such as `Vim`. One reason is I need to use these tools in many situations anyway, and not knowing how to use them well has hindered me from working more productively.

This document is my own cheatsheet of using `Vim`.

## References

- [1] [vim/vim](https://github.com/vim/vim): The official Vim repository.
- [2] [vimawesome](https://vimawesome.com/): Awesome Vim plugins from across the universe.

## One-liners

This section lists the operations (usually shortcuts) that can be explained in one line. For operations that need more details to understand, see the subsequent sections.

- `gg`: Move to **beginning of file**.
- `Shift + G`: Move to **end of file**.
- `daw`: Delete word around cursor.
- `caw`: Delete word around cursor and put to "insert" mode, i.e., "change the word".

## Remove unnecessary indents while keeping the relative indentation

Delete the text line and then paste with `]p` with your cursor from an empty line:

```
dG]p
```

`]p` will paste the text, but will adjust the indent to the current line.

Also look at [vim-unimpaired]([)https://github.com/tpope/vim-unimpaired).

## Substitute (i.e., find & replace)

The general syntax is `:[%]s/<search_phrase>/<replace_phrase>/<options>` where `options` can be:
- `%`: Specify the substitution scope:
  - Without `%`: The substitution only happens on the current line.
  - With `%`: The substitution happens in the entire file.
- `c`: Ask to confirm before replacing.
- `i`: Ignore the case.
- `g`:
  - Without `g`: Only substitute the first occurrence in the current substitution scope.
  - With `g`: Substitute all the occurrence in the current substitution scope.

Examples:
- `s/i/I`: On the current line, replace the first occurrence of `i` with `I` without confirmation. Note that words like `this` will be replaced as `thIs`.
- `s/the/THE/cg`: On the current line, replace all `the` with `THE` and confirm with the user at every occurrence.
- `%s/the/THE/cg`: In the current entire file, replace all `the` with `THE` and confirm with the user at every occurrence. Note that words like `nonetheless` will be replaced as `noneTHEless`.

## Split and navigate windows

Splitting:
- Split horizontally: `:sp [file path]`.
- Split vertically: `:vsp [file path]`.

Navigating:
- `Ctrl-w-w` navigates through the windows.
- `Ctrl-w-W` navigates through the windows in the opposite direction as `Ctrl-w-w`.
- `Ctrl-w-N-w` jumps to the Nth window.
- `Ctrl+w +p`: Previous, last accessed window.
- `Ctrl+w +t`: Top-left window.
- `Ctrl+w +b`: Bottom-right window.

Reordering:
- `Ctrl+w +r`: Shift the current window forward (i.e., changing its position).
- `Ctrl+w +R`: Shift the current window backward.

Resizing:
- `Ctrl+w+N++`: Vertically increase.
- `Ctrl+w+N+-`: Vertically decrease.
- `Ctrl+w+N+>`: Horizontally increase.
- `Ctrl+w+N+<`: Horizontally decrease.

Closing:
- `quit` or `close`: Close the current window.
- `quitall`: Close Vim.
- `only`: Close other windows and only leave the current window.

Buffer list:
- `buffers`

## Indentation

- Insert mode:
  - Tab and Backspace
  - `Ctrl-t`: Indent forward (increase indentation)
  - `Ctrl-d`: Indent backward (decrease indentation)
- Visual mode:
  - `>`: Increase indentation
  - `<`: Decrease indentation

## Insert Unicode characters

- Press `i` or `a` into insert mode.
- Press `Ctrl-v`. A `^` will appear at the cursor.
- For code points in the range of `0~ffff`, press `u` (lower case), followed by the code point, followed by `ESC`. For example, `u2122<ESC>` for `™`.
- For code points in the range higher than `ffff`, press `U` (upper case), followed by the code point, followed by `ESC`. For example, `u1f37b<ESC>` for `🍻`.

---

(To be continued)
