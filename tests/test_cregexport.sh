#!/bin/bash
#
# cregexport tool testing script
#
# Copyright (C) 2013-2016, Joachim Metz <joachim.metz@gmail.com>
#
# Refer to AUTHORS for acknowledgements.
#
# This software is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this software.  If not, see <http://www.gnu.org/licenses/>.
#

EXIT_SUCCESS=0;
EXIT_FAILURE=1;
EXIT_IGNORE=77;

test_export()
{ 
	DIRNAME=$1;
	INPUT_FILE=$2;
	BASENAME=`basename ${INPUT_FILE}`;

	if [ -d tmp ];
	then
		rm -rf tmp;
	fi
	mkdir tmp;

	${CREGEXPORT} ${INPUT_FILE} | sed '1,2d' > tmp/${BASENAME}.log;

	RESULT=$?;

	if [ -f "input/.cregexport/${DIRNAME}/${BASENAME}.log.gz" ];
	then
		zdiff "input/.cregexport/${DIRNAME}/${BASENAME}.log.gz" "tmp/${BASENAME}.log";

		RESULT=$?;
	else
		mv "tmp/${BASENAME}.log" "input/.cregexport/${DIRNAME}";

		gzip "input/.cregexport/${DIRNAME}/${BASENAME}.log";
	fi

	rm -rf tmp;

	echo -n "Testing cregexport of input: ${INPUT_FILE} ";

	if test ${RESULT} -ne ${EXIT_SUCCESS};
	then
		echo " (FAIL)";
	else
		echo " (PASS)";
	fi
	return ${RESULT};
}

CREGEXPORT="../cregtools/cregexport";

if ! test -x ${CREGEXPORT};
then
	CREGEXPORT="../cregtools/cregexport.exe";
fi

if ! test -x ${CREGEXPORT};
then
	echo "Missing executable: ${CREGEXPORT}";

	exit ${EXIT_FAILURE};
fi

if ! test -d "input";
then
	echo "No input directory found.";

	exit ${EXIT_IGNORE};
fi

OLDIFS=${IFS};
IFS="
";

RESULT=`ls input/* | tr ' ' '\n' | wc -l`;

if test ${RESULT} -eq 0;
then
	echo "No files or directories found in the input directory.";

	EXIT_RESULT=${EXIT_IGNORE};
else
	if ! test -d "input/.cregexport";
	then
		mkdir "input/.cregexport";
	fi

	for TESTDIR in input/*;
	do
		if [ -d "${TESTDIR}" ];
		then
			DIRNAME=`basename ${TESTDIR}`;

			if [ ! -d "input/.cregexport/${DIRNAME}" ];
			then
				mkdir "input/.cregexport/${DIRNAME}";
			fi
			for TESTFILE in ${TESTDIR}/*;
			do
				if ! test_export "${DIRNAME}" "${TESTFILE}";
				then
					exit ${EXIT_FAILURE};
				fi
			done
		fi
	done

	EXIT_RESULT=${EXIT_SUCCESS};
fi

IFS=${OLDIFS};

exit ${EXIT_RESULT};

