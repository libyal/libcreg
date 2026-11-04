/*
 * OSS-Fuzz target for libcreg key type
 *
 * Copyright (C) 2013-2026, Joachim Metz <joachim.metz@gmail.com>
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

#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>

/* Note that some of the OSS-Fuzz engines use C++
 */
extern "C" {

#include "ossfuzz_libbfio.h"
#include "ossfuzz_libcreg.h"

#if !defined( LIBCREG_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_open_file_io_handle(
      libcreg_file_t *file,
      libbfio_handle_t *file_io_handle,
      int access_flags,
      libcreg_error_t **error );

#endif /* !defined( LIBCREG_HAVE_BFIO ) */

/* Limits on the traversal and buffer sizes to bound the time a single input takes
 */
#define FUZZER_KEY_LIMIT           4
#define FUZZER_STRING_SIZE_LIMIT   ( (size_t) 0x00100000 )
#define FUZZER_LOOKUP_STRING_LIMIT ( (size_t) 0x4000 )

/* The ASCII codepages supported by libcreg_file_set_ascii_codepage
 */
static const int value_codepages[] = {
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
	LIBCREG_CODEPAGE_WINDOWS_1258,
};

static const size_t value_number_of_codepages =
	  sizeof( value_codepages ) / sizeof( value_codepages[ 0 ] );

/* Decodes the name and metadata of a key using the typed accessors
 * Returns 1 if successful or -1 on error
 */
