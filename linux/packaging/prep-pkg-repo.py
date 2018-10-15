#!/usr/bin/env python
# -*- mode: python; coding: UTF-8; -*-


"""Prepares a simple and secure Debian package archive."""


from __future__ import print_function

import os
import subprocess32


def _run_cmd(cmd, **kwargs):
    print('Run: "{cmd_str}"'.format(cmd_str=' '.join(cmd)))
    subprocess32.check_call(cmd, **kwargs)


def _main():
    """Guide step by step through the creation of the package repository."""

    # Verify we are at the right location.
    print('Verifying we are at the right location...')
    assert os.path.isdir('./gpgkeys')
    assert os.path.isdir('./yaobin')
    assert os.path.isfile('./build.py')
    print('Succeeded!')

    # Delete the package repository folder if it already exists.
    # We always want to start with a new one.
    print('Deleting the old package repository folder, if it exists...')
    _run_cmd(['rm', '-rf', 'pkg-repo'])
    print('Succeeded!')

    # Create the folder for the package repository.
    print('Creating the package repository folder...')
    _run_cmd(['mkdir', '-vp', os.path.join(os.getcwd(), 'pkg-repo')])
    print('Succeeded!')

    # Create the binary '.deb' package and move it to 'pkg-repo'.
    print('Creating the binary .deb package...')
    _run_cmd(['./build.py', '-n', 'yaobin', '-v', '1.0.0'])
    # _run_cmd(['mv', 'yaobin-wen_1.0.0_all.deb', 'pkg-repo'])
    print('Succeeded!')

    # Sign the package.
    print('Signing the binary .deb package...')
    _run_cmd([
        'dpkg-sig',
        '-m', 'Yaobin Wen',
        # NOTE(ywen): The '--homedir ./gpgkeys' will properly quote the two
        # strings and pass them into '--gpg-options'. If you write
        # '"--homedir ./gpgkeys"', gpg complains
        # 'Invalid option "--homedir ./gpgkeys"'.
        # NOTE(ywen): By 03/30/2018, `dpkg-sig` has a bug of mismatching option
        # name. The option should be 'gpg-options', but the document says
        # 'gpgoptions'. See the bug report
        # https://bugs.launchpad.net/ubuntu/+source/dpkg-sig/+bug/1741254.
        '--gpg-options', '--homedir ./gpgkeys',
        '--sign', 'builder',
        'yaobin-wen_1.0.0_all.deb',
    ])
    print('Succeeded!')

    # Create the 'conf' folder.
    print('Creating the "conf" folder...')
    _run_cmd(['mkdir', '-vp', os.path.join('.', 'pkg-repo', 'conf')])
    print('Succeeded!')

    # Create the 'distributions' file.
    print('Creating the "distributions" file...')
    with open(
        os.path.join('.', 'pkg-repo', 'conf', 'distributions'), 'w'
    ) as fdist:
        fdist.writelines([
            'Origin: ', 'https://github.com/yaobinwen/robin_on_rails', '\n',
            'Label: ', 'Demo', '\n',
            'Codename: ', 'stretch', '\n',
            'Architectures: ', 'amd64', '\n',
            'Components: ', 'main', '\n',
            'Description: ',
                'Demo to show how to prepare a secure package repository', '\n',
            # NOTE(ywen): The key ID must match that of the public key in
            # './gpgkeys'.
            'SignWith: ', '214BA15D', '\n',
        ])
    print('Succeeded!')

    # Create the package repository.
    print('Creating the package repository...')
    _run_cmd(
        cmd=[
            'reprepro',
            '--verbose',
            '--gnupghome', './gpgkeys',
            '--basedir', 'pkg-repo',
            'includedeb', 'stretch', 'yaobin-wen_1.0.0_all.deb',
        ],
    #     env={
    #         'GNUPGHOME': './gpgkeys',
    #     }
    )
    print('Succeeded!')


if __name__ == "__main__":
    _main()
