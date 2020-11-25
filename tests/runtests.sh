#!/bin/sh
# Script to run tests
#
# Version: 20201121

if test -f ${PWD}/libcreg/.libs/libcreg.1.dylib && test -f ./pycreg/.libs/pycreg.so;
then
	install_name_tool -change /usr/local/lib/libcreg.1.dylib ${PWD}/libcreg/.libs/libcreg.1.dylib ./pycreg/.libs/pycreg.so;
fi

make check CHECK_WITH_STDERR=1;
RESULT=$?;

if test ${RESULT} -ne 0 && test -f tests/test-suite.log;
then
	cat tests/test-suite.log;
fi
exit ${RESULT};

