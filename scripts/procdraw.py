import itertools, os, subprocess

functions_src_dir = os.path.join('Documentation', 'docs', 'functions')
function_schema = os.path.join(functions_src_dir, 'function-spec.rng')

cpplint_prog = os.path.join('third_party', 'google', 'styleguide',
                            'cpplint', 'cpplint.py')

def find(*args):
    for path in args:
        for (dirpath, dirnames, filenames) in os.walk(path):
            for filename in filenames:
                yield os.path.join(dirpath, filename)

def exec_command(args):
    returncode = subprocess.call(args)
    if returncode != 0:
        raise Exception('Error {:d}'.format(returncode))

def is_cpp_file(filename):
    return filename.endswith('.cpp') or filename.endswith('.h')

def src_files():
    return (f for f in find('src', 'tests') if is_cpp_file(f))

def function_specs():
    return (f for f in find(functions_src_dir) if f.endswith('.xml'))

def cpplint(files):
    exec_command(itertools.chain(['python', cpplint_prog], files))

def jing(schema, files):
    exec_command(itertools.chain(['jing', schema], files))

def clang_format(files):
    exec_command(itertools.chain(['clang-format', '-i', '-style=file'], files))
