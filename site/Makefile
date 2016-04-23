FUNCTIONS_SRC_DIR = ../Documentation/functions
FUNCTIONS_OUT_DIR = _functions_generated

FUNCTIONS_SRC_FILES = $(filter-out $(FUNCTIONS_SRC_DIR)/schemas.xml, $(wildcard $(FUNCTIONS_SRC_DIR)/*.xml))
FUNCTIONS_OUT_FILES = $(patsubst $(FUNCTIONS_SRC_DIR)/%.xml, $(FUNCTIONS_OUT_DIR)/%.html, $(FUNCTIONS_SRC_FILES))

.PHONY: all
all: content css/main.css ;

.PHONY: content
content: $(FUNCTIONS_OUT_FILES) ;

$(FUNCTIONS_OUT_DIR)/%.html: $(FUNCTIONS_SRC_DIR)/%.xml _xslt/functionspec2html.xsl
	xsltproc --output $@ _xslt/functionspec2html.xsl $<

css/main.css: _scss/main.scss _scss/_settings.scss
	gulp

.PHONY: clean
clean:
	rm -rf $(FUNCTIONS_OUT_DIR)
	rm -f css/main.css
