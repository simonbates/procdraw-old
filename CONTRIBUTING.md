# Contributing to Procdraw

## Development environment automated tasks

### Prerequisites

- Python 3

### Install Python dependencies

    > py -m venv env
    > .\env\Scripts\activate
    > python -m pip install --upgrade pip
    > pip install -r requirements.txt

### Available tasks

    > invoke --list

    Available tasks:

      check-cpp-files          Check the C++ source files
      check-python-codestyle   Run pycodestyle on Python files
      format-cpp               Format the C++ source files with clang-format
      generate-gl              Run glad to generate the OpenGL loader
      validate-xml             Validate the docs XML
      website.build            Build the website
      website.server           Run a local dev web server
