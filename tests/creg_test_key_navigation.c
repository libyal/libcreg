/*
 * Library key_navigation type test program
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

#include "../libcreg/libcreg_io_handle.h"
#include "../libcreg/libcreg_key_navigation.h"

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

/* Tests the libcreg_key_navigation_initialize function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_navigation_initialize(
     void )
{
	libcerror_error_t *error                 = NULL;
	libcreg_io_handle_t *io_handle           = NULL;
	libcreg_key_navigation_t *key_navigation = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libcreg_io_handle_initialize(
	          &io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test regular cases
	 */
	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          io_handle,
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
	          io_handle,
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
	          io_handle,
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
	          io_handle,
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
	          io_handle,
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

	/* Clean up
	 */
	result = libcreg_io_handle_free(
	          &io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( key_navigation != NULL )
	{
		libcreg_key_navigation_free(
		 &key_navigation,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libcreg_io_handle_free(
		 &io_handle,
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

/* Tests the libcreg_key_navigation_read_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int creg_test_key_navigation_read_file_io_handle(
     void )
{
	libcerror_error_t *error                 = NULL;
	libcreg_io_handle_t *io_handle           = NULL;
	libcreg_key_navigation_t *key_navigation = NULL;
	int result                               = 0;

	/* Initialize test
	 */
	result = libcreg_io_handle_initialize(
	          &io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          io_handle,
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

	/* Test regular cases
	 */
/* TODO implement */

	/* Test error cases
	 */
	result = libcreg_key_navigation_read_file_io_handle(
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

	result = libcreg_key_navigation_read_file_io_handle(
	          key_navigation,
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

	/* Clean up
	 */
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

	result = libcreg_io_handle_free(
	          &io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

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
	if( key_navigation != NULL )
	{
		libcreg_key_navigation_free(
		 &key_navigation,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libcreg_io_handle_free(
		 &io_handle,
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
	libcreg_io_handle_t *io_handle           = NULL;
	libcreg_key_navigation_t *key_navigation = NULL;
	int result                               = 0;

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

	CREG_TEST_UNREFERENCED_PARAMETER( argc )
	CREG_TEST_UNREFERENCED_PARAMETER( argv )

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

	CREG_TEST_RUN(
	 "libcreg_key_navigation_initialize",
	 creg_test_key_navigation_initialize );

	CREG_TEST_RUN(
	 "libcreg_key_navigation_free",
	 creg_test_key_navigation_free );

	CREG_TEST_RUN(
	 "libcreg_key_navigation_read_file_io_handle",
	 creg_test_key_navigation_read_file_io_handle );

	/* TODO: add tests for libcreg_key_navigation_read_data_blocks */

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )

	/* Initialize test
	 */
	result = libcreg_io_handle_initialize(
	          &io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "io_handle",
	 io_handle );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_key_navigation_initialize(
	          &key_navigation,
	          io_handle,
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

/* TODO read key navigation data */

	/* TODO: add tests for libcreg_key_navigation_get_key_hierarchy_entry_at_offset */

	/* TODO: add tests for libcreg_key_navigation_get_number_of_data_blocks */

	/* TODO: add tests for libcreg_key_navigation_get_data_block_at_index */

	/* Clean up
	 */
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

	result = libcreg_io_handle_free(
	          &io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "io_handle",
	 io_handle );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	/* TODO: add tests for libcreg_key_navigation_read_data_block_element_data */

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
	if( key_navigation != NULL )
	{
		libcreg_key_navigation_free(
		 &key_navigation,
		 NULL );
	}
	if( io_handle != NULL )
	{
		libcreg_io_handle_free(
		 &io_handle,
		 NULL );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */
#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

	return( EXIT_FAILURE );
}

