import http.server
import os
import socketserver

from invoke import task

from .website_builder import WebsiteBuilder


_project_dir = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
_build_dir = os.path.join(_project_dir, "website", "build")


class BuildDirRequestHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=_build_dir, **kwargs)


@task
def build(_):
    """
    Build the website
    """
    builder = WebsiteBuilder(os.path.join(_project_dir, "docs"),
                             os.path.join(_project_dir, "website"),
                             _build_dir)
    builder.build()


@task
def server(c):
    """
    Run a local dev web server
    """
    address = c.config["procdraw"]["web_server_address"]
    port = c.config["procdraw"]["web_server_port"]
    with socketserver.TCPServer((address, port), BuildDirRequestHandler) as httpd:
        print("Web Server is available at http://localhost:{}/".format(port))
        httpd.serve_forever()
