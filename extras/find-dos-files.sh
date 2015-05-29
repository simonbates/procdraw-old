#!/bin/sh

find . -name .git -prune \
    -o -name build -prune \
    -o -type f -print0 \
    | xargs -0 grep -l $(printf '\r')
