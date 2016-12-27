#!/bin/sh

cat > "$HOME/.bash_aliases" << "EOF"
unalias ls
EOF

cat > "$HOME/make_test_procdraw.sh" << "EOF"
#!/bin/sh
cd $HOME && rm -rf procdraw_build && mkdir procdraw_build
cd procdraw_build && cmake /procdraw && make && make check
EOF

chmod u+x "$HOME/make_test_procdraw.sh"
