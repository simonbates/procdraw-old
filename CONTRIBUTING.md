# Contributing to Procdraw

## Development environment automated tasks

### Prerequisites

- Python 3

### Install dependencies

    > py -m venv env
    > .\env\Scripts\activate
    > python -m pip install --upgrade pip
    > pip install -r requirements.txt

### Available tasks

You can list the available tasks with `invoke --list`:

    > invoke --list
    Available tasks:

      check-file-headers   Check the C++ source file headers
      format-cpp           Format the C++ source files with clang-format
      validate-xml         Validate the docs XML
      website.build        Build the website
      website.server       Run a local dev web server

Check the C++ source file headers:

    > invoke check-file-headers

Format the C++ source files with clang-format:

    > invoke format-cpp

Validate the docs XML:

    > invoke validate-xml

Build the website:

    > invoke website.build

Run a local dev web server:

    > invoke website.server
