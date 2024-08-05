#!/bin/sh

# -----------------------------------------------------------------------------
# The ways that work.

## If the key-value pair is not enclosed in quotation marks, then there must
## not be any white space after `,`.
ansible-playbook -v -e str_list='["abc","def"]' ./cli-extra-vars-list.yml
ansible-playbook -v -e str_list="['abc','def']" ./cli-extra-vars-list.yml

## If the key-value pair is enclosed in quotation marks, then the inner
## quotation marks must be escaped appropriately.
ansible-playbook -v -e "str_list='[\"abc\",\"def\"]'" ./cli-extra-vars-list.yml
ansible-playbook -v -e "str_list='[\"abc\", \"def\"]'" ./cli-extra-vars-list.yml

# -----------------------------------------------------------------------------
# The ways that don't work.

## `[abc,def]` is treated as a string.
ansible-playbook -v -e str_list=[abc,def] ./cli-extra-vars-list.yml
ansible-playbook -v -e str_list='[abc,def]' ./cli-extra-vars-list.yml

## The white space after `,` makes `[abc, def]` be broken into two strings:
## "[abc," and "def]".
ansible-playbook -v -e str_list='[abc, def]' ./cli-extra-vars-list.yml
ansible-playbook -v -e str_list="[abc, def]" ./cli-extra-vars-list.yml
ansible-playbook -v -e str_list='["abc", "def"]' ./cli-extra-vars-list.yml
ansible-playbook -v -e str_list="['abc', 'def']" ./cli-extra-vars-list.yml
