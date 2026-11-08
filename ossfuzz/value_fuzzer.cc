/*
 * OSS-Fuzz target for libcreg value type
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

/* Limits the number of sub keys to walk per key
 */
#define FUZZER_KEY_LIMIT  8

/* Limits the number of values to walk per key
 */
#define FUZZER_VALUE_LIMIT  8

/* Limits the size of the names used to look up values
 */
#define FUZZER_LOOKUP_STRING_LIMIT  ( (size_t) 0x4000 )

/* The ASCII codepages supported by libcreg_file_set_ascii_codepage
 */
static const int ascii_codepages[] = {
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

static const size_t number_of_ascii_codepages =
	  sizeof( ascii_codepages ) / sizeof( ascii_codepages[ 0 ] );

/* Decodes the value using the accessors of the value type
 */
static void value_decode_fuzzer(
     libcreg_value_t *value )
{
	uint8_t name[ 2048 ];
	uint8_t utf8_string[ 8192 ];
	uint8_t value_data[ 8192 ];
	uint16_t utf16_string[ 4096 ];

	off64_t offset            = 0;
	size_t name_size          = 0;
	size_t utf8_name_size     = 0;
	size_t utf16_name_size    = 0;
	size_t value_data_size    = 0;
	uint32_t value_type       = 0;
	uint32_t value_32bit      = 0;
	uint64_t value_64bit      = 0;

	libcreg_value_is_corrupted(
	 value,
	 NULL );

	libcreg_value_get_offset(
	 value,
	 &offset,
	 NULL );

	libcreg_value_get_value_type(
	 value,
	 &value_type,
	 NULL );

	/* The typed accessors return an error for an unsupported
	 * value type, which is part of what this exercises
	 */
	libcreg_value_get_value_32bit(
	 value,
	 &value_32bit,
	 NULL );

	libcreg_value_get_value_64bit(
	 value,
	 &value_64bit,
	 NULL );

	/* The name may be absent, e.g. the default value
	 */
	libcreg_value_get_name_size(
	 value,
	 &name_size,
	 NULL );

	libcreg_value_get_utf8_name_size(
	 value,
	 &utf8_name_size,
	 NULL );

	libcreg_value_get_utf16_name_size(
	 value,
	 &utf16_name_size,
	 NULL );

	if( name_size > 0 )
	{
		libcreg_value_get_name(
		 value,
		 name,
		 2048,
		 NULL );
	}
	if( utf8_name_size > 0 )
	{
		libcreg_value_get_utf8_name(
		 value,
		 utf8_string,
		 8192,
		 NULL );
	}
	if( utf16_name_size > 0 )
	{
		libcreg_value_get_utf16_name(
		 value,
		 utf16_string,
		 4096,
		 NULL );
	}
	libcreg_value_get_value_data_size(
	 value,
	 &value_data_size,
	 NULL );

	libcreg_value_get_value_data(
	 value,
	 value_data,
	 8192,
	 NULL );

	libcreg_value_get_value_utf8_string_size(
	 value,
	 &value_data_size,
	 NULL );

	libcreg_value_get_value_utf8_string(
	 value,
	 utf8_string,
	 8192,
	 NULL );

	libcreg_value_get_value_utf16_string_size(
	 value,
	 &value_data_size,
	 NULL );

	libcreg_value_get_value_utf16_string(
	 value,
	 utf16_string,
	 4096,
	 NULL );

	libcreg_value_get_value_binary_data_size(
	 value,
	 &value_data_size,
	 NULL );

	libcreg_value_get_value_binary_data(
	 value,
	 value_data,
	 8192,
	 NULL );
}

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	libbfio_handle_t *file_io_handle = NULL;
	libcreg_file_t *file             = NULL;
	libcreg_key_t *key               = NULL;
	libcreg_key_t *sub_key           = NULL;
	libcreg_value_t *value           = NULL;
	size_t query_size                = 0;
	int number_of_sub_keys           = 0;
	int number_of_values             = 0;
	int sub_key_index                = 0;
	int value_index                  = 0;

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
	/* The codepage used to convert value names and value data
	 * is selected from the fuzz data
	 */
	if( size > 0 )
	{
		libcreg_file_set_ascii_codepage(
		 file,
		 ascii_codepages[ data[ 0 ] % number_of_ascii_codepages ],
		 NULL );
	}
	if( size > 0 )
	{
		query_size = size;

		if( query_size > FUZZER_LOOKUP_STRING_LIMIT )
		{
			query_size = FUZZER_LOOKUP_STRING_LIMIT;
		}
	}
	if( libcreg_file_get_root_key(
	     file,
	     &key,
	     NULL ) == 1 )
	{
		/* Retrieves the default value
		 */
		libcreg_key_get_value_by_utf8_name(
		 key,
		 NULL,
		 0,
		 &value,
		 NULL );

		if( value != NULL )
		{
			value_decode_fuzzer(
			 value );

			libcreg_value_free(
			 &value,
			 NULL );
		}
		value = NULL;

		if( libcreg_key_get_number_of_values(
		     key,
		     &number_of_values,
		     NULL ) == 1 )
		{
			for( value_index = 0;
			     value_index < number_of_values;
			     value_index++ )
			{
				if( value_index >= FUZZER_VALUE_LIMIT )
				{
					break;
				}
				value = NULL;

				if( libcreg_key_get_value_by_index(
				     key,
				     value_index,
				     &value,
				     NULL ) != 1 )
				{
					continue;
				}
				value_decode_fuzzer(
				 value );

				libcreg_value_free(
				 &value,
				 NULL );
			}
		}
		if( libcreg_key_get_number_of_sub_keys(
		     key,
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
				     key,
				     sub_key_index,
				     &sub_key,
				     NULL ) != 1 )
				{
					continue;
				}
				if( libcreg_key_get_number_of_values(
				     sub_key,
				     &number_of_values,
				     NULL ) == 1 )
				{
					for( value_index = 0;
					     value_index < number_of_values;
					     value_index++ )
					{
						if( value_index >= FUZZER_VALUE_LIMIT )
						{
							break;
						}
						value = NULL;

						if( libcreg_key_get_value_by_index(
						     sub_key,
						     value_index,
						     &value,
						     NULL ) != 1 )
						{
							continue;
						}
						value_decode_fuzzer(
						 value );

						libcreg_value_free(
						 &value,
						 NULL );
					}
				}
				/* Retrieves a value by a name derived from
				 * the fuzz data. The data is not necessarily
				 * valid UTF-8 or UTF-16 which exercises the
				 * validation and conversion error paths
				 * in the library
				 */
				if( size > 0 )
				{
					value = NULL;

					libcreg_key_get_value_by_utf8_name(
					 sub_key,
					 data,
					 query_size,
					 &value,
					 NULL );

					if( value != NULL )
					{
						value_decode_fuzzer(
						 value );

						libcreg_value_free(
						 &value,
						 NULL );
					}
					value = NULL;

					if( query_size >= 2 )
					{
						libcreg_key_get_value_by_utf16_name(
						 sub_key,
						 ( const uint16_t * ) data,
						 ( query_size / 2 ),
						 &value,
						 NULL );

						if( value != NULL )
						{
							value_decode_fuzzer(
							 value );

							libcreg_value_free(
							 &value,
							 NULL );
						}
					}
				}
				libcreg_key_free(
				 &sub_key,
				 NULL );
			}
		}
		libcreg_key_free(
		 &key,
		 NULL );
	}
	libcreg_file_close(
	 file,
	 NULL );

on_error_libcreg_file:
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
