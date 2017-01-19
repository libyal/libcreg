/*
 * Library support functions test program
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
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#include "creg_test_getopt.h"
#include "creg_test_libbfio.h"
#include "creg_test_libcerror.h"
#include "creg_test_libclocale.h"
#include "creg_test_libcreg.h"
#include "creg_test_libuna.h"
#include "creg_test_macros.h"
#include "creg_test_unused.h"

LIBCREG_EXTERN \
int libcreg_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

/* Retrieves source as a narrow string
 * Returns 1 if successful or -1 on error
 */
int creg_test_support_get_narrow_source(
     const system_character_t *source,
     char *narrow_string,
     size_t narrow_string_size,
     libcerror_error_t **error )
{
	static char *function     = "creg_test_support_get_narrow_source";
	size_t narrow_source_size = 0;
	size_t source_length      = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result                = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( narrow_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid narrow string.",
		 function );

		return( -1 );
	}
	if( narrow_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid narrow string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          &narrow_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_size_from_utf32(
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_size_from_utf16(
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &narrow_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine narrow string size.",
		 function );

		return( -1 );
	}
#else
	narrow_source_size = source_length + 1;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( narrow_string_size < narrow_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: narrow string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf8_string_copy_from_utf32(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf8_string_copy_from_utf16(
		          (libuna_utf8_character_t *) narrow_string,
		          narrow_string_size,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_byte_stream_copy_from_utf32(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf32_character_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_byte_stream_copy_from_utf16(
		          (uint8_t *) narrow_string,
		          narrow_string_size,
		          libclocale_codepage,
		          (libuna_utf16_character_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
#else
	if( system_string_copy(
	     narrow_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set narrow string.",
		 function );

		return( -1 );
	}
	narrow_string[ source_length ] = 0;

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Retrieves source as a wide string
 * Returns 1 if successful or -1 on error
 */
int creg_test_support_get_wide_source(
     const system_character_t *source,
     wchar_t *wide_string,
     size_t wide_string_size,
     libcerror_error_t **error )
{
	static char *function   = "creg_test_support_get_wide_source";
	size_t wide_source_size = 0;
	size_t source_length    = 0;

#if !defined( HAVE_WIDE_SYSTEM_CHARACTER )
	int result              = 0;
#endif

	if( source == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid source.",
		 function );

		return( -1 );
	}
	if( wide_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid wide string.",
		 function );

		return( -1 );
	}
	if( wide_string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid wide string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	source_length = system_string_length(
	                 source );

	if( source_length > (size_t) ( SSIZE_MAX - 1 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid source length value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	wide_source_size = source_length + 1;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_utf8(
		          (libuna_utf8_character_t *) source,
		          source_length + 1,
		          &wide_source_size,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_size_from_byte_stream(
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          &wide_source_size,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to determine wide string size.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	if( wide_string_size < wide_source_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: wide string too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( system_string_copy(
	     wide_string,
	     source,
	     source_length ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}
	wide_string[ source_length ] = 0;
#else
	if( libclocale_codepage == 0 )
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_utf8(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_utf8(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          error );
#endif
	}
	else
	{
#if SIZEOF_WCHAR_T == 4
		result = libuna_utf32_string_copy_from_byte_stream(
		          (libuna_utf32_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#elif SIZEOF_WCHAR_T == 2
		result = libuna_utf16_string_copy_from_byte_stream(
		          (libuna_utf16_character_t *) wide_string,
		          wide_string_size,
		          (uint8_t *) source,
		          source_length + 1,
		          libclocale_codepage,
		          error );
#endif
	}
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set wide string.",
		 function );

		return( -1 );
	}

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

	return( 1 );
}

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libcreg_get_version function
 * Returns 1 if successful or 0 if not
 */
int creg_test_get_version(
     void )
{
	const char *version_string = NULL;
	int result                 = 0;

	version_string = libcreg_get_version();

	result = narrow_string_compare(
	          version_string,
	          LIBCREG_VERSION_STRING,
	          9 );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcreg_get_access_flags_read function
 * Returns 1 if successful or 0 if not
 */
int creg_test_get_access_flags_read(
     void )
{
	int access_flags = 0;

	access_flags = libcreg_get_access_flags_read();

	CREG_TEST_ASSERT_EQUAL_INT(
	 "access_flags",
	 access_flags,
	 LIBCREG_ACCESS_FLAG_READ );

	return( 1 );

on_error:
	return( 0 );
}

/* Tests the libcreg_get_codepage function
 * Returns 1 if successful or 0 if not
 */
int creg_test_get_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int codepage             = 0;
	int result               = 0;

	result = libcreg_get_codepage(
	          &codepage,
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
	result = libcreg_get_codepage(
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

/* Tests the libcreg_set_codepage function
 * Returns 1 if successful or 0 if not
 */
int creg_test_set_codepage(
     void )
{
	libcerror_error_t *error = NULL;
	int result               = 0;

	result = libcreg_set_codepage(
	          0,
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
	result = libcreg_set_codepage(
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

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	return( 0 );
}

/* Tests the libcreg_check_file_signature function
 * Returns 1 if successful or 0 if not
 */
int creg_test_check_file_signature(
     const system_character_t *source )
{
	char narrow_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = creg_test_support_get_narrow_source(
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

	/* Test check file signature
	 */
	result = libcreg_check_file_signature(
	          narrow_source,
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
	result = libcreg_check_file_signature(
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

#if defined( HAVE_WIDE_CHARACTER_TYPE )

/* Tests the libcreg_check_file_signature_wide function
 * Returns 1 if successful or 0 if not
 */
int creg_test_check_file_signature_wide(
     const system_character_t *source )
{
	wchar_t wide_source[ 256 ];

	libcerror_error_t *error = NULL;
	int result               = 0;

	/* Initialize test
	 */
	result = creg_test_support_get_wide_source(
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

	/* Test check file signature
	 */
	result = libcreg_check_file_signature_wide(
	          wide_source,
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
	result = libcreg_check_file_signature_wide(
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

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

/* Tests the libcreg_check_file_signature_file_io_handle function
 * Returns 1 if successful or 0 if not
 */
int creg_test_check_file_signature_file_io_handle(
     const system_character_t *source )
{
	uint8_t empty_block[ 512 ];

	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	void *memset_result              = NULL;
	size_t source_length             = 0;
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

	source_length = system_string_length(
	                 source );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libbfio_file_set_name_wide(
	          file_io_handle,
	          source,
	          source_length,
	          &error );
#else
	result = libbfio_file_set_name(
	          file_io_handle,
	          source,
	          source_length,
	          &error );
#endif
	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CREG_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libbfio_handle_open(
	          file_io_handle,
	          LIBBFIO_OPEN_READ,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CREG_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test check file signature
	 */
	result = libcreg_check_file_signature_file_io_handle(
	          file_io_handle,
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
	result = libcreg_check_file_signature_file_io_handle(
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
	result = libbfio_handle_close(
	          file_io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

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

	/* Initialize test
	 */
	memset_result = memory_set(
	                 empty_block,
	                 0,
	                 sizeof( uint8_t ) * 512 );

        CREG_TEST_ASSERT_IS_NOT_NULL(
         "memset_result",
         memset_result );

	result = libbfio_memory_range_initialize(
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

	result = libbfio_memory_range_set(
	          file_io_handle,
	          empty_block,
	          sizeof( uint8_t ) * 512,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CREG_TEST_ASSERT_IS_NULL(
         "error",
         error );

	result = libbfio_handle_open(
	          file_io_handle,
	          LIBBFIO_OPEN_READ,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 1 );

        CREG_TEST_ASSERT_IS_NULL(
         "error",
         error );

	/* Test check file signature
	 */
	result = libcreg_check_file_signature_file_io_handle(
	          file_io_handle,
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
	result = libbfio_handle_close(
	          file_io_handle,
	          &error );

	CREG_TEST_ASSERT_EQUAL_INT(
	 "result",
	 result,
	 0 );

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

	/* TODO test file too small */

	return( 1 );

on_error:
	if( error != NULL )
	{
		libcerror_error_free(
		 &error );
	}
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
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
	system_character_t *source = NULL;
	system_integer_t option    = 0;

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

	CREG_TEST_RUN(
	 "libcreg_get_version",
	 creg_test_get_version );

	CREG_TEST_RUN(
	 "libcreg_get_access_flags_read",
	 creg_test_get_access_flags_read );

	CREG_TEST_RUN(
	 "libcreg_get_codepage",
	 creg_test_get_codepage );

	CREG_TEST_RUN(
	 "libcreg_set_codepage",
	 creg_test_set_codepage );

#if !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 )
	if( source != NULL )
	{
		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_check_file_signature",
		 creg_test_check_file_signature,
		 source );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_check_file_signature_wide",
		 creg_test_check_file_signature_wide,
		 source );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

		CREG_TEST_RUN_WITH_ARGS(
		 "libcreg_check_file_signature_file_io_handle",
		 creg_test_check_file_signature_file_io_handle,
		 source );
	}
#endif /* !defined( __BORLANDC__ ) || ( __BORLANDC__ >= 0x0560 ) */

	return( EXIT_SUCCESS );

on_error:
	return( EXIT_FAILURE );
}

