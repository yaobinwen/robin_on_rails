# C/C++ Mock

## Overview

The problem I hope I could address is the pain of testing the code that makes system calls in order to test if the code handles the system call errors correctly.

However, this may be limited because C++ code has name mangling that makes this harder.

## References

- [1] [Advice on Mocking System Calls](https://stackoverflow.com/a/2925879/630364)
  - The option 1 uses  the `--wrap` option of GNU compiler.
- [2] [Redirecting functions in shared ELF libraries](https://www.codeproject.com/Articles/70302/Redirecting-functions-in-shared-ELF-libraries)
