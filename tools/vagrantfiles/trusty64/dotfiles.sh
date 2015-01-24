#!/bin/sh

cat > "$HOME/.bash_profile" << EOF
if [ -f "$HOME/.bashrc" ]; then
    . "$HOME/.bashrc"
fi
unalias ls
EOF
