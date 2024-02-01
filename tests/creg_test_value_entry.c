/*
 * Library value_entry type test program
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

#include "../libcreg/libcreg_value_entry.h"

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

/* Tests the libcreg_value_entry_initialize function
 * Returns 1 if successful or 0 if not
 */
int creg_test_value_entry_initialize(
     void )
{
	libcerror_error_t *error           = NULL;
	libcreg_value_entry_t *value_entry = NULL;
	int result                         = 0;

#if defined( HAVE_CREG_TEST_MEMORY )
	int number_of_malloc_fail_tests    = 1;
	int number_of_memset_fail_tests    = 1;
	int test_number                    = 0;
#endif

	/* Test regular cases
	 */
	result = libcreg_value_entry_initialize(
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "value_entry",
	 value_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_value_entry_free(
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "value_entry",
	 value_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_value_entry_initialize(
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

	value_entry = (libcreg_value_entry_t *) 0x12345678UL;

	result = libcreg_value_entry_initialize(
	          &value_entry,
	          &error );

	value_entry = NULL;

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
		/* Test libcreg_value_entry_initialize with malloc failing
		 */
		creg_test_malloc_attempts_before_fail = test_number;

		result = libcreg_value_entry_initialize(
		          &value_entry,
		          &error );

		if( creg_test_malloc_attempts_before_fail != -1 )
		{
			creg_test_malloc_attempts_before_fail = -1;

			if( value_entry != NULL )
			{
				libcreg_value_entry_free(
				 &value_entry,
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
			 "value_entry",
			 value_entry );

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
		/* Test libcreg_value_entry_initialize with memset failing
		 */
		creg_test_memset_attempts_before_fail = test_number;

		result = libcreg_value_entry_initialize(
		          &value_entry,
		          &error );

		if( creg_test_memset_attempts_before_fail != -1 )
		{
			creg_test_memset_attempts_before_fail = -1;

			if( value_entry != NULL )
			{
				libcreg_value_entry_free(
				 &value_entry,
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
			 "value_entry",
			 value_entry );

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
	if( value_entry != NULL )
	{
		libcreg_value_entry_free(
		 &value_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_value_entry_free function
 * Returns 1 if successful or 0 if not
 */
int creg_test_value_entry_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcreg_value_entry_free(
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

/* Tests the libcreg_value_entry_read_data function
 * Returns 1 if successful or 0 if not
 */
int creg_test_value_entry_read_data(
     void )
{
	libcerror_error_t *error           = NULL;
	libcreg_value_entry_t *value_entry = NULL;
	int result                         = 0;

	/* Initialize test
	 */
	result = libcreg_value_entry_initialize(
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "value_entry",
	 value_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_value_entry_read_data(
	          NULL,
	          NULL,
	          0,
	          0,
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

	result = libcreg_value_entry_read_data(
	          value_entry,
	          NULL,
	          0,
	          0,
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
	result = libcreg_value_entry_free(
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "value_entry",
	 value_entry );

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
	if( value_entry != NULL )
	{
		libcreg_value_entry_free(
		 &value_entry,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_value_entry_get_data_size function
 * Returns 1 if successful or 0 if not
 */
int creg_test_value_entry_get_data_size(
     void )
{
	libcerror_error_t *error           = NULL;
	libcreg_value_entry_t *value_entry = NULL;
	size_t data_size                   = 0;
	int data_size_is_set               = 0;
	int result                         = 0;

	/* Initialize test
	 */
	result = libcreg_value_entry_initialize(
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "value_entry",
	 value_entry );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcreg_value_entry_get_data_size(
	          value_entry,
	          &data_size,
	          &error );

	CREG_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	data_size_is_set = result;

	/* Test error cases
	 */
	result = libcreg_value_entry_get_data_size(
	          NULL,
	          &data_size,
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

	if( data_size_is_set != 0 )
	{
		result = libcreg_value_entry_get_data_size(
		          value_entry,
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
	}
	/* Clean up
	 */
	result = libcreg_value_entry_free(
	          &value_entry,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "value_entry",
	 value_entry );

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
	CREG_TEST_UNREFERENCED_PARAMETER( argc )
	CREG_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

	CREG_TEST_RUN(
	 "libcreg_value_entry_initialize",
	 creg_test_value_entry_initialize );

	CREG_TEST_RUN(
	 "libcreg_value_entry_free",
	 creg_test_value_entry_free );

	CREG_TEST_RUN(
	 "libcreg_value_entry_read_data",
	 creg_test_value_entry_read_data );

	CREG_TEST_RUN(
	 "libcreg_value_entry_get_data_size",
	 creg_test_value_entry_get_data_size );

	/* TODO: add tests for libcreg_value_entry_get_data */

	/* TODO: add tests for libcreg_value_entry_compare_name_with_utf8_string */

	/* TODO: add tests for libcreg_value_entry_compare_name_with_utf16_string */

#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

