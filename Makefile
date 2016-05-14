FUNCTIONS_SRC_DIR = Documentation/functions
FUNCTION_SCHEMA = $(FUNCTIONS_SRC_DIR)/function-spec.rnc

.PHONY: gtags
gtags:
	gtags

.PHONY: check-fns
check-fns:
	find $(FUNCTIONS_SRC_DIR) -name \*.xml -not -name schemas.xml \
	-exec jing -c $(FUNCTION_SCHEMA) {} +

.PHONY: cpplint
cpplint:
	find src tests -type f -not -name CMakeLists.txt -not -name \*.xml \
	-exec third_party/google/styleguide/cpplint/cpplint.py {} +
