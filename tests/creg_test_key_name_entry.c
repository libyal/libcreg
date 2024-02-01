/*
 * Library key_name_entry type test program
 *
 * Copyright (C) 2013-2024, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
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

#include "../libcreg/libcreg_key_name_entry.h"

uint8_t creg_test_key_name_entry_data1[ 28 ] = {
	0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x53, 0x6f, 0x66, 0x74, 0x77, 0x61, 0x72, 0x65 };

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

/* Tests the libcreg_key_name_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_name_entry_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libcreg_key_name_entry_t *key_name_entry = NULL;
	int result                               = 0;

#if defined( HAVE_CREG_TEST_MEMORY )
	int number_of_malloc_fail_tests          = 1;
	int number_of_memset_fail_tests          = 1;
	int test_number                          = 0;
#endif

	/* Test regular cases
	 */
	result = libcreg_key_name_entry_initialize(
	          &key_name_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "key_name_entry",
	 key_name_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_key_name_entry_free(
	          &key_name_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "key_name_entry",
	 key_name_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_key_name_entry_initialize(
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

	key_name_entry = (libcreg_key_name_entry_t *) 0x12345678UL;

	result = libcreg_key_name_entry_initialize(
	          &key_name_entry,
	          &error );

	key_name_entry = NULL;

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

#if defined( HAVE_CREG_TEST_MEMORY )

	for( test_number = 0;
	     test_number < number_of_malloc_fail_tests;
	     test_number++ )
	{
		/* Test libcreg_key_name_entry_initialize with malloc failing
		 */
		creg_test_malloc_attempts_before_fail = test_number;

		result = libcreg_key_name_entry_initialize(
		          &key_name_entry,
		          &error );

		if( creg_test_malloc_attempts_before_fail != -1 )
		{
			creg_test_malloc_attempts_before_fail = -1;

			if( key_name_entry != NULL )
			{
				libcreg_key_name_entry_free(
				 &key_name_entry,
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
			 "key_name_entry",
			 key_name_entry );

			CREG_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
	for( test_number = 0;
	     test_number < number_of_memset_fail_tests;
	     test_number++ )
	{
		/* Test libcreg_key_name_entry_initialize with memset failing
		 */
		creg_test_memset_attempts_before_fail = test_number;

		result = libcreg_key_name_entry_initialize(
		          &key_name_entry,
		          &error );

		if( creg_test_memset_attempts_before_fail != -1 )
		{
			creg_test_memset_attempts_before_fail = -1;

			if( key_name_entry != NULL )
			{
				libcreg_key_name_entry_free(
				 &key_name_entry,
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
			 "key_name_entry",
			 key_name_entry );

			CREG_TEST_ASSERT_IS_NOT_NULL(
			 "error",
			 error );

			libcerror_error_free(
			 &error );
		}
	}
#endif /* defined( HAVE_CREG_TEST_MEMORY ) */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 &key_name_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_key_name_entry_free function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_name_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcreg_key_name_entry_free(
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

/* Tests the libcreg_key_name_entry_read_data function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_name_entry_read_data(
     void )
{
	libcerror_error_t *error                 = NULL;
	libcreg_key_name_entry_t *key_name_entry = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libcreg_key_name_entry_initialize(
	          &key_name_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "key_name_entry",
	 key_name_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcreg_key_name_entry_read_data(
	          key_name_entry,
	          creg_test_key_name_entry_data1,
	          28,
	          LIBCREG_CODEPAGE_WINDOWS_1252,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_key_name_entry_read_data(
	          NULL,
	          creg_test_key_name_entry_data1,
	          28,
	          LIBCREG_CODEPAGE_WINDOWS_1252,
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

	result = libcreg_key_name_entry_read_data(
	          key_name_entry,
	          NULL,
	          28,
	          LIBCREG_CODEPAGE_WINDOWS_1252,
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

	result = libcreg_key_name_entry_read_data(
	          key_name_entry,
	          creg_test_key_name_entry_data1,
	          0,
	          LIBCREG_CODEPAGE_WINDOWS_1252,
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

	/* Clean up
	 */
	result = libcreg_key_name_entry_free(
	          &key_name_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "key_name_entry",
	 key_name_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 &key_name_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_key_name_entry_get_number_of_entries function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_name_entry_get_number_of_entries(
     libcreg_key_name_entry_t *key_name_entry )
{
	libcerror_error_t *error = NULL;
	int number_of_entries    = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libcreg_key_name_entry_get_number_of_entries(
	          key_name_entry,
	          &number_of_entries,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_key_name_entry_get_number_of_entries(
	          NULL,
	          &number_of_entries,
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

	result = libcreg_key_name_entry_get_number_of_entries(
	          key_name_entry,
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

/* Tests the libcreg_key_name_entry_get_entry_by_index function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_name_entry_get_entry_by_index(
     libcreg_key_name_entry_t *key_name_entry )
{
	libcerror_error_t *error           = NULL;
	libcreg_value_entry_t *value_entry = NULL;
	int number_of_entries              = 0;
	int result                         = 0;

	/* Initialize test
	 */
	result = libcreg_key_name_entry_get_number_of_entries(
	          key_name_entry,
	          &number_of_entries,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	if( number_of_entries > 0 )
	{
		result = libcreg_key_name_entry_get_entry_by_index(
		          key_name_entry,
		          0,
		          &value_entry,
		          &error );

		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CREG_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		CREG_TEST_ASSERT_IS_NOT_NULL(
		 "value_entry",
		 value_entry );

		result = libcreg_value_entry_free(
		          &value_entry,
		          &error );

		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CREG_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	/* Test error cases
	 */
	result = libcreg_key_name_entry_get_entry_by_index(
	          NULL,
	          0,
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "value_entry",
	 value_entry );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcreg_key_name_entry_get_entry_by_index(
	          key_name_entry,
	          -1,
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "value_entry",
	 value_entry );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcreg_key_name_entry_get_entry_by_index(
	          key_name_entry,
	          0,
	          NULL,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "value_entry",
	 value_entry );

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
	if( value_entry != NULL )
	{
		libcreg_value_entry_free(
		 &value_entry,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

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
#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	libcerror_error_t *error                 = NULL;
	libcreg_key_name_entry_t *key_name_entry = NULL;
	int result                               = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

	CREG_TEST_UNREFERENCED_PARAMETER( argc )
	CREG_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

	CREG_TEST_RUN(
	 "libcreg_key_name_entry_initialize",
	 creg_test_key_name_entry_initialize );

	CREG_TEST_RUN(
	 "libcreg_key_name_entry_free",
	 creg_test_key_name_entry_free );

	/* TODO: add tests for libcreg_key_name_entry_read_entry_size */

	CREG_TEST_RUN(
	 "libcreg_key_name_entry_read_data",
	 creg_test_key_name_entry_read_data );

	/* TODO: add tests for libcreg_key_name_entry_read_values */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libcreg_key_name_entry_initialize(
	          &key_name_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "key_name_entry",
	 key_name_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_key_name_entry_read_data(
	          key_name_entry,
	          creg_test_key_name_entry_data1,
	          28,
	          LIBCREG_CODEPAGE_WINDOWS_1252,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	CREG_TEST_RUN_WITH_ARGS(
	 "libcreg_key_name_entry_get_number_of_entries",
	 creg_test_key_name_entry_get_number_of_entries,
	 key_name_entry );

	CREG_TEST_RUN_WITH_ARGS(
	 "creg_test_key_name_entry_get_entry_by_index",
	 creg_test_key_name_entry_get_entry_by_index,
	 key_name_entry );

	/* TODO: add tests for libcreg_key_name_entry_get_entry_by_index */

	/* TODO: add tests for libcreg_key_name_entry_compare_name_with_utf8_string */

	/* TODO: add tests for libcreg_key_name_entry_compare_name_with_utf16_string */

	/* Clean up
	 */
	result = libcreg_key_name_entry_free(
	          &key_name_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "key_name_entry",
	 key_name_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )
#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 &key_name_entry,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

