#!/usr/bin/env python3


'''This file contains examples of using `re`. For its documentation, see
https://docs.python.org/3/library/re.html
This document is referred to as `DOC` in the code.

Some noteworthy parts:
- The solution is to use Python’s raw string notation for regular expression
  patterns.
- Repetition qualifiers (*, +, ?, {m,n}, etc) cannot be directly nested.
- Pay attention to the concept of "greedy" and "non-greedy" match. See the
  section for "*?, +?, ??".
'''


import re
import unittest


class TestGroup(unittest.TestCase):

    def test_set_flags(self):
        ''' (?aiLmsux)
        '''
        regex = re.compile(rb'(?im)^[a-z]+$')
        testees = [
            (b'abc', True),
            (b'aBc', True),
            (b'abc\nABC', True),
            (b'123\n456', False),
            (b'abc123', False),
            (b'123', False),
        ]
        for t in testees:
            self.assertEqual(regex.match(
                t[0]) != None, t[1], '{s} failed'.format(s=t[0])
            )

    def test_num_ref(self):
        ''' (...)
        '''
        regex = re.compile(rb'^[x-z]+(a.c)[x-z]+(l.n)[x-z]+')
        match = regex.match(b'xaxcylynz_kkkk')
        self.assertIsNotNone(match)
        # group(0) is the entire match (not necessarily the entire string).
        self.assertEqual(match.group(0), b'xaxcylynz')
        self.assertTupleEqual(match.group(1, 2), (b'axc', b'lyn'))
        self.assertRaises(IndexError, match.group, 3)

    def test_name_ref(self):
        ''' (?P<name>...)
        '''
        regex = re.compile(rb'^[x-z]+(?P<no1>a.c)[x-z]+(?P<no2>l.n)[x-z]+')
        match = regex.match(b'xaxcylynz_kkkk')
        self.assertIsNotNone(match)
        # group(0) is the entire match (not necessarily the entire string).
        self.assertEqual(match.group(0), b'xaxcylynz')
        self.assertTupleEqual(match.group('no1', 'no2'), (b'axc', b'lyn'))
        self.assertRaises(IndexError, match.group, 'no3')


if __name__ == '__main__':
    unittest.main()
