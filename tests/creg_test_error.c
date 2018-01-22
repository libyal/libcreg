/*
 * Library error functions test program
 *
 * Copyright (C) 2013-2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "creg_test_libcerror.h"
#include "creg_test_libcreg.h"
#include "creg_test_macros.h"
#include "creg_test_unused.h"

/* Tests the libcreg_error_free function
 * Returns 1 if successful or 0 if not
 */
int creg_test_error_free(
     void )
{
	/* Test invocation of function only
	 */
	libcreg_error_free(
	 NULL );

	return( 1 );
}

/* Tests the libcreg_error_fprint function
 * Returns 1 if successful or 0 if not
 */
int creg_test_error_fprint(
     void )
{
	/* Test invocation of function only
	 */
	libcreg_error_fprint(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libcreg_error_sprint function
 * Returns 1 if successful or 0 if not
 */
int creg_test_error_sprint(
     void )
{
	/* Test invocation of function only
	 */
	libcreg_error_sprint(
	 NULL,
	 NULL,
	 0 );

	return( 1 );
}

/* Tests the libcreg_error_backtrace_fprint function
 * Returns 1 if successful or 0 if not
 */
int creg_test_error_backtrace_fprint(
     void )
{
	/* Test invocation of function only
	 */
	libcreg_error_backtrace_fprint(
	 NULL,
	 NULL );

	return( 1 );
}

/* Tests the libcreg_error_backtrace_sprint function
 * Returns 1 if successful or 0 if not
 */
int creg_test_error_backtrace_sprint(
     void )
{
	/* Test invocation of function only
	 */
	libcreg_error_backtrace_sprint(
	 NULL,
	 NULL,
	 0 );

	return( 1 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc CREG_TEST_ATTRIBUTE_UNUSED,
     wchar_t * const argv[] CREG_TEST_ATTRIBUTE_UNUSED )
#else
int main(
     int argc CREG_TEST_ATTRIBUTE_UNUSED,
     char * const argv[] CREG_TEST_ATTRIBUTE_UNUSED )
#endif
{
	CREG_TEST_UNREFERENCED_PARAMETER( argc )
	CREG_TEST_UNREFERENCED_PARAMETER( argv )

	CREG_TEST_RUN(
	 "libcreg_error_free",
	 creg_test_error_free );

	CREG_TEST_RUN(
	 "libcreg_error_fprint",
	 creg_test_error_fprint );

	CREG_TEST_RUN(
	 "libcreg_error_sprint",
	 creg_test_error_sprint );

	CREG_TEST_RUN(
	 "libcreg_error_backtrace_fprint",
	 creg_test_error_backtrace_fprint );

	CREG_TEST_RUN(
	 "libcreg_error_backtrace_sprint",
	 creg_test_error_backtrace_sprint );

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

