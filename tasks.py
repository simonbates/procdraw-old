import itertools
import os
import sys

from invoke import Collection, task

import procdraw_utils as utils
import website


_project_dir = os.path.abspath(os.path.dirname(__file__))


@task
def check_cpp_files(_):
    """
    Check the C++ source files
    """
    src_dir = os.path.relpath(os.path.join(_project_dir, "src"))
    files = utils.find_cpp_files([src_dir])
    reporter = utils.CheckResultTapReporter(42)
    checker = utils.SourceFileChecker()
    for file in files:
        with open(file) as file_in:
            reporter.add(checker.check(file, file_in, "//"))
    reporter.print_summary()
    if reporter.is_fail():
        sys.exit(1)


@task
def check_python_codestyle(_):
    """
    Run pycodestyle on Python files
    """
    files = []
    files += [os.path.relpath(os.path.join(_project_dir, f))
              for f in os.listdir(_project_dir) if utils.is_python_file(f)]
    website_dir = os.path.relpath(os.path.join(_project_dir, "website"))
    files += utils.find_python_files([website_dir])
    utils.run(itertools.chain(["pycodestyle"], files))


@task
def format_cpp(c):
    """
    Format the C++ source files with clang-format
    """
    clang_format = c.config["procdraw"]["clang_format_name"]
    src_dir = os.path.relpath(os.path.join(_project_dir, "src"))
    files = utils.find_cpp_files([src_dir])
    utils.run(itertools.chain([clang_format, "-i", "-style=file"], files))


@task
def generate_gl(_):
    """
    Run glad to generate the OpenGL loader
    """
    output_dir = os.path.join(_project_dir, "GL")
    utils.run(["glad",
               "--generator=c",
               "--no-loader",
               "--out-path={:s}".format(output_dir)])
    with open(os.path.join(output_dir, "README.txt"), "w") as file:
        file.write("The contents of this directory are generated " +
                   "by 'invoke generate-gl'.\n")


@task
def validate_xml(_):
    """
    Validate the docs XML
    """
    reporter = utils.CheckResultTapReporter(1)
    reporter.add(
        utils.validate_xml(os.path.join(_project_dir, "docs", "docs.rng"),
                           os.path.join(_project_dir, "docs", "docs.xml")))
    reporter.print_summary()
    if reporter.is_fail():
        sys.exit(1)


ns = Collection(check_cpp_files, check_python_codestyle, format_cpp,
                generate_gl, validate_xml, website)

ns.configure({
    "procdraw": {
        "clang_format_name": "clang-format",
        "web_server_address": "127.0.0.1",
        "web_server_port": 8000
    }
})
