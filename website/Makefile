NODE_SASS = ./node_modules/.bin/node-sass
POSTCSS = ./node_modules/.bin/postcss
FOUNDATION_SCSS_DIR = ./bower_components/foundation-sites/scss

FUNCTIONS_SRC_DIR = ../Documentation/functions
FUNCTIONS_OUT_DIR = _functions_generated

FUNCTIONS_SRC_FILES = $(filter-out $(FUNCTIONS_SRC_DIR)/schemas.xml, $(wildcard $(FUNCTIONS_SRC_DIR)/*.xml))
FUNCTIONS_OUT_FILES = $(patsubst $(FUNCTIONS_SRC_DIR)/%.xml, $(FUNCTIONS_OUT_DIR)/%.html, $(FUNCTIONS_SRC_FILES))

site: $(FUNCTIONS_OUT_FILES) css/main.css
	jekyll build

.PHONY: site

$(FUNCTIONS_OUT_DIR)/%.html: $(FUNCTIONS_SRC_DIR)/%.xml _xslt/functionspec2html.xsl
	xsltproc --output $@ _xslt/functionspec2html.xsl $<

css/main.css: _scss/main.scss _scss/_settings.scss
	mkdir -p css
	$(NODE_SASS) --include-path $(FOUNDATION_SCSS_DIR) _scss/main.scss \
	| $(POSTCSS) --use autoprefixer \
	> css/main.css

clean:
	rm -rf $(FUNCTIONS_OUT_DIR)
	rm -f css/main.css
	jekyll clean

.PHONY: clean
