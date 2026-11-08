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

/* Limits the size of the names and paths used to look up keys
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

int LLVMFuzzerTestOneInput(
     const uint8_t *data,
     size_t size )
{
	uint8_t name[ 2048 ];
	uint8_t utf8_string[ 8192 ];
	uint16_t utf16_string[ 4096 ];

	libbfio_handle_t *file_io_handle = NULL;
	libcreg_file_t *file             = NULL;
	libcreg_key_t *key               = NULL;
	libcreg_key_t *sub_key           = NULL;
	libcreg_value_t *value           = NULL;
	off64_t offset                   = 0;
	size_t name_size                 = 0;
	size_t query_size                = 0;
	int number_of_sub_keys           = 0;
	int sub_key_index                = 0;

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
	/* The codepage used to convert key names and value data
	 * is selected from the fuzz data
	 */
	if( size > 0 )
	{
		libcreg_file_set_ascii_codepage(
		 file,
		 ascii_codepages[ data[ 0 ] % number_of_ascii_codepages ],
		 NULL );
	}
	if( libcreg_file_get_root_key(
	     file,
	     &key,
	     NULL ) == 1 )
	{
		libcreg_key_is_corrupted(
		 key,
		 NULL );

		libcreg_key_get_offset(
		 key,
		 &offset,
		 NULL );

		libcreg_key_get_name_size(
		 key,
		 &name_size,
		 NULL );

		libcreg_key_get_name(
		 key,
		 name,
		 2048,
		 NULL );

		libcreg_key_get_utf8_name_size(
		 key,
		 &name_size,
		 NULL );

		libcreg_key_get_utf8_name(
		 key,
		 utf8_string,
		 8192,
		 NULL );

		libcreg_key_get_utf16_name_size(
		 key,
		 &name_size,
		 NULL );

		libcreg_key_get_utf16_name(
		 key,
		 utf16_string,
		 4096,
		 NULL );

		/* Looks up sub keys and values using names and paths derived
		 * from the fuzz data. The data is not necessarily valid UTF-8
		 * or UTF-16 which exercises the validation and conversion
		 * error paths in the library
		 */
		if( size > 0 )
		{
			query_size = size;

			if( query_size > FUZZER_LOOKUP_STRING_LIMIT )
			{
				query_size = FUZZER_LOOKUP_STRING_LIMIT;
			}
			sub_key = NULL;

			libcreg_key_get_sub_key_by_utf8_name(
			 key,
			 data,
			 query_size,
			 &sub_key,
			 NULL );

			if( sub_key != NULL )
			{
				libcreg_key_free(
				 &sub_key,
				 NULL );
			}
			sub_key = NULL;

			libcreg_key_get_sub_key_by_utf8_path(
			 key,
			 data,
			 query_size,
			 &sub_key,
			 NULL );

			if( sub_key != NULL )
			{
				libcreg_key_free(
				 &sub_key,
				 NULL );
			}
			value = NULL;

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
				libcreg_value_free(
				 &value,
				 NULL );
			}
			value = NULL;

			libcreg_key_get_value_by_utf8_name(
			 key,
			 data,
			 query_size,
			 &value,
			 NULL );

			if( value != NULL )
			{
				libcreg_value_free(
				 &value,
				 NULL );
			}
			if( query_size >= 2 )
			{
				sub_key = NULL;

				libcreg_key_get_sub_key_by_utf16_name(
				 key,
				 ( const uint16_t * ) data,
				 ( query_size / 2 ),
				 &sub_key,
				 NULL );

				if( sub_key != NULL )
				{
					libcreg_key_free(
					 &sub_key,
					 NULL );
				}
				sub_key = NULL;

				libcreg_key_get_sub_key_by_utf16_path(
				 key,
				 ( const uint16_t * ) data,
				 ( query_size / 2 ),
				 &sub_key,
				 NULL );

				if( sub_key != NULL )
				{
					libcreg_key_free(
					 &sub_key,
					 NULL );
				}
				value = NULL;

				libcreg_key_get_value_by_utf16_name(
				 key,
				 ( const uint16_t * ) data,
				 ( query_size / 2 ),
				 &value,
				 NULL );

				if( value != NULL )
				{
					libcreg_value_free(
					 &value,
					 NULL );
				}
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
				libcreg_key_is_corrupted(
				 sub_key,
				 NULL );

				libcreg_key_get_offset(
				 sub_key,
				 &offset,
				 NULL );

				libcreg_key_get_name_size(
				 sub_key,
				 &name_size,
				 NULL );

				libcreg_key_get_name(
				 sub_key,
				 name,
				 2048,
				 NULL );

				libcreg_key_get_utf8_name_size(
				 sub_key,
				 &name_size,
				 NULL );

				libcreg_key_get_utf8_name(
				 sub_key,
				 utf8_string,
				 8192,
				 NULL );

				libcreg_key_get_utf16_name_size(
				 sub_key,
				 &name_size,
				 NULL );

				libcreg_key_get_utf16_name(
				 sub_key,
				 utf16_string,
				 4096,
				 NULL );

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
						libcreg_value_free(
						 &value,
						 NULL );
					}
					if( query_size >= 2 )
					{
						value = NULL;

						libcreg_key_get_value_by_utf16_name(
						 sub_key,
						 ( const uint16_t * ) data,
						 ( query_size / 2 ),
						 &value,
						 NULL );

						if( value != NULL )
						{
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
