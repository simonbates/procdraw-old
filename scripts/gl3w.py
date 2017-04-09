import os, procdraw

gl3w_out_dir = os.path.join('third_party', 'gl3w_out')

if not os.path.exists(gl3w_out_dir):
    os.makedirs(gl3w_out_dir)

os.chdir(gl3w_out_dir)

procdraw.exec_command(['python', os.path.join('..', 'gl3w', 'gl3w_gen.py')])
