# Vim

## Overview

After using various IDEs (e.g., Visual Studio Code) for so many years, I finally decided to pick up command-line editors such as `Vim`. One reason is I need to use these tools in many situations anyway, and not knowing how to use them well has hindered me from working more productively.

This document is my own cheatsheet of using `Vim`.

---

## Operations

This section lists the operations that can be explained in one line. For operations that need more details to understand, see the subsequent sections.

- `Shift + G`: Move to **end of file**.
- `daw`: Delete word around cursor.
- `caw`: Delete word around cursor and put to "insert" mode, i.e., "change the word".

---

## Remove unnecessary indents while keeping the relative indentation

Delete the text line and then paste with `]p` with your cursor from an empty line:

```
dG]p
```

`]p` will paste the text, but will adjust the indent to the current line.

Also look at [vim-unimpaired]([)https://github.com/tpope/vim-unimpaired).

---

(To be continued)