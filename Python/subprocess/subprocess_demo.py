#!/usr/bin/python3

import json
import re
import subprocess

_RE_DEV_NOT_EXISTS = re.compile(r"Device .+ does not exist.")


class CalledProcessDetailedError(subprocess.CalledProcessError):
    def __str__(self):
        msg = super().__str__()
        return f"{msg} (stdout: {self.stdout}; stderr: {self.stderr})"


cmd = ["ip", "-json", "route", "show", "dev", "xyz"]
cp = subprocess.run(
    cmd, check=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding="utf-8"
)

try:
    if cp.returncode == 0:
        output = json.loads(cp.stdout)
    else:
        m = _RE_DEV_NOT_EXISTS.fullmatch(cp.stdout)
        if m is not None:
            output = None
        else:
            raise CalledProcessDetailedError(
                returncode=cp.returncode, cmd=cmd, output=cp.stdout, stderr=cp.stderr
            )
except Exception as ex:
    print(str(ex))
