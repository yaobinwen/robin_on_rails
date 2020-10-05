#!/bin/sh

vagrant up

vagrant provision --provision-with "reproduce"
