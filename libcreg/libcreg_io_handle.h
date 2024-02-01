/*
 * Input/Output (IO) handle
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

#if !defined( _LIBCREG_IO_HANDLE_H )
#define _LIBCREG_IO_HANDLE_H

#include <common.h>
#include <types.h>

#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

extern const char *creg_file_signature;

typedef struct libcreg_io_handle libcreg_io_handle_t;

struct libcreg_io_handle
{
	/* The major version
	 */
	uint16_t major_version;

	/* The minor version
	 */
	uint16_t minor_version;

	/* The the data blocks list offset
	 */
	uint32_t data_blocks_list_offset;

	/* The file type
	 */
	uint32_t file_type;

	/* The codepage of the extended ASCII strings
	 */
	int ascii_codepage;

	/* Various flags
	 */
	uint8_t flags;
};

int libcreg_io_handle_initialize(
     libcreg_io_handle_t **io_handle,
     libcerror_error_t **error );

int libcreg_io_handle_free(
     libcreg_io_handle_t **io_handle,
     libcerror_error_t **error );

int libcreg_io_handle_clear(
     libcreg_io_handle_t *io_handle,
     libcerror_error_t **error );

int libcreg_io_handle_read_file_header(
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t *number_of_data_blocks,
     libcerror_error_t **error );

int libcreg_io_handle_read_key_hierarchy_entry(
     intptr_t *data_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_area_t *area,
     libfdata_cache_t *cache,
     off64_t element_value_offset,
     int element_data_file_index,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_IO_HANDLE_H ) */

