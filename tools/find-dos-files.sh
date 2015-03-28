#!/bin/sh

find . -name .git -prune -o -type f -print0 | xargs -0 grep -l $(printf '\r')
