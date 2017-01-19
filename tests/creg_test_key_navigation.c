/*
 * Library key_navigation type test program
 *
 * Copyright (C) 2013-2017, Joachim Metz <joachim.metz@gmail.com>
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
#include "creg_test_memory.h"
#include "creg_test_unused.h"

#include "../libcreg/libcreg_key_navigation.h"

#if defined( __GNUC__ )

/* Tests the libcreg_key_navigation_initialize function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_navigation_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libcreg_key_navigation_t *key_navigation = NULL;
	int result                               = 0;

	/* Test key_navigation initialization
	 */
	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          NULL,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CREG_TEST_ASSERT_IS_NOT_NULL(
         "key_navigation",
         key_navigation );

        CREG_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libcreg_key_navigation_free(
	          &key_navigation,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CREG_TEST_ASSERT_IS_NULL(
         "key_navigation",
         key_navigation );

        CREG_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test error cases
	 */
	result = libcreg_key_navigation_initialize(
	          NULL,
	          NULL,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CREG_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	key_navigation = (libcreg_key_navigation_t *) 0x12345678UL;

	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          NULL,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CREG_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	key_navigation = NULL;

#if defined( HAVE_CREG_TEST_MEMORY )

	/* Test libcreg_key_navigation_initialize with malloc failing
	 */
	creg_test_malloc_attempts_before_fail = 0;

	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          NULL,
	          &error );

	if( creg_test_malloc_attempts_before_fail != -1 )
	{
		creg_test_malloc_attempts_before_fail = -1;

		if( key_navigation != NULL )
		{
			libcreg_key_navigation_free(
			 &key_navigation,
			 NULL );
		}
	}
	else
	{
		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CREG_TEST_ASSERT_IS_NULL(
		 "key_navigation",
		 key_navigation );

		CREG_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
	/* Test libcreg_key_navigation_initialize with memset failing
	 */
	creg_test_memset_attempts_before_fail = 0;

	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          NULL,
	          &error );

	if( creg_test_memset_attempts_before_fail != -1 )
	{
		creg_test_memset_attempts_before_fail = -1;

		if( key_navigation != NULL )
		{
			libcreg_key_navigation_free(
			 &key_navigation,
			 NULL );
		}
	}
	else
	{
		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CREG_TEST_ASSERT_IS_NULL(
		 "key_navigation",
		 key_navigation );

		CREG_TEST_ASSERT_IS_NOT_NULL(
		 "error",
		 error );

		libcerror_error_free(
		 &error );
	}
#endif /* defined( HAVE_CREG_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( key_navigation != NULL )
	{
		libcreg_key_navigation_free(
		 &key_navigation,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_key_navigation_free function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_navigation_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcreg_key_navigation_free(
	          NULL,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

        CREG_TEST_ASSERT_IS_NOT_NULL(
         "error",
         error );

	libcerror_error_free(
	 &error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) */

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

#if defined( __GNUC__ )

	CREG_TEST_RUN(
	 "libcreg_key_navigation_initialize",
	 creg_test_key_navigation_initialize );

	CREG_TEST_RUN(
	 "libcreg_key_navigation_free",
	 creg_test_key_navigation_free );

#endif /* defined( __GNUC__ ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

