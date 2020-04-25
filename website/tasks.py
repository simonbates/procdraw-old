import http.server
import os
import socketserver

from invoke import Collection, task

from website_builder import WebsiteBuilder


_project_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
_default_output_dir = os.path.join(_project_dir, "website", "public")


@task(help={"output": "Path to write files to"})
def build(_, output=None):
    """
    Build the website
    """

    if output is not None:
        output_dir = output
    else:
        output_dir = _default_output_dir

    builder = WebsiteBuilder(os.path.join(_project_dir, "docs"),
                             os.path.join(_project_dir, "website"),
                             output_dir)
    builder.build()


@task(help={"directory": "Directory to serve files from"})
def server(c, directory=None):
    """
    Run a local dev web server
    """

    if directory is not None:
        server_directory = directory
    else:
        server_directory = _default_output_dir

    address = c.config["procdraw"]["web_server_address"]
    port = c.config["procdraw"]["web_server_port"]

    class BuildDirRequestHandler(http.server.SimpleHTTPRequestHandler):
        def __init__(self, *args, **kwargs):
            super().__init__(*args, directory=server_directory, **kwargs)

    print("Serving files from {:s}".format(server_directory))

    with socketserver.TCPServer((address, port),
                                BuildDirRequestHandler) as httpd:
        print("Web Server is available at http://localhost:{}/".format(port))
        httpd.serve_forever()


ns = Collection(build, server)

ns.configure({
    "procdraw": {
        "web_server_address": "127.0.0.1",
        "web_server_port": 8000
    }
})
