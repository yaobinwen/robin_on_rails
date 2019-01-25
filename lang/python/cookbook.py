#!/usr/bin/env python

'''The cookbook contains various useful code snippets.
'''


def dict_comprehension():
    '''PEP 274 - Dict Comprehensions: https://www.python.org/dev/peps/pep-0274/
    The example comes from [here](https://stackoverflow.com/a/14507637/630364).
    '''
    print {n: n**2 for n in range(5)}


def main():
    dict_comprehension()


if __name__ == '__main__':
    main()
