import itertools, os, subprocess, sys

def find(*args):
    for path in args:
        for (dirpath, dirnames, filenames) in os.walk(path):
            for filename in filenames:
                yield os.path.join(dirpath, filename)

def exec_command(args):
    returncode = subprocess.call(args)
    if returncode != 0:
        sys.exit(returncode)

def src_files():
    return (f for f in find('src', 'tests')
            if f.endswith('.cpp') or f.endswith('.h'))

def function_specs():
    return (f for f in find('Documentation/docs/functions')
            if f.endswith('.xml'))

cpplint = 'third_party/google/styleguide/cpplint/cpplint.py'
exec_command(itertools.chain(['python', cpplint], src_files()))

schema = 'Documentation/docs/functions/function-spec.rnc'
exec_command(itertools.chain(['jing', '-c', schema], function_specs()))
