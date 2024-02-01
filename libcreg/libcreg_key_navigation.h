/*
 * Key navigation functions
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

#if !defined( _LIBCREG_KEY_NAVIGATION_H )
#define _LIBCREG_KEY_NAVIGATION_H

#include <common.h>
#include <types.h>

#include "libcreg_data_block.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_hierarchy_entry.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libfcache.h"
#include "libcreg_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_key_navigation libcreg_key_navigation_t;

struct libcreg_key_navigation
{
	/* The IO handle
	 */
	libcreg_io_handle_t *io_handle;

	/* The key hierarchy area
	 */
	libfdata_area_t *key_hierarchy_area;

	/* The key hierarchy cache
	 */
	libfcache_cache_t *key_hierarchy_cache;

	/* The data blocks list
	 */
	libfdata_list_t *data_blocks_list;

	/* The data blocks cache
	 */
	libfcache_cache_t *data_blocks_cache;
};

int libcreg_key_navigation_initialize(
     libcreg_key_navigation_t **key_navigation,
     libcreg_io_handle_t *io_handle,
     libcerror_error_t **error );

int libcreg_key_navigation_free(
     libcreg_key_navigation_t **key_navigation,
     libcerror_error_t **error );

int libcreg_key_navigation_read_file_io_handle(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

int libcreg_key_navigation_read_data_blocks(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t file_size,
     libcerror_error_t **error );

int libcreg_key_navigation_get_key_hierarchy_entry_at_offset(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     off64_t key_hierarchy_entry_offset,
     libcreg_key_hierarchy_entry_t **key_hierarchy_entry,
     libcerror_error_t **error );

int libcreg_key_navigation_get_number_of_data_blocks(
     libcreg_key_navigation_t *key_navigation,
     int *number_of_data_blocks,
     libcerror_error_t **error );

int libcreg_key_navigation_get_data_block_at_index(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     int data_block_index,
     libcreg_data_block_t **data_block,
     libcerror_error_t **error );

int libcreg_key_navigation_read_data_block_element_data(
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfdata_cache_t *cache,
     int data_range_file_index,
     off64_t data_range_offset,
     size64_t data_range_size,
     uint32_t data_range_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_KEY_NAVIGATION_H ) */

