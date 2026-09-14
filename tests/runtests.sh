#!/bin/sh
# Script to run tests
#
# Version: 20260714

if [ -f "${PWD}/libcreg/.libs/libcreg.1.dylib" ] && [ -f ./pycreg/.libs/pycreg.so ]
then
    install_name_tool -change /usr/local/lib/libcreg.1.dylib "${PWD}/libcreg/.libs/libcreg.1.dylib" ./pycreg/.libs/pycreg.so
fi

make check-build > /dev/null

# shellcheck disable=SC2068
make check $@
RESULT=$?

if [ ${RESULT} -ne 0 ]
then
    find . -name \*.log -path \*.dir/\*/\*.log -print -exec cat {} \;
fi
exit ${RESULT}

