REFERENCE_SRC_DIR = ../Documentation/reference
REFERENCE_OUT_DIR = _reference_generated

REFERENCE_SRC_FILES = $(filter-out $(REFERENCE_SRC_DIR)/schemas.xml, $(wildcard $(REFERENCE_SRC_DIR)/*.xml))
REFERENCE_OUT_FILES = $(patsubst $(REFERENCE_SRC_DIR)/%.xml, $(REFERENCE_OUT_DIR)/%.html, $(REFERENCE_SRC_FILES))

.PHONY: all
all: content css/main.css ;

.PHONY: content
content: $(REFERENCE_OUT_FILES) ;

$(REFERENCE_OUT_DIR)/%.html: $(REFERENCE_SRC_DIR)/%.xml _xslt/functionspec2html.xsl
	xsltproc --output $@ _xslt/functionspec2html.xsl $<

css/main.css: _scss/main.scss _scss/_settings.scss
	gulp

.PHONY: clean
clean:
	rm -rf $(REFERENCE_OUT_DIR)
	rm -f css/main.css