int key_decode_fuzzer(
     libcreg_key_t *key )
{
	uint8_t *name            = NULL;
	uint8_t *utf8_string     = NULL;
	uint16_t *utf16_string   = NULL;
	off64_t offset           = 0;
	size_t name_size         = 0;
	size_t utf8_string_size  = 0;
	size_t utf16_string_size = 0;
	uint8_t name_byte        = 0;
	int result               = 0;

	if( key == NULL )
	{
		return( -1 );
	}
	libcreg_key_get_offset(
	 key,
	 &offset,
	 NULL );

	libcreg_key_is_corrupted(
	 key,
	 NULL );

	result = libcreg_key_get_name_size(
	          key,
	          &name_size,
	          NULL );
       
	if( ( result == 1 )
	 && ( name_size > 0 )
	 && ( name_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		name = (uint8_t *) malloc( name_size );

		if( name != NULL )
		{
			result = libcreg_key_get_name(
			          key,
			          name,
			          name_size,
			          NULL );
		       
			if( result == 1 )
			{
				name_byte = name[ 0 ];
			}
			free( name );
		}
	}
	result = libcreg_key_get_utf8_name_size(
	          key,
	          &utf8_string_size,
	          NULL );
       
	if( ( result == 1 )
	 && ( utf8_string_size > 0 )
	 && ( utf8_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf8_string = (uint8_t *) malloc( utf8_string_size );

		if( utf8_string != NULL )
		{
			result = libcreg_key_get_utf8_name(
			          key,
			          utf8_string,
			          utf8_string_size,
			          NULL );
		       
			if( result == 1 )
			{
				name_byte ^= utf8_string[ 0 ];
			}
			free( utf8_string );
		}
	}
	/* The size is the number of UTF-16 characters,
	 * including the end of string character
	 */
	result = libcreg_key_get_utf16_name_size(
	          key,
	          &utf16_string_size,
	          NULL );
       
	if( ( result == 1 )
	 && ( utf16_string_size > 0 )
	 && ( utf16_string_size <= FUZZER_STRING_SIZE_LIMIT ) )
	{
		utf16_string = (uint16_t *) malloc( utf16_string_size * sizeof( uint16_t ) );

		if( utf16_string != NULL )
		{
			result = libcreg_key_get_utf16_name(
			          key,
			          utf16_string,
			          utf16_string_size,
			          NULL );
		       
			if( result == 1 )
			{
				name_byte ^= (uint8_t) utf16_string[ 0 ];
			}
			free( utf16_string );
		}
	}
	/* Use the decoded values as a sink to keep them live
	 */
	if( ( name_byte ^ (uint8_t) offset ) != 0 )
	{
		/* No action required
		 */
	}
	return( 1 );
}

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle  = NULL;
	libcreg_file_t *file              = NULL;
	libcreg_key_t *root_key           = NULL;
	libcreg_key_t *sub_key            = NULL;
	libcreg_key_t *lookup_key         = NULL;
	libcreg_value_t *value            = NULL;
	uint16_t *utf16_lookup_string     = NULL;
	size_t index                      = 0;
	size_t utf8_lookup_string_length  = 0;
	size_t utf16_lookup_string_length = 0;
	int number_of_sub_keys            = 0;
	int sub_key_index                 = 0;

	if( libbfio_memory_range_initialize(
	     &file_io_handle,
	     NULL ) != 1 )
	{
		return( 0 );
	}
	if( libbfio_memory_range_set(
	     file_io_handle,
	     (uint8_t *) data,
	     size,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libcreg_file_initialize(
	     &file,
	     NULL ) != 1 )
	{
		goto on_error_libbfio;
	}
	if( libcreg_file_open_file_io_handle(
	     file,
	     file_io_handle,
	     LIBCREG_OPEN_READ,
	     NULL ) != 1 )
	{
		goto on_error_libcreg_file;
	}
	/* The codepage is selected from the fuzz data to steer the
	 * codepage conversion of key names
	 */
	if( size > 0 )
	{
		libcreg_file_set_ascii_codepage(
		 file,
		 value_codepages[ data[ 0 ] % value_number_of_codepages ],
		 NULL );
	}
	utf8_lookup_string_length = size;

	if( utf8_lookup_string_length > FUZZER_LOOKUP_STRING_LIMIT )
	{
		utf8_lookup_string_length = FUZZER_LOOKUP_STRING_LIMIT;
	}
	utf16_lookup_string_length = ( size / 2 );

	if( utf16_lookup_string_length > FUZZER_LOOKUP_STRING_LIMIT )
	{
		utf16_lookup_string_length = FUZZER_LOOKUP_STRING_LIMIT;
	}
	if( utf16_lookup_string_length > 0 )
	{
		utf16_lookup_string = (uint16_t *) malloc( utf16_lookup_string_length * sizeof( uint16_t ) );

		if( utf16_lookup_string != NULL )
		{
			for( index = 0;
			     index < utf16_lookup_string_length;
			     index++ )
			{
				utf16_lookup_string[ index ] = (uint16_t) ( data[ index * 2 ] | ( (uint16_t) data[ index * 2 + 1 ] << 8 ) );
			}
		}
	}
	if( libcreg_file_get_root_key(
	     file,
	     &root_key,
	     NULL ) == 1 )
	{
		key_decode_fuzzer(
		 root_key );

		/* Searches for sub key and value using names and paths derived
		 * from the fuzz data. The data is not necessarily valid UTF-8
		 * or UTF-16 which exercises the validation and conversion
		 * error paths in the library
		 */
		if( utf8_lookup_string_length > 0 )
		{
			libcreg_key_get_sub_key_by_utf8_name(
			 root_key,
			 data,
			 utf8_lookup_string_length,
			 &lookup_key,
			 NULL );

			if( lookup_key != NULL )
			{
				key_decode_fuzzer(
				 lookup_key );
				libcreg_key_free(
				 &lookup_key,
				 NULL );
			}
			lookup_key = NULL;

			libcreg_key_get_sub_key_by_utf8_path(
			 root_key,
			 data,
			 utf8_lookup_string_length,
			 &lookup_key,
			 NULL );

			if( lookup_key != NULL )
			{
				libcreg_key_free(
				 &lookup_key,
				 NULL );
			}
			lookup_key = NULL;

			libcreg_file_get_key_by_utf8_path(
			 file,
			 data,
			 utf8_lookup_string_length,
			 &lookup_key,
			 NULL );

			if( lookup_key != NULL )
			{
				libcreg_key_free(
				 &lookup_key,
				 NULL );
			}
			lookup_key = NULL;

			libcreg_key_get_value_by_utf8_name(
			 root_key,
			 data,
			 utf8_lookup_string_length,
			 &value,
			 NULL );

			if( value != NULL )
			{
				libcreg_value_free(
				 &value,
				 NULL );
			}
			value = NULL;
		}
		/* Retrieves the default value
		 */
		libcreg_key_get_value_by_utf8_name(
		 root_key,
		 NULL,
		 0,
		 &value,
		 NULL );

		if( value != NULL )
		{
			libcreg_value_free(
			 &value,
			 NULL );
		}
		value = NULL;

		if( ( utf16_lookup_string != NULL )
		 && ( utf16_lookup_string_length > 0 ) )
		{
			libcreg_key_get_sub_key_by_utf16_name(
			 root_key,
			 utf16_lookup_string,
			 utf16_lookup_string_length,
			 &lookup_key,
			 NULL );

			if( lookup_key != NULL )
			{
				libcreg_key_free(
				 &lookup_key,
				 NULL );
			}
			lookup_key = NULL;

			libcreg_key_get_sub_key_by_utf16_path(
			 root_key,
			 utf16_lookup_string,
			 utf16_lookup_string_length,
			 &lookup_key,
			 NULL );

			if( lookup_key != NULL )
			{
				libcreg_key_free(
				 &lookup_key,
				 NULL );
			}
			lookup_key = NULL;

			libcreg_file_get_key_by_utf16_path(
			 file,
			 utf16_lookup_string,
			 utf16_lookup_string_length,
			 &lookup_key,
			 NULL );

			if( lookup_key != NULL )
			{
				libcreg_key_free(
				 &lookup_key,
				 NULL );
			}
			lookup_key = NULL;

			libcreg_key_get_value_by_utf16_name(
			 root_key,
			 utf16_lookup_string,
			 utf16_lookup_string_length,
			 &value,
			 NULL );

			if( value != NULL )
			{
				libcreg_value_free(
				 &value,
				 NULL );
			}
			value = NULL;
		}
		if( libcreg_key_get_number_of_sub_keys(
		     root_key,
		     &number_of_sub_keys,
		     NULL ) == 1 )
		{
			for( sub_key_index = 0;
			     sub_key_index < number_of_sub_keys;
			     sub_key_index++ )
			{
				if( sub_key_index >= FUZZER_KEY_LIMIT )
				{
					break;
				}
				sub_key = NULL;

				if( libcreg_key_get_sub_key_by_index(
				     root_key,
				     sub_key_index,
				     &sub_key,
				     NULL ) != 1 )
				{
					continue;
				}
				key_decode_fuzzer(
				 sub_key );

				/* Retrieves a value by name derived from the fuzz data
				 */
				if( utf8_lookup_string_length > 0 )
				{
					libcreg_key_get_value_by_utf8_name(
					 sub_key,
					 data,
					 utf8_lookup_string_length,
					 &value,
					 NULL );

					if( value != NULL )
					{
						libcreg_value_free(
						 &value,
						 NULL );
					}
					value = NULL;
				}
				if( ( utf16_lookup_string != NULL )
				 && ( utf16_lookup_string_length > 0 ) )
				{
					libcreg_key_get_value_by_utf16_name(
					 sub_key,
					 utf16_lookup_string,
					 utf16_lookup_string_length,
					 &value,
					 NULL );

					if( value != NULL )
					{
						libcreg_value_free(
						 &value,
						 NULL );
					}
					value = NULL;
				}
				libcreg_key_free(
				 &sub_key,
				 NULL );
			}
		}
		libcreg_key_free(
		 &root_key,
		 NULL );
	}
	libcreg_file_close(
	 file,
	 NULL );

on_error_libcreg_file:
	free( utf16_lookup_string );
	libcreg_file_free(
	 &file,
	 NULL );

on_error_libbfio:
	libbfio_handle_free(
	 &file_io_handle,
	 NULL );

	return( 0 );
}

} /* extern "C" */
