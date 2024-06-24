# Python Questions

## Basic Questions

(A general sense about Python.)

What do you think are the **advantages and disadvantages** of Python compared to other programming languages such as C++?
- 1). Follow-up: **How do you guarantee the correctness of the code?** (Rationale: Because of the dynamic-typing nature of Python code, we need unit tests to make sure the code works well to some certain level.)

---

Which version of Python do you use the most? Python 2 or 3?

---

(About `argparse` because we use it a lot.)

**Have you ever used the Python module `argparse`?**

---

(Tuple and list)

- Have you ever used `tuple` and `list`?
- When you write code, how do you decide which one you want to use?
  - If the list of data will never be changed, I prefer `tuple` because it's more memory efficient.
  - If I want to dyanmically shrink or extend the list, I'll prefer `list`.
- Have you ever used `namedtuple`?
  - A tuple can only access members via index, e.g., t[0], t[1].
  - A named tuple allows access to members via names, e.g., t.first_name, t.last_name.

---

What is an f-string?

[Python 3's f-Strings: An Improved String Formatting Syntax (Guide)](https://realpython.com/python-f-strings/)

---

Have you written test code in Python? What modules do you use to write test code?
- `unittest`, at least.
- Did you use `mock` and `patch` in the `unittest` module? In what situations do you usually use them?
  - `mock`: Want to test how the target is used, make sure the target is used correctly. Instead, we don't worry too much about whether the target produces the correct values.
  - `patch`: Only want to alter the behavior of part of the target.

---

**How do you copy an object in Python?**
- `copy.copy`: Shallow copy: constructs a new compound object and then (to the extent possible) inserts references into it to the objects found in the original.
- `copy.deepcopy`: Deep copy: constructs a new compound object and then, recursively, inserts copies into it of the objects found in the original.

---

`*args` and `**kwargs`:
- What are they?
- When should we use them?
- What do you think are the pros and cons of using them vs. listing all the arguments in the function signature?

[Python args and kwargs: Demystified](https://realpython.com/python-kwargs-and-args/)

---

(Localization vs Internationalization)

`str` vs` bytes`:
- What's the difference?
  - `str` are characters of Unicode; `bytes` are a sequence of bytes, and each byte is a data block of 8 bits. They may or may not represent a character.
- How to convert them from one to the other?

---

(Localization vs Internationalization)

Have you ever used the Python module `locale`? Do you know what it is about?

## Advanced Questions

Pass-by-assignment:

[Pass by Reference in Python: Background and Best Practices](https://realpython.com/python-pass-by-reference/)

```python
#!/usr/bin/python3

def F(x):
    x = 10

A = 1
F(A)
print(A)    # What is the output and why?
```

> Python passes arguments neither by reference nor by value, but by assignment.

---

`with` and context manager: [Context Managers and Python's with Statement](https://realpython.com/python-with-statement/)
- Have you used context managers before?
- If you want to write a class that can be used in a context, what are the methods you need to implement?
  - `__enter__`: called by the `with` statement to enter the runtime context. For setup.
  - `__exit__`: called when the execution leaves the with code block. For teardown.

---

**Decorators**: Have you ever used decorators before? Have you ever written your own decorators before? What is the purpose of decorators? How do they work?

[Primer on Python Decorators](https://realpython.com/primer-on-python-decorators/)

---

**Generators vs iterators:**
- 1. How are they different?
  - Generators generate data on the fly; iterators have all the data ready.
  - Generators generate data every time; iterators do not generate data on every iteration.
  - Generators can't be accessed randomly; iterators provide random access.
- 2. What are the pros and cons of the two and when should we use one over the other?

---

**Global Interpreter Lock**
- a). How does it potentially affect the program execution? (Only one thread running at a time.)
- b). What kind of problems are not very suitable for Python? (Not suitable for CPU-bound problems where we want to utilize the CPU as much as possible)
- c). What methods can be used to deal with the potential impact caused by GIL? (e.g., multiprocessing)

[What Is the Python Global Interpreter Lock (GIL)?](https://realpython.com/python-gil/)

---

Memory management: [Memory Management in Python](https://realpython.com/python-memory-management/)

---

Namespaces & Scope: [Namespaces and Scope in Python](https://realpython.com/python-namespaces-scope/)

## Misc.

**Error handling**

Which one do you prefer? Exit code? Exceptions? What are the pros and cons of them?

---

**Performance**

If you find the system performance is not good enough, how would you usually approach to the problem to find the solution? If you had such experience in your previous projects, could you talk about it?
- b). Follow-up: **Let's assume that later we figured the bottleneck is in some Python code. What possible methods can you think of to make it run faster?** (Two important methods are concurrent/parallel programming and the use of C/C++ extension.)
