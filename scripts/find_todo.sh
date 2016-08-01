#!/bin/sh

git grep --untracked --line-number --ignore-case todo -- . ':!/third_party'
