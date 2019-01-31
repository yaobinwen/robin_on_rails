#!/usr/bin/env python3


import argparse
import contextlib
import datetime
import os
import shutil
import subprocess
import sys
import tempfile
import unittest


# Initially, it's None.
_RSYNC_TMPDIR = None


def _syntax():
    parser = argparse.ArgumentParser(
        description="Run rsync examples.",
    )
    parser.add_argument(
        '-t', '--tmpdir',
        metavar='TMPDIR',
        help='The temporary directory to store all the examples.',
    )
    parser.add_argument(
        '-s', '--save-tmpdir',
        action='store_true',
        help=(
            'Save (do not remove) the temporary directory after all the '
            'examples are run.'
        ),
    )
    return parser


@contextlib.contextmanager
def _dtemp(tmpdir, save_tmpdir):
    if tmpdir is None:
        tmpdir = tempfile.mkdtemp(
            prefix='{name}_{datetime}_'.format(
                name='rsync-demo',
                datetime=datetime.datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
            )
        )
    try:
        yield tmpdir
    finally:
        if not save_tmpdir:
            shutil.rmtree(tmpdir, ignore_errors=True)


def _make_dir(p, spec):
    for fname, content in spec.items():
        fpath = os.path.join(p, fname)
        if content is None:
            with open(fpath, 'w'):
                pass
        elif isinstance(content, dict):
            os.mkdir(fpath)
            _make_dir(fpath, content)
        else:
            raise TypeError(
                'Expect "content" to be "dict" but is "{t}"'.format(
                    t=repr(type(content))
                )
            )


class Test_r_recursive(unittest.TestCase):

    def __init__(self, methodName):
        super(Test_r_recursive, self).__init__(methodName)
        self._method_dir = None

    def setUp(self):
        c, t = self.id().split('.')[-2:]
        self._method_dir = os.path.join(_RSYNC_TMPDIR, c, t)
        psrc = os.path.join(self._method_dir, 'src')
        os.makedirs(psrc)
        _make_dir(psrc, {
            'hello': None,
            'world': {
                'f1': None,
            },
            'empty_dir': {},
        })

    def test_r(self):
        subprocess.check_call(
            ['rsync', '-r', 'src', 'dst'],
            cwd=self._method_dir,
        )

    def test_no_r(self):
        subprocess.check_call(
            ['rsync', 'src', 'dst'],
            cwd=self._method_dir,
        )


def main(tmpdir, save_tmpdir):
    # 'unittest.main' seems to exit directly without returning, so any code
    # after that won't be executed. Use a context manager to run the cleanup
    # code.
    with _dtemp(tmpdir, save_tmpdir) as tmpdir:
        global _RSYNC_TMPDIR
        _RSYNC_TMPDIR = tmpdir        
        unittest.main()

if __name__ == '__main__':
    known_args, remaining_args = _syntax().parse_known_args()
    sys.argv[1:] = remaining_args
    main(**vars(known_args))
