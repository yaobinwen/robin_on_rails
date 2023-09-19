# Go Lang Notes

## 0. Overview

This document is the notes of learning [_The Go Programming Language_](https://www.gopl.io/).

## 1. A birds-eye view of Go

- 1). It uses **garbage collection** to manage resources.
- 2). It's well suited for building **infrastructure** like networked servers, and tools and systems for programmers.
- 3). It uses error code to do error handling; it doesn't have exceptions.
- 4). **Modules** consist of **packages**; packages consist of **source files**.
- 5). Go specifies data types after the variable names, such as `var x int` or `func f(t int32) float64`. This is similar to Python with type annotation: `def f(t: int) -> str:`.
- 6). No `;` at the end of lines.
- 7). A typical structure of a source file is:

```go
// Declare the package that this source file belongs to.
package pkg_name

import "module_1"
import "module_2"

// Package-level decalrations.
const constName = 123

func main() {
  // Implementation of main
}
```

## 2. Names

Referred chapters: 2.1

### 2.1 Valid names

A name:
- Begins with a **Unicode letter** or an **underscore**
- May have **any number** of additional letters, digits, and underscores.
- No hyphens.
- Is **case-sensitive**.

### 2.2 Keywords (cannot be used)

The following 25 keywords **cannot** be used as names:
- `break`
- `case`
- `chan`
- `const`
- `continue`
- `default`
- `defer`
- `else`
- `fallthrough`
- `for`
- `func`
- `go`
- `goto`
- `if`
- `import`
- `interface`
- `map`
- `package`
- `range`
- `return`
- `select`
- `struct`
- `switch`
- `type`
- `var`

### 2.3 Predeclared names (may be re-used)

The following _predeclared names_ may be used but not recommended for potential confusion:
- Constants:
  - `true`
  - `false`
  - `iota`
  - `nil`
- Types:
  - `int`
  - `int8`
  - `int16`
  - `int32`
  - `int64`
  - `uint`
  - `uint8`
  - `uint16`
  - `uint32`
  - `uint64`
  - `uintptr`
  - `float32`
  - `float64`
  - `complex128`
  - `complex64`
  - `bool`
  - `byte`
  - `rune`
  - `string`
  - `error`
- Functions:
  - `make`
  - `len`
  - `cap`
  - `new`
  - `append`
  - `copy`
  - `close`
  - `delete`
  - `complex`
  - `real`
  - `imag`
  - `panic`
  - `recover`

### 2.4 Visibility

- **Local**: Declared within a function.
- **Package**: Declared outside of a function.
- **Exported**: Package level with first letter in upper case, such as `Printf` in `fmt`.

### 2.5 Package names

Package names are always in lower case, such as `fmt`.

### 2.6 Camel case is preferred

For example, `QuoteRuneToASCII`, `parseRequestLine`.

The letters of acronyms and initialisms like `ASCII` and `HTML` are always rendered in the same case, e.g., `htmlEscape`, `HTMLEscape`, or `escapseHTML`, but never `escapeHtml`.

## 3. Declaration: Variables

Referred chapters: 2.4

The general form:
- 1). The general form: `var name type = expression`.
- 2). If no initial value, the variable is initialized to the **zero value** of its type.
- 3). Multiple variables of the same type: `var i, j, k int // int, int, int`.
- 4). Multiple variables of different types: `var b, f, s = true, 2.3, "four" // bool, float64, string`.
- 5). Unpacking: `var f, err = os.Open(name)`.

The short form:
- 1). `name := expression`.
- 2). Multiple ones: `i, j := 0, 1`.
- 3). Unpacking: `f, err := os.Open(name)`.
- 4). Must declare at least one new variable, otherwise compile error.

## 4. Declaration: Types

Referred chapters: 2.5

- 1). The general form: `type name underlying-type`. Examples:

```go
type Celsius float64
type Fahrenheit float64

const (
  AbsoluteZeroC Celsius = -273.15
  FreezingC     Celsius = 0
  BoilingC      Celsius = 100
)
```

- 2). Even if the user-defined types have the same underlying types, they **cannot be compared or combined in arithmetic expressions**, in order to avoid errors due to implicit type conversion.
  - a). To compare or combine them, use an explicit conversion like `Celsius(t)` or `Fahrenheit(t)`.
- 3). "In any case, a conversion never fails at run time."
