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
    '''Make a directory tree according to the spec.

    Args:
        p: The path in which the directory tree is to be created. If p doesn't
           exist, it will be created.
        spec: The directory tree specification.
    '''
    os.makedirs(p, exist_ok=True)
    for name, content in spec.items():
        # 'name' is in the format of '<type>_<real_name>' where '<type>' can
        # be 'L', 'F', or 'D' for symbolic link, text file, or directory.
        # Hard links and non-text files are not supported yet.
        ftype = name[:1]
        fname = name[2:]
        if ftype == 'L':
            fpath = os.path.join(p, fname)
            os.symlink(src=content, dst=fpath)
        elif ftype == 'F':
            fpath = os.path.join(p, fname)
            with open(fpath, 'w') as fd:
                if content is not None:
                    fd.write(str(content))
        elif ftype == 'D':
            fpath = os.path.join(p, fname)
            os.mkdir(fpath)
            _make_dir(fpath, content)
            pass
        else:
            raise ValueError(
                'Unrecognized type "{ftype}"'.format(ftype=ftype)
            )


# _DEFAULT_DIR_SPEC creates the tree as follows:
# src
# ├── d1
# │   ├── d11
# │   │   └── l111 -> ../f11
# │   └── f11
# ├── d2
# │   ├── d21
# │   ├── f21
# │   └── l21 -> ../d1
# ├── f1
# ├── f2
# ├── l1 -> ./d1
# ├── l2 -> ./f1
# └── l3 -> ./l1
_DEFAULT_DIR_SPEC = {
    'D_d1': {
        'D_d11': {
            'L_l111': '../f11',
        },
        'F_f11': 'f11',
    },
    'D_d2': {
        'D_d21': {},
        'F_f21': 'f21',
        'L_l21': '../d1',
    },
    'F_f1': 'f1',
    'F_f2': 'f2',
    'L_l1': './d1',
    'L_l2': './f1',
    'L_l3': './l1',
}


_DEFAULT_RSYNC_OPTIOINS = [
    '-recursive',   # recurse into directories
    '-quiet',   # suppress non-error messages
]


class _RsyncTestBase(unittest.TestCase):

    def __init__(self, methodName):
        super(_RsyncTestBase, self).__init__(methodName)
        self._method_dir = None

    def _setUpSrc(self, spec):
        c, t = self.id().split('.')[-2:]
        self._method_dir = os.path.join(_RSYNC_TMPDIR, c, t)
        psrc = os.path.join(self._method_dir, 'src')
        os.makedirs(psrc)
        _make_dir(psrc, spec)

    def setUp(self):
        self._setUpSrc(spec=_DEFAULT_DIR_SPEC)

    def _call_rsync(
        self, options, default_options=_DEFAULT_RSYNC_OPTIOINS,
        src='src/', dst='dst'
    ):
        cmd = ['rsync'] + options + default_options + [src, dst]
        subprocess.check_call(cmd, cwd=self._method_dir)

class Test_r_recursive(_RsyncTestBase):

    def test_r(self):
        self._call_rsync(options=[])

    def test_no_r(self):
        self._call_rsync(options=[], default_options=[])


class Test_l_links(_RsyncTestBase):
    ''' -l, --links
    '''

    def test_l(self):
        self._call_rsync(options=['-l'])

    def test_no_l(self):
        self._call_rsync(options=[])


class Test_L_copy_links(_RsyncTestBase):
    ''' -L, --copy-links
    '''

    def test_L(self):
        self._call_rsync(options=['-L'])

    def test_no_L(self):
        self._call_rsync(options=[])


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
