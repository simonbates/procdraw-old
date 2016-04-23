FUNCTIONS_SRC_DIR = Documentation/functions
FUNCTIONS_SRC_FILES = $(filter-out $(FUNCTIONS_SRC_DIR)/schemas.xml, $(wildcard $(FUNCTIONS_SRC_DIR)/*.xml))
FUNCTION_SCHEMA = $(FUNCTIONS_SRC_DIR)/function-spec.rnc

.PHONY: gtags
gtags:
	gtags

.PHONY: check-fns
check-fns: $(FUNCTIONS_SRC_FILES) ;

.PHONY: $(FUNCTIONS_SRC_FILES)
$(FUNCTIONS_SRC_FILES):
	jing -c $(FUNCTION_SCHEMA) $@
