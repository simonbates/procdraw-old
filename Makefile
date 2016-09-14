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
	find include src tests -type f \( -name \*.cpp -o -name \*.h \) \
	-exec third_party/google/styleguide/cpplint/cpplint.py {} +

.PHONY: format
format:
	find include src tests -type f \( -name \*.cpp -o -name \*.h \) \
	-exec clang-format -i -style=Mozilla {} +

.PHONY: gl3w
gl3w:
	mkdir -p third_party/gl3w_out
	cd third_party/gl3w_out && python ../gl3w/gl3w_gen.py
