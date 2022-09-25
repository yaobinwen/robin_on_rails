#!/usr/bin/python3


import argparse
import pathlib


def _syntax():
    p = argparse.ArgumentParser()

    p.add_argument(
        "--scan-id-list",
        help="Scan ID list",
        nargs="+",
        required=True,
        type=int,
    )

    p.add_argument(
        "--output-dir",
        help="Output directory path",
        required=True,
        type=pathlib.Path,
    )

    return p


def _main(scan_id_list, output_dir):
    print(f"'scan_id_list' type: {scan_id_list.__class__}")
    print(f"'scan_id_list' element type: {scan_id_list[0].__class__}")
    print(f"'scan_id_list' data: {scan_id_list}")

    print(f"'output_dir': {output_dir}")



if __name__ == "__main__":
    _main(**vars(_syntax().parse_args()))
