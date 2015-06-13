#!/bin/sh

astyle --options=extras/astylerc --recursive 'src/*.cpp' 'src/*.h' 'tests/*.cpp' 'tests/*.h'
