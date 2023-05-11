#!/bin/sh

# We don't want `vendor` because its query fails on a TPM simulator.
CAPS=$(tpm2_getcap -l | cut -d " " -f 2 | sort -u | grep -v vendor)

for CAP in $CAPS;
do
  echo "=============================="
  echo "Capability: $CAP"
  tpm2_getcap "$CAP"
done
