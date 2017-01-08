WEBSITE_OUT_DIR = out

XML_SRC_FILES = \
	$(shell find docs -type f -name '*.xml') \
	index.xml
XML_OUT_FILES = $(patsubst %.xml, $(WEBSITE_OUT_DIR)/%.html, $(XML_SRC_FILES))

NODE_SASS = ./node_modules/.bin/node-sass
POSTCSS = ./node_modules/.bin/postcss
FOUNDATION_SCSS_DIR = ./node_modules/foundation-sites/scss

website: $(XML_OUT_FILES) $(WEBSITE_OUT_DIR)/css/main.css

.PHONY: website

$(WEBSITE_OUT_DIR)/docs/%.html: docs/%.xml procdraw-website.xsl
	xsltproc --output $@ procdraw-website.xsl $<

$(WEBSITE_OUT_DIR)/docs/functions/%.html: docs/functions/%.xml procdraw-website.xsl
	xsltproc --output $@ procdraw-website.xsl $<

$(WEBSITE_OUT_DIR)/%.html: %.xml procdraw-website.xsl
	xsltproc --output $@ procdraw-website.xsl $<

$(WEBSITE_OUT_DIR)/css/main.css: scss/main.scss scss/_settings.scss
	mkdir -p $(WEBSITE_OUT_DIR)/css
	$(NODE_SASS) --include-path $(FOUNDATION_SCSS_DIR) scss/main.scss \
	| $(POSTCSS) --use autoprefixer \
	> $(WEBSITE_OUT_DIR)/css/main.css

clean:
	rm -rf $(WEBSITE_OUT_DIR)

.PHONY: clean
