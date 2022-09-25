#!/usr/bin/env python


def generate_even_nums(n):
    print 'Entering...'
    for i in range(0, n):
        print 'i = ', i
        yield i * 2


g = generate_even_nums(2)
print '-' * 10
for n in g:
    print 'n = ', n


# The output is:
# ----------
# Entering...
# i =  0
# n =  0
# i =  1
# n =  2
#
# So when generate_even_nums is called, its code is NOT executed.
