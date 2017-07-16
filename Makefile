FUNCTIONS_SRC_DIR = Documentation/docs/functions
FUNCTION_SCHEMA = $(FUNCTIONS_SRC_DIR)/function-spec.rng

# Override with: make format CLANG_FORMAT=<new-name>
CLANG_FORMAT = clang-format

.PHONY: default
default: lint

.PHONY: cpplint
cpplint:
	find src tests -type f \( -name \*.cpp -o -name \*.h \) \
	-exec third_party/google-styleguide/cpplint/cpplint.py {} +

.PHONY: validate-function-specs
validate-function-specs:
	find $(FUNCTIONS_SRC_DIR) -name \*.xml \
	-exec xmllint --noout --relaxng $(FUNCTION_SCHEMA) {} +

.PHONY: lint
lint: cpplint validate-function-specs

.PHONY: format
format:
	find src tests -type f \( -name \*.cpp -o -name \*.h \) \
	-exec $(CLANG_FORMAT) -i -style=file {} +

.PHONY: gl3w
gl3w:
	mkdir -p third_party/gl3w_out
	cd third_party/gl3w_out && python ../gl3w/gl3w_gen.py
