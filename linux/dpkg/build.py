#!/usr/bin/env python


import argparse
import os
import subprocess32


def _syntax():
    parser = argparse.ArgumentParser(
        description='Build the specified versions of the specified package'
    )
    parser.add_argument(
        '--pkg-name', '-n',
        metavar='NAME',
        help='Name of the package to be built',
        nargs='+',
        type=str,
        required=True,
    )
    parser.add_argument(
        '--pkg-version', '-v',
        metavar='VERSION',
        help='Name of the package to be built',
        nargs='+',
        type=str,
        required=True,
    )
    return parser


def _build_pkg(pkg_name, pkg_version):
    control_file_path = '{cwd}/{pkg_name}/DEBIAN/control'.format(
        cwd=os.getcwd(), pkg_name=pkg_name
    )
    subprocess32.check_call([
        'perl', '-pi', '-e',
        's/Version: (.*)/Version: {pkg_ver}/'.format(pkg_ver=pkg_version),
        control_file_path,
    ])
    subprocess32.check_call([
        'dpkg', '--build', pkg_name, os.getcwd(),
    ])


def main(pkg_names, pkg_versions):
    for pkg_name in pkg_names:
        for pkg_version in pkg_versions:
            _build_pkg(pkg_name, pkg_version)


if __name__ == "__main__":
    args = vars(_syntax().parse_args())
    main(pkg_names=args['pkg_name'], pkg_versions=args['pkg_version'])
