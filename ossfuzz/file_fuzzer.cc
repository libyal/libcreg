/*
 * OSS-Fuzz target for libcreg file type
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

/* Limits the size of the paths used to look up keys
 */
#define FUZZER_PATH_SIZE_LIMIT  ( (size_t) 0x4000 )

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
	libbfio_handle_t *file_io_handle = NULL;
	libcreg_file_t *file             = NULL;
	libcreg_key_t *key               = NULL;
	size_t query_size                = 0;
	uint32_t file_type               = 0;
	uint16_t major_version           = 0;
	uint16_t minor_version           = 0;
	int ascii_codepage               = 0;
	int codepage_index               = 0;

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
	libcreg_file_get_type(
	 file,
	 &file_type,
	 NULL );

	libcreg_file_get_ascii_codepage(
	 file,
	 &ascii_codepage,
	 NULL );

	libcreg_file_get_format_version(
	 file,
	 &major_version,
	 &minor_version,
	 NULL );

	libcreg_file_is_corrupted(
	 file,
	 NULL );

	/* Cycles through the supported ASCII codepages to exercise the
	 * codepage handling in the library
	 */
	for( codepage_index = 0;
	     codepage_index < number_of_ascii_codepages;
	     codepage_index++ )
	{
		libcreg_file_set_ascii_codepage(
		 file,
		 ascii_codepages[ codepage_index ],
		 NULL );

		libcreg_file_get_ascii_codepage(
		 file,
		 &ascii_codepage,
		 NULL );
	}
	/* Sets an unsupported codepage to exercise the error path
	 */
	libcreg_file_set_ascii_codepage(
	 file,
	 65001,
	 NULL );
#if defined( LIBCREG_HAVE_BFIO )
	libcreg_check_file_signature_file_io_handle(
	 file_io_handle,
	 NULL );
#endif /* defined( LIBCREG_HAVE_BFIO ) */
	/* Looks up keys using paths derived from the fuzz data.
	 * The data is not necessarily valid UTF-8 or UTF-16
	 * which exercises the validation and conversion error
	 * paths in the library
	 */
	if( size > 0 )
	{
		query_size = size;

		if( query_size > FUZZER_PATH_SIZE_LIMIT )
		{
			query_size = FUZZER_PATH_SIZE_LIMIT;
		}
		key = NULL;

		libcreg_file_get_key_by_utf8_path(
		 file,
		 data,
		 query_size,
		 &key,
		 NULL );

		if( key != NULL )
		{
			libcreg_key_free(
			 &key,
			 NULL );
		}
		if( query_size >= 2 )
		{
			key = NULL;

			libcreg_file_get_key_by_utf16_path(
			 file,
			 ( const uint16_t * ) data,
			 ( query_size / 2 ),
			 &key,
			 NULL );

			if( key != NULL )
			{
				libcreg_key_free(
				 &key,
				 NULL );
			}
		}
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
