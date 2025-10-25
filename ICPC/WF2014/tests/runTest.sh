#!/bin/bash

if [ ${1} == "remove" ]; then
    rm -f tmp_prog1 tmp_prog2 tmp_output1 tmp_output2;
    exit;
fi;

FLAGS="-O2"

if [ ${1} == "compile" ]; then
    g++ code/test_${2}_old.cc ${FLAGS} -o tmp_prog2
    if g++ code/test_${2}.cc ${FLAGS} -o tmp_prog1; then
	echo "OK.";
    else
	echo "*** FAIL! ***";
    fi;
    exit;
fi;

./tmp_prog1 < data/${1} >| tmp_output1
./tmp_prog2 < data/${1} >| tmp_output2

# /usr/bin/time -f "\nreal %E user %U sys %S" ./tmp_prog1 < data/${1} >| tmp_output1
# /usr/bin/time -f "real %E user %U sys %S" ./tmp_prog2 < data/${1} >| tmp_output2

DIFF=$(diff tmp_output1 tmp_output2)
if [ "$DIFF" != "" ]; then
    echo "*** FAIL! ***";
else
    echo "OK.";
fi;
