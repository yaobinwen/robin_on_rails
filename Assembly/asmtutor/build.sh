#!/bin/sh

ASM_FILENAME="$1"
: "${ASM_FILENAME:?not set}"

FILENAME_STEM="$(python3 -c "from pathlib import Path; p=Path('${ASM_FILENAME}'); print(p.stem)")"

# `-g` generates the debug symbols.
# `-F dwarf` generates the debug symbols in DWARF format.
nasm \
  -g -F dwarf \
  -f elf -o "${FILENAME_STEM}.o" "$ASM_FILENAME"
ld -m elf_i386 "${FILENAME_STEM}.o" -o "${FILENAME_STEM}.exe"
