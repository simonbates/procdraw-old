#!/bin/sh

astyle --options=scripts/astylerc --recursive 'include/*.h' 'src/*.cpp' 'tests/*.cpp' 'tests/*.h'
