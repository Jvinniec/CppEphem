#! /usr/bin/env

if [ "x$CC" == "xgcc" ] ; then
    gcov -o test/CMakeFiles/test_cppephem.dir/src/ ./test_cppephem.cpp
elif [ "x$CC" == "xclang" ] ; then
    llvm-cov gcov -o test/CMakeFiles/test_cppephem.dir/src/ ./test_cppephem.cpp
else
    echo "\n\nNO APPROPRIATE CODE COVERAGE TOOL FOUND\n\n"
fi
