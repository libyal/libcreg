#!/bin/sh
# Script to run tests
#
# Version: 20260609

if test -f ${PWD}/libcreg/.libs/libcreg.1.dylib && test -f ./pycreg/.libs/pycreg.so
then
	install_name_tool -change /usr/local/lib/libcreg.1.dylib ${PWD}/libcreg/.libs/libcreg.1.dylib ./pycreg/.libs/pycreg.so
fi

make check-build > /dev/null

make check $@
RESULT=$?

if test ${RESULT} -ne 0
then
	find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

