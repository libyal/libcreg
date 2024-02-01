/*
 * Data block functions
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

#if !defined( _LIBCREG_DATA_BLOCK_H )
#define _LIBCREG_DATA_BLOCK_H

#include <common.h>
#include <types.h>

#include "libcreg_key_name_entry.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcdata.h"
#include "libcreg_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_data_block libcreg_data_block_t;

struct libcreg_data_block
{
	/* The offset of the data block
	 */
	off64_t offset;

	/* The size of the data block
	 */
	uint32_t size;

	/* The unused size
	 */
	uint32_t unused_size;

	/* The used size
	 */
	uint32_t used_size;

	/* The data block data
	 */
	uint8_t *data;

	/* The data block data size
	 */
	size_t data_size;

	/* The key name entries array
	 */
	libcdata_array_t *entries_array;
};

int libcreg_data_block_initialize(
     libcreg_data_block_t **data_block,
     libcerror_error_t **error );

int libcreg_data_block_free(
     libcreg_data_block_t **data_block,
     libcerror_error_t **error );

int libcreg_data_block_read_header(
     libcreg_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error );

int libcreg_data_block_read_entries(
     libcreg_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_data_block_get_number_of_entries(
     libcreg_data_block_t *data_block,
     int *number_of_entries,
     libcerror_error_t **error );

int libcreg_data_block_get_entry_by_identifier(
     libcreg_data_block_t *data_block,
     uint16_t identifier,
     libcreg_key_name_entry_t **key_name_entry,
     int ascii_codepage,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_DATA_BLOCK_H ) */

