# The Programming Languages

## Overview

This document is about all kinds of programming languages. The mentioned languages are listed in the alphabetical order.

## C++

- [ISO C++](https://isocpp.org/)
- [The Definitive C++ Book Guide and List](https://stackoverflow.com/questions/388242/the-definitive-c-book-guide-and-list)
- [Using C++11’s Smart Pointers](http://umich.edu/~eecs381/handouts/C++11_smart_ptrs.pdf)
- How `switch` is implemented: [From Switch Statement Down to Machine Code](http://lazarenko.me/switch/)
- [C++ Compiler Explorer](https://godbolt.org/) shows you how the C++ source code is generated to machine code.
- [Duff's device](https://en.wikipedia.org/wiki/Duff%27s_device) is "a way of manually implementing [loop unrolling](https://en.wikipedia.org/wiki/Loop_unrolling) by interleaving two syntactic constructs of C: the do-while loop and a switch statement."
- [再再论指针](https://blog.csdn.net/megaboy/article/details/482783): This is a series of articles I read to learn the C pointers. They are old.
- [Coliru](https://coliru.stacked-crooked.com/): An online tool to write C++ code for quick verification of something.
- [How to read in a file in C++](http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html)
- Rvalue references:
  - [A Brief Introduction to Rvalue References](https://www.artima.com/cppsource/rvalue.html)
  - [C++ Rvalue References Explained](http://thbecker.net/articles/rvalue_references/section_01.html)
- Default initialization of POD types: See [this answer](https://stackoverflow.com/a/15212447/630364). Note it uses C++ 03 standard.

## JavaScript

- [Understanding delete](http://perfectionkills.com/understanding-delete/)
- [Memory leak patterns in JavaScript](https://www.ibm.com/developerworks/web/library/wa-memleak/)

## Lisp

- [Practical Common Lisp](http://www.gigamonkeys.com/book/)

## Python

- [Real Python](https://realpython.com/)
- [Python 3 Module of the Week](https://pymotw.com/3/)
- [Idioms and Anti-Idioms in Python](https://docs.python.org/3.1/howto/doanddont.html)
- [Porting Python 2 Code to Python 3](https://docs.python.org/3/howto/pyporting.html)
- [Cheat Sheet: Writing Python 2-3 compatible code](http://python-future.org/compatible_idioms.html)
- Implicit namespace packages:
  - This concept was introduced by [PEP 420](https://www.python.org/dev/peps/pep-0420/), but I think the PEP does a poor job to explain what an "implicit namespace package" is and what problem it tries to resolve.
  - Instead, I found [this Stack Overflow answer](https://stackoverflow.com/a/21819733/630364) explains it really well.
  - Also take a look at the official sample code: [Python Namespace Package Examples](https://github.com/pypa/sample-namespace-packages)
- Python Packaging: see [`python/packaging/README.md`](./python/packaging/README.md).

## POSIX Shell

See the folder `linux/shell`.

## References

- [Exercism.io: Code practice and mentorship for everyone](https://exercism.io/)
