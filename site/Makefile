REFERENCE_SRC_DIR = ../Documentation/reference
REFERENCE_OUT_DIR = _reference

REFERENCE_SRC_FILES = $(filter-out $(REFERENCE_SRC_DIR)/schemas.xml, $(wildcard $(REFERENCE_SRC_DIR)/*.xml))
REFERENCE_OUT_FILES = $(patsubst $(REFERENCE_SRC_DIR)/%.xml, $(REFERENCE_OUT_DIR)/%.html, $(REFERENCE_SRC_FILES))

.PHONY: content
content: $(REFERENCE_OUT_FILES) ;

$(REFERENCE_OUT_DIR)/%.html: $(REFERENCE_SRC_DIR)/%.xml _xslt/reference2html.xsl
	xsltproc --output $@ _xslt/reference2html.xsl $<

.PHONY: clean
clean:
	rm -rf $(REFERENCE_OUT_DIR)
