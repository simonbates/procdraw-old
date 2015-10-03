#!/bin/sh

astyle --options=extras/astylerc '*.cc' '*.h' 'tests/unit_tests/*.cc' 'tests/function_spec_tests/*.cc'
