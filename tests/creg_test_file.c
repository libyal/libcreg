/*
 * Library file type test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "creg_test_functions.h"
#include "creg_test_getopt.h"
#include "creg_test_libbfio.h"
#include "creg_test_libcerror.h"
#include "creg_test_libcreg.h"
#include "creg_test_macros.h"
#include "creg_test_memory.h"

#include "../libcreg/libcreg_file.h"

#if defined( HAVE_WIDE_SYSTEM_CHARACTER ) && SIZEOF_WCHAR_T != 2 && SIZEOF_WCHAR_T != 4
#error Unsupported size of wchar_t
#endif

/* Define to make creg_test_file generate verbose output
#define CREG_TEST_FILE_VERBOSE
 */

#if !defined( LIBCREG_HAVE_BFIO )

LIBCREG_EXTERN \
int libcreg_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_open_file_io_handle(
     libcreg_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcreg_error_t **error );

#endif /* !defined( LIBCREG_HAVE_BFIO ) */

/* Creates and opens a source file
 * Returns 1 if successful or -1 on error
 */
int creg_test_file_open_source(
     libcreg_file_t **file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	static char *function = "creg_test_file_open_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( file_io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file IO handle.",
		 function );

		return( -1 );
	}
	if( libcreg_file_initialize(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize file.",
		 function );

		goto on_error;
	}
	result = libcreg_file_open_file_io_handle(
	          *file,
	          file_io_handle,
	          LIBCREG_OPEN_READ,
	          error );

	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *file != NULL )
	{
		libcreg_file_free(
		 file,
		 NULL );
	}
	return( -1 );
}

/* Closes and frees a source file
 * Returns 1 if successful or -1 on error
 */
int creg_test_file_close_source(
     libcreg_file_t **file,
     libcerror_error_t **error )
{
	static char *function = "creg_test_file_close_source";
	int result            = 0;

	if( file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file.",
		 function );

		return( -1 );
	}
	if( libcreg_file_close(
	     *file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close file.",
		 function );

		result = -1;
	}
	if( libcreg_file_free(
	     file,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free file.",
		 function );

		result = -1;
	}
	return( result );
}

