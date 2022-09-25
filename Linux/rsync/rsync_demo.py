#!/usr/bin/env python3


'''This file contains examples of using `rsync`. For its documentation,
see: http://manpages.ubuntu.com/manpages/trusty/man1/rsync.1.html
This document is referred to as `DOC` in the code.
'''


import argparse
import contextlib
import datetime
import collections
import os
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile
import time
import unittest


# The temporary directory that stores all the test files.
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
        fpath = os.path.join(p, fname)
        if ftype == 'L':
            os.symlink(src=content, dst=fpath)
        elif ftype == 'F':
            with open(fpath, 'w') as fd:
                if content is not None:
                    fd.write(str(content))
        elif ftype == 'D':
            os.mkdir(fpath)
            _make_dir(fpath, content)
            pass
        else:
            raise ValueError(
                'Unrecognized type "{ftype}"'.format(ftype=ftype)
            )


def _verify_dir_non_strict(p, spec, tc):
    '''Verify the directory tree against the spec in a non-strict way.

    Args:
        p: The path of the directory to be verified. It must exist.
        spec: The spec the directory tree must conform.
        tc: A 'unittest.TestCase' instance.

    Notes:
        Being "non-strict" means the directory tree may have more files than
        what the spec requires. For example, the spec may say "file f1 must
        exist", then it is perfectly OK if the directory have other files
        besides "f1".
    '''
    for name, content in spec.items():
        ftype = name[:1]
        fname = name[2:]
        fpath = os.path.join(p, fname)
        if ftype == 'L':
            tc.assertTrue(os.path.islink(fpath))
            tc.assertEqual(os.readlink(fpath), content)
        elif ftype == 'F':
            tc.assertTrue(
                os.path.isfile(fpath) and not os.path.islink(fpath)
            )
            with open(fpath, 'r') as fd:
                actual = fd.read()
            expected = content if content is not None else ''
            tc.assertEqual(actual, expected)
        elif ftype == 'D':
            tc.assertTrue(
                os.path.isdir(fpath) and not os.path.islink(fpath)
            )
            _verify_dir_non_strict(fpath, content, tc)
        else:
            raise ValueError(
                'Unrecognized type "{ftype}"'.format(ftype=ftype)
            )


def _verify_dir_strict(p, spec, tc):
    '''Verify the directory tree against the spec ina strict way.

    Args:
        p: The path of the directory to be verified. It must exist.
        spec: The spec the directory tree must conform.
        tc: A 'unittest.TestCase' instance.

    Notes:
        Being "strict" means the directory tree must have exactly what the spec
        specifies, no more, no less. For example, if the spec says "file f1
        must exist", the directory must only have file "f1" without anything
        else. If the directory has another file called "f2", the verification
        fails.
    '''
    # Non-strict verification is part of the strict verification. It does the
    # from-spec-to-dir direction.
    _verify_dir_non_strict(p, spec, tc)

    # __verify_strict does the from-dir-to-spec direction.
    def __verify_strict(p, spec, tc):
        for root, dirs, files in os.walk(p):
            if root != p:
                # We only do it one level.
                continue

            for d in dirs:
                dname = 'D_' + d
                tc.assertTrue(dname in spec)    # It exists.
                tc.assertIsInstance(spec[dname], dict)  # It is a directory.
                dpath = os.path.join(p, d)
                __verify_strict(dpath, spec[dname], tc)
            for f in files:
                fpath = os.path.join(root, f)
                islink = os.path.islink(p)
                prefix = 'L_' if islink else 'F_'
                fname = prefix + f
                tc.assertTrue(fname in spec)
                content = spec[fname]
                if islink:
                    tc.assertEqual(os.readlink(fpath), content)
                else:
                    with open(fpath, 'r') as fd:
                        actual = fd.read()
                    expected = content if content is not None else ''
                    tc.assertEqual(actual, expected)

    __verify_strict(p, spec, tc)


_DEFAULT_DIR_SPEC = {
    'D_d1': {
        'D_d11': {
            'L_l111': '../../f1',   # Point outside of d1
        },
        'D_d12': {
            'L_l121': '../f11',     # Point inside of d1
        },
        'F_f11': 'f11',
        'F_f12': 'f12',
        'L_l11': './f11',
        'L_l12': './f12',
    },
    'D_d2': {
        'D_d21': {},    # Empty directory
        'F_f21': 'f21',
    },
    'F_f1': 'f1',
    'F_f2': 'f2',
    'L_l1': './d1',
    'L_l2': './f1',
    'L_l3': './l1',
}


_DEFAULT_RSYNC_OPTIOINS = [
    '--recursive',   # recurse into directories
    '--stats',  # give some file-transfer stats
]


