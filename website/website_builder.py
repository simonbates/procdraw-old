import os
import shutil

from lxml import etree
from mako.lookup import TemplateLookup

import procdraw_utils as utils


class WebsiteBuilder:
    def __init__(self, docs_dir, website_dir, build_dir):
        self.website_dir = website_dir
        self.build_dir = build_dir
        self.docs_doc = etree.parse(os.path.join(docs_dir, "docs.xml"))
        templates_dir = os.path.join(website_dir, "templates")
        self.lookup = TemplateLookup(directories=[templates_dir])

    def build(self):
        utils.mkdir_p(self.build_dir)
        self.copy_static()
        self.create_pages()

    def copy_static(self):
        print("Copying static files...", end="", flush=True)

        shutil.copy(os.path.join(self.website_dir,
                                 "static",
                                 "site.css"),
                    self.build_dir)

        print("done", flush=True)

    def create_pages(self):
        print("Creating pages...", end="", flush=True)

        template = self.lookup.get_template("docs.html")
        with open(os.path.join(self.build_dir, "index.html"), "w") as file:
            file.write(template.render(docs=self.docs_doc))

        print("done", flush=True)
