#!/bin/sh

cat > "$HOME/.bash_aliases" << "EOF"
unalias ls
EOF

cat > "$HOME/make-test-procdraw.sh" << "EOF"
#!/bin/sh
cd $HOME
rm -rf procdraw-build
mkdir procdraw-build
cd procdraw-build
cmake /procdraw
make
make test
EOF

chmod u+x "$HOME/make-test-procdraw.sh"