class _RsyncOutput(object):

    def __init__(self, output):
        self._output = output
        lines = output.split(b'\n')

        FileStats = collections.namedtuple(
            'FileStats', ['total', 'reg', 'dir']
        )
        p = rb'^.+ (?P<total>\d+)(?: .+ (?P<reg>\d+),.+ (?P<dir>\d+)\))?$'
        for l in lines:
            if l.startswith(b'Number of files'):
                # Number of files: 34 (reg: 22, dir: 12)
                m = re.match(p, l)
                t = int(m.group('total'))
                r = int(m.group('reg')) if t > 0 else 0
                d = int(m.group('dir')) if t > 0 else 0
                self._total_files = FileStats(total=t, reg=r, dir=d)
            elif l.startswith(b'Number of created files'):
                # Number of created files: 34 (reg: 22, dir: 12)
                m = re.match(p, l)
                t = int(m.group('total'))
                r = int(m.group('reg')) if t > 0 else 0
                d = int(m.group('dir')) if t > 0 else 0
                self._created_files = FileStats(total=t, reg=r, dir=d)
            elif l.startswith(b'Number of deleted files'):
                # Number of deleted files: 0
                pre = b'Number of deleted files: '
                self._deleted = int(l[len(pre):])
            elif l.startswith(b'Number of regular files transferred'):
                # Number of regular files transferred: 22
                pre = b'Number of regular files transferred: '
                self._reg_trans = int(l[len(pre):])
            else:
                # We don't care about other lines for now.
                pass

    @property
    def total_files(self):
        return self._total_files

    @property
    def created_files(self):
        return self._created_files

    @property
    def deleted_files(self):
        return self._deleted

    @property
    def transferred_reg_files(self):
        return self._reg_trans


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
        output = subprocess.check_output(cmd, cwd=self._method_dir)
        return _RsyncOutput(output)

    def _verify_dst(self, spec, dst=None, strict=False):
        if dst is None:
            dst = os.path.join(self._method_dir, 'dst')

        _verify_dir = _verify_dir_strict if strict else _verify_dir_non_strict
        _verify_dir(dst, spec, self)


class Test_c_checksum(_RsyncTestBase):
    ''' -c, --checksum
    '''

    def test_c(self):
        # With '-c', `rsync` uses the file checksum to decide if it should be
        # copied or not, even if the modified time changes.

        # Let's copy it once. Actually, it doesn't matter if we preserve the
        # links or not. But let's preserve them.
        self._call_rsync(options=['--links', '-c'])

        # If we copy it again, there should be nothing copied.
        output = self._call_rsync(options=['--links', '-c'])
        self.assertEqual(output.transferred_reg_files, 0)

        f1path = os.path.join(self._method_dir, 'dst', 'f1')

        # Next, just touch a file. We need to sleep for at least one second
        # to make the touch sensible. This makes the tests go slow, sadly.
        time.sleep(1)
        pathlib.Path(f1path).touch()
        # If we copy it again, there should still be nothing copied because
        # the file checksum doesn't change.
        output = self._call_rsync(options=['--links', '-c'])
        self.assertEqual(output.transferred_reg_files, 0)

        # Now let's modify the file content so its checksum changes, too.
        with open(f1path, 'a') as fh:
            fh.write('f1_new')
        # Now the file will be copied.
        output = self._call_rsync(options=['--links', '-c'])
        self.assertEqual(output.transferred_reg_files, 1)

    def test_no_c(self):
        # Without '-c', `rsync` uses the file size or modified time to decide
        # if the file should be copied or not, even if its content remains the
        # same.

        # Let's copy it once. Actually, it doesn't matter if we preserve the
        # links or not. But let's preserve them.
        self._call_rsync(options=['--links'])

        # If we copy it again, there should be nothing copied.
        output = self._call_rsync(options=['--links'])
        self.assertEqual(output.transferred_reg_files, 0)

        # Next, just touch a file. We need to sleep for at least one second
        # to make the touch sensible. This makes the tests go slow, sadly.
        time.sleep(1)
        pathlib.Path(os.path.join(self._method_dir, 'dst', 'f1')).touch()

        # If we copy it again, the touched file should be copied.
        output = self._call_rsync(options=['--links'])
        self.assertEqual(output.transferred_reg_files, 1)


class Test_r_recursive(_RsyncTestBase):

    def test_r(self):
        # Because '-r' is included by default, we don't need to include it.
        stats = self._call_rsync(options=[])
        self._verify_dst(spec={
            'D_d1': {
                'D_d11': {},
                'D_d12': {},
                'F_f11': 'f11',
                'F_f12': 'f12',
            },
            'D_d2': {
                'D_d21': {},    # Empty directory
                'F_f21': 'f21',
            },
            'F_f1': 'f1',
            'F_f2': 'f2',
        }, strict=True)

    def test_no_r(self):
        self._call_rsync(options=[], default_options=[])
        self.assertFalse(os.path.exists(os.path.join(self._method_dir, 'dst')))


class Test_l_links(_RsyncTestBase):
    ''' -l, --links
    '''

    def test_l(self):
        self._call_rsync(options=['-l'])
        # TODO(ywen): Add asserts.


class Test_L_copy_links(_RsyncTestBase):
    ''' -L, --copy-links
    '''

    def test_L(self):
        self._call_rsync(options=['-L'])
        # TODO(ywen): Add asserts.


class Test_copy_unsafe_links(_RsyncTestBase):
    ''' --copy-un-safe-links
    See 'DOC:SYMBOLIC LINKS' section for more.
    '''

    def test_copy_unsafe_links(self):
        self._call_rsync(
            options=['--links', '--copy-unsafe-links'],
            src='src/d1/'
        )
        self._verify_dst(spec={
            'D_d11': {
                # 'l111' is turned from a link to a file because it is unsafe.
                'F_l111': 'f1'
            },
            'D_d12': {
                'L_l121': '../f11'
            },
            # Do not have to verify the entire tree. Just verify the concerned
            # part.
            'L_l11': './f11',
            'L_l12': './f12',
        })


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
