#!/usr/bin/python3

A = 1

def outer():
    A = 2

    def inner():
        print(f"Inside inner(): A = {A}")   # What's the output and why?
        A = 3

    inner()
    print(f"After inner(): A = {A}")    # What's the output and why?


outer()
print(f"After outer(): A = {A}")    # What's the output and why?