/* Tests the libcreg_file_initialize function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_initialize(
     void )
{
	libcerror_error_t *error        = NULL;
	libcreg_file_t *file            = NULL;
	int result                      = 0;

#if defined( HAVE_CREG_TEST_MEMORY )
	int number_of_malloc_fail_tests = 1;
	int number_of_memset_fail_tests = 1;
	int test_number                 = 0;
#endif

	/* Test regular cases
	 */
	result = libcreg_file_initialize(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_file_free(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_file_initialize(
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

	file = (libcreg_file_t *) 0x12345678UL;

	result = libcreg_file_initialize(
	          &file,
	          &error );

	file = NULL;

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
		/* Test libcreg_file_initialize with malloc failing
		 */
		creg_test_malloc_attempts_before_fail = test_number;

		result = libcreg_file_initialize(
		          &file,
		          &error );

		if( creg_test_malloc_attempts_before_fail != -1 )
		{
			creg_test_malloc_attempts_before_fail = -1;

			if( file != NULL )
			{
				libcreg_file_free(
				 &file,
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
			 "file",
			 file );

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
		/* Test libcreg_file_initialize with memset failing
		 */
		creg_test_memset_attempts_before_fail = test_number;

		result = libcreg_file_initialize(
		          &file,
		          &error );

		if( creg_test_memset_attempts_before_fail != -1 )
		{
			creg_test_memset_attempts_before_fail = -1;

			if( file != NULL )
			{
				libcreg_file_free(
				 &file,
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
			 "file",
			 file );

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
	if( file != NULL )
	{
		libcreg_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_file_free function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_free(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcreg_file_free(
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

/* Tests the libcreg_file_open function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_open(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	libcreg_file_t *file     = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = creg_test_get_narrow_source(
	          source,
	          narrow_source,
	          256,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_file_initialize(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libcreg_file_open(
	          file,
	          narrow_source,
	          LIBCREG_OPEN_READ,
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
	result = libcreg_file_open(
	          NULL,
	          narrow_source,
	          LIBCREG_OPEN_READ,
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

	result = libcreg_file_open(
	          file,
	          NULL,
	          LIBCREG_OPEN_READ,
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

	result = libcreg_file_open(
	          file,
	          narrow_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libcreg_file_open(
	          file,
	          narrow_source,
	          LIBCREG_OPEN_READ,
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
	result = libcreg_file_free(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libcreg_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libcreg_file_open_wide function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_open_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	libcreg_file_t *file     = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = creg_test_get_wide_source(
	          source,
	          wide_source,
	          256,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_file_initialize(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libcreg_file_open_wide(
	          file,
	          wide_source,
	          LIBCREG_OPEN_READ,
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
	result = libcreg_file_open_wide(
	          NULL,
	          wide_source,
	          LIBCREG_OPEN_READ,
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

	result = libcreg_file_open_wide(
	          file,
	          NULL,
	          LIBCREG_OPEN_READ,
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

	result = libcreg_file_open_wide(
	          file,
	          wide_source,
	          -1,
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

	/* Test open when already opened
	 */
	result = libcreg_file_open_wide(
	          file,
	          wide_source,
	          LIBCREG_OPEN_READ,
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
	result = libcreg_file_free(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libcreg_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libcreg_file_open_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_open_file_io_handle(
     const system_character_t *source )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libcreg_file_t *file             = NULL;
	size_t string_length             = 0;
	int result                       = 0;

	/* Initialize test
	 */
	result = libbfio_file_initialize(
	          &file_io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "file_io_handle",
	 file_io_handle );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	string_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          string_length,
	          &error );
#endif
	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_file_initialize(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open
	 */
	result = libcreg_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBCREG_OPEN_READ,
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
	result = libcreg_file_open_file_io_handle(
	          NULL,
	          file_io_handle,
	          LIBCREG_OPEN_READ,
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

	result = libcreg_file_open_file_io_handle(
	          file,
	          NULL,
	          LIBCREG_OPEN_READ,
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

	result = libcreg_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          -1,
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

	/* Test open when already opened
	 */
	result = libcreg_file_open_file_io_handle(
	          file,
	          file_io_handle,
	          LIBCREG_OPEN_READ,
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
	result = libcreg_file_free(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libbfio_handle_free(
	          &file_io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "file_io_handle",
	 file_io_handle );

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
	if( file != NULL )
	{
		libcreg_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_file_close function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_close(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test error cases
	 */
	result = libcreg_file_close(
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

/* Tests the libcreg_file_open and libcreg_file_close functions
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_open_close(
     const system_character_t *source )
{
	libcerror_error_t *error = NULL;
	libcreg_file_t *file     = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = libcreg_file_initialize(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "file",
	 file );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcreg_file_open_wide(
	          file,
	          source,
	          LIBCREG_OPEN_READ,
	          &error );
#else
	result = libcreg_file_open(
	          file,
	          source,
	          LIBCREG_OPEN_READ,
	          &error );
#endif

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_file_close(
	          file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test open and close a second time to validate clean up on close
	 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcreg_file_open_wide(
	          file,
	          source,
	          LIBCREG_OPEN_READ,
	          &error );
#else
	result = libcreg_file_open(
	          file,
	          source,
	          LIBCREG_OPEN_READ,
	          &error );
#endif

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	result = libcreg_file_close(
	          file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Clean up
	 */
	result = libcreg_file_free(
	          &file,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "file",
	 file );

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
	if( file != NULL )
	{
		libcreg_file_free(
		 &file,
		 NULL );
	}
	return( 0 );
}

/* Tests the libcreg_file_signal_abort function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_signal_abort(
     libcreg_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libcreg_file_signal_abort(
	          file,
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
	result = libcreg_file_signal_abort(
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

/* Tests the libcreg_file_is_corrupted function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_is_corrupted(
     libcreg_file_t *file )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libcreg_file_is_corrupted(
	          file,
	          &error );

	CREG_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	/* Test error cases
	 */
	result = libcreg_file_is_corrupted(
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

/* Tests the libcreg_file_get_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_get_ascii_codepage(
     libcreg_file_t *file )
{
	libcerror_error_t *error  = NULL;
	int ascii_codepage        = 0;
	int ascii_codepage_is_set = 0;
	int result                = 0;

	/* Test regular cases
	 */
	result = libcreg_file_get_ascii_codepage(
	          file,
	          &ascii_codepage,
	          &error );

	CREG_TEST_ASSERT_NOT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	ascii_codepage_is_set = result;

	/* Test error cases
	 */
	result = libcreg_file_get_ascii_codepage(
	          NULL,
	          &ascii_codepage,
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

	if( ascii_codepage_is_set != 0 )
	{
		result = libcreg_file_get_ascii_codepage(
		          file,
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
	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libcreg_file_set_ascii_codepage function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_set_ascii_codepage(
     libcreg_file_t *file )
{
	int supported_codepages[ 15 ] = {
		LIBCREG_CODEPAGE_ASCII,
		LIBCREG_CODEPAGE_WINDOWS_874,
		LIBCREG_CODEPAGE_WINDOWS_932,
		LIBCREG_CODEPAGE_WINDOWS_936,
		LIBCREG_CODEPAGE_WINDOWS_949,
		LIBCREG_CODEPAGE_WINDOWS_950,
		LIBCREG_CODEPAGE_WINDOWS_1250,
		LIBCREG_CODEPAGE_WINDOWS_1251,
		LIBCREG_CODEPAGE_WINDOWS_1252,
		LIBCREG_CODEPAGE_WINDOWS_1253,
		LIBCREG_CODEPAGE_WINDOWS_1254,
		LIBCREG_CODEPAGE_WINDOWS_1255,
		LIBCREG_CODEPAGE_WINDOWS_1256,
		LIBCREG_CODEPAGE_WINDOWS_1257,
		LIBCREG_CODEPAGE_WINDOWS_1258 };

	int unsupported_codepages[ 17 ] = {
		LIBCREG_CODEPAGE_ISO_8859_1,
		LIBCREG_CODEPAGE_ISO_8859_2,
		LIBCREG_CODEPAGE_ISO_8859_3,
		LIBCREG_CODEPAGE_ISO_8859_4,
		LIBCREG_CODEPAGE_ISO_8859_5,
		LIBCREG_CODEPAGE_ISO_8859_6,
		LIBCREG_CODEPAGE_ISO_8859_7,
		LIBCREG_CODEPAGE_ISO_8859_8,
		LIBCREG_CODEPAGE_ISO_8859_9,
		LIBCREG_CODEPAGE_ISO_8859_10,
		LIBCREG_CODEPAGE_ISO_8859_11,
		LIBCREG_CODEPAGE_ISO_8859_13,
		LIBCREG_CODEPAGE_ISO_8859_14,
		LIBCREG_CODEPAGE_ISO_8859_15,
		LIBCREG_CODEPAGE_ISO_8859_16,
		LIBCREG_CODEPAGE_KOI8_R,
		LIBCREG_CODEPAGE_KOI8_U };

	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int index                = 0;
	int result               = 0;

	/* Test set ASCII codepage
	 */
	for( index = 0;
	     index < 15;
	     index++ )
	{
		codepage = supported_codepages[ index ];

		result = libcreg_file_set_ascii_codepage(
		          file,
		          codepage,
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
	result = libcreg_file_set_ascii_codepage(
	          NULL,
	          LIBCREG_CODEPAGE_ASCII,
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

	for( index = 0;
	     index < 17;
	     index++ )
	{
		codepage = unsupported_codepages[ index ];

		result = libcreg_file_set_ascii_codepage(
		          file,
		          codepage,
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
	result = libcreg_file_set_ascii_codepage(
	          file,
	          LIBCREG_CODEPAGE_WINDOWS_1252,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

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
	return( 0 );
}

/* Tests the libcreg_file_get_type function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_get_type(
     libcreg_file_t *file )
{
	libcerror_error_t *error = NULL;
	uint32_t type            = 0;
	int result               = 0;

	/* Test regular cases
	 */
	result = libcreg_file_get_type(
	          file,
	          &type,
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
	result = libcreg_file_get_type(
	          NULL,
	          &type,
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

	result = libcreg_file_get_type(
	          file,
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

/* Tests the libcreg_file_get_root_key function
 * Returns 1 if successful or 0 if not
 */
int creg_test_file_get_root_key(
     libcreg_file_t *file )
{
	libcerror_error_t *error = NULL;
	libcreg_key_t *root_key  = NULL;
	int result               = 0;

	/* Test regular cases
	 */
	result = libcreg_file_get_root_key(
	          file,
	          &root_key,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "error",
	 error );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "root_key",
	 root_key );

	result = libcreg_key_free(
	          &root_key,
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
	result = libcreg_file_get_root_key(
	          NULL,
	          &root_key,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "root_key",
	 root_key );

	CREG_TEST_ASSERT_IS_NOT_NULL(
	 "error",
	 error );

	libcerror_error_free(
	 &error );

	result = libcreg_file_get_root_key(
	          file,
	          NULL,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 -1 );

	CREG_TEST_ASSERT_IS_NULL(
	 "root_key",
	 root_key );

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
	if( root_key != NULL )
	{
		libcreg_key_free(
		 &root_key,
		 NULL );
	}
	return( 0 );
}

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain(
     int argc,
     wchar_t * const argv[] )
#else
int main(
     int argc,
     char * const argv[] )
#endif
{
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	libcreg_file_t *file             = NULL;
	system_character_t *source       = NULL;
	system_integer_t option          = 0;
	size_t string_length             = 0;
	int result                       = 0;

	while( ( option = creg_test_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM ".\n",
				 argv[ optind - 1 ] );

				return( EXIT_FAILURE );
		}
	}
	if( optind < argc )
	{
		source = argv[ optind ];
	}
#if defined( HAVE_DEBUG_OUTPUT ) && defined( CREG_TEST_FILE_VERBOSE )
	libcreg_notify_set_verbose(
	 1 );
	libcreg_notify_set_stream(
	 stderr,
	 NULL );
#endif

	CREG_TEST_RUN(
	 "libcreg_file_initialize",
	 creg_test_file_initialize );

	CREG_TEST_RUN(
	 "libcreg_file_free",
	 creg_test_file_free );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		result = libbfio_file_initialize(
		          &file_io_handle,
		          &error );

		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        CREG_TEST_ASSERT_IS_NOT_NULL(
	         "file_io_handle",
	         file_io_handle );

	        CREG_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		string_length = system_string_length(
		                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libbfio_file_set_name_wide(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#else
		result = libbfio_file_set_name(
		          file_io_handle,
		          source,
		          string_length,
		          &error );
#endif
		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

	        CREG_TEST_ASSERT_IS_NULL(
	         "error",
	         error );

		result = libcreg_check_file_signature_file_io_handle(
		          file_io_handle,
		          &error );

		CREG_TEST_ASSERT_NOT_EQUAL_INT(
		 "result",
		 result,
		 -1 );

		CREG_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( result != 0 )
	{
		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_open",
		 creg_test_file_open,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_open_wide",
		 creg_test_file_open_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_open_file_io_handle",
		 creg_test_file_open_file_io_handle,
		 source );

		CREG_TEST_RUN(
		 "libcreg_file_close",
		 creg_test_file_close );

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_open_close",
		 creg_test_file_open_close,
		 source );

		/* Initialize file for tests
		 */
		result = creg_test_file_open_source(
		          &file,
		          file_io_handle,
		          &error );

		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CREG_TEST_ASSERT_IS_NOT_NULL(
		 "file",
		 file );

		CREG_TEST_ASSERT_IS_NULL(
		 "error",
		 error );

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_signal_abort",
		 creg_test_file_signal_abort,
		 file );

#if defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT )

		/* TODO: add tests for libcreg_file_open_read */

#endif /* defined( __GNUC__ ) && !defined( LIBCREG_DLL_IMPORT ) */

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_is_corrupted",
		 creg_test_file_is_corrupted,
		 file );

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_get_ascii_codepage",
		 creg_test_file_get_ascii_codepage,
		 file );

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_set_ascii_codepage",
		 creg_test_file_set_ascii_codepage,
		 file );

		/* TODO: add tests for libcreg_file_get_format_version */

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_get_type",
		 creg_test_file_get_type,
		 file );

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_file_get_root_key",
		 creg_test_file_get_root_key,
		 file );

		/* TODO: add tests for libcreg_file_get_key_by_utf8_path */

		/* TODO: add tests for libcreg_file_get_key_by_utf16_path */

		/* Clean up
		 */
		result = creg_test_file_close_source(
		          &file,
		          &error );

		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 0 );

		CREG_TEST_ASSERT_IS_NULL(
		 "file",
		 file );

		CREG_TEST_ASSERT_IS_NULL(
		 "error",
		 error );
	}
	if( file_io_handle != NULL )
	{
		result = libbfio_handle_free(
		          &file_io_handle,
		          &error );

		CREG_TEST_ASSERT_EQUAL_INT(
		 "result",
		 result,
		 1 );

		CREG_TEST_ASSERT_IS_NULL(
	         "file_io_handle",
	         file_io_handle );

	        CREG_TEST_ASSERT_IS_NULL(
	         "error",
	         error );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file != NULL )
	{
		libcreg_file_free(
		 &file,
		 NULL );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

