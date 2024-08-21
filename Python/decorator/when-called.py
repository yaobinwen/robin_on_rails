#!/usr/bin/python3

import functools

print(f"(1) running module {__file__}")

print("(2a)")


def log_start_end(func):
    print(f"starting decorator {log_start_end.__name__} decorating {func.__name__}...")

    @functools.wraps(func)
    def wrapper(*args, **kwds):
        print(f"calling {func.__name__}...")
        func(*args, **kwds)
        print(f"returned from {func.__name__}")

    print(f"returning decorator {log_start_end.__name__} decorating {func.__name__}...")
    return wrapper


print(f"(2b) after {log_start_end.__name__} is defined")

print("(3a)")


@log_start_end
def func1():
    print("func1")


print(f"(3b) after {func1.__name__} is defined")

print("(4a)")


class C(object):
    @log_start_end
    def hello(self, name):
        print(f"hello, {name}")


print(f"(4b) after {C.__name__} is defined")

print("(5a)")


@log_start_end
def main():
    func1()

    c = C()
    c.hello(name="zzz")


print(f"(5b) after {main.__name__} is defined")


if __name__ == "__main__":
    main()

print(f"(5) after {main.__name__} is called")
