import os, subprocess

# TODO: Fail on non-zero returncode from subprocess.call()

def find(*args):
    for path in args:
        for (dirpath, dirnames, filenames) in os.walk(path):
            for filename in filenames:
                yield os.path.join(dirpath, filename)

def src_files():
    return (f for f in find('src', 'tests')
            if f.endswith('.cpp') or f.endswith('.h'))

def function_specs():
    return (f for f in find('Documentation/docs/functions')
            if f.endswith('.xml'))

def jing(schema, files):
    args = ['jing', '-c', schema]
    args.extend(files)
    return subprocess.call(args)

for f in src_files():
    print(f)

print(jing('Documentation/docs/functions/function-spec.rnc', function_specs()))
