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

- `MSBuild -t:Format`
- `MSBuild -t:GenerateGL`
- `MSBuild -t:Test`
