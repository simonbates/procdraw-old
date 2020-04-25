# Contributing to Procdraw

## Development environment automated tasks

### Prerequisites

- Python 3

### Install Python dependencies

    > py -m venv env
    > .\env\Scripts\activate
    > python -m pip install --upgrade pip
    > pip install -r requirements.txt

### Available targets

- `MSBuild -t:Configure -p:VcpkgRoot=[vcpkg root]`
- `MSBuild -t:Compile`
- `MSBuild -t:Test`
- `MSBuild -t:Lint`
- `MSBuild -t:Format`
- `MSBuild -t:GenerateGL`
