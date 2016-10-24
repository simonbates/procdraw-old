FUNCTIONS_SRC_DIR = Documentation/functions
FUNCTION_SCHEMA = $(FUNCTIONS_SRC_DIR)/function-spec.rnc

default: lint

.PHONY: default

format:
	find src -type f \( -name \*.cpp -o -name \*.h \) \
	-exec clang-format -i '-style={BasedOnStyle: Mozilla, SpacesBeforeTrailingComments: 2}' {} +

.PHONY: format

cpplint:
	find src -type f \( -name \*.cpp -o -name \*.h \) \
	-exec third_party/google/styleguide/cpplint/cpplint.py {} +

.PHONY: cpplint

validate-function-specs:
	find $(FUNCTIONS_SRC_DIR) -name \*.xml -not -name schemas.xml \
	-exec jing -c $(FUNCTION_SCHEMA) {} +

.PHONY: validate-function-specs

lint: cpplint validate-function-specs

.PHONY: lint

gl3w:
	mkdir -p third_party/gl3w_out
	cd third_party/gl3w_out && python ../gl3w/gl3w_gen.py

.PHONY: gl3w
