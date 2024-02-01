/*
 * Key item functions
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

#if !defined( _LIBCREG_KEY_ITEM_H )
#define _LIBCREG_KEY_ITEM_H

#include <common.h>
#include <types.h>

#include "libcreg_key_descriptor.h"
#include "libcreg_key_name_entry.h"
#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcdata.h"
#include "libcreg_libcerror.h"
#include "libcreg_value_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_key_item libcreg_key_item_t;

struct libcreg_key_item
{
	/* The key name entry
	 */
	libcreg_key_name_entry_t *key_name_entry;

	/* The sub key descriptors array
	 */
	libcdata_array_t *sub_key_descriptors_array;

	/* The sub key range list
	 */
	libcdata_range_list_t *sub_key_range_list;

	/* Various item flags
	 */
	uint8_t item_flags;
};

int libcreg_key_item_initialize(
     libcreg_key_item_t **key_item,
     libcerror_error_t **error );

int libcreg_key_item_free(
     libcreg_key_item_t **key_item,
     libcerror_error_t **error );

int libcreg_key_item_read(
     libcreg_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     off64_t key_offset,
     libcerror_error_t **error );

int libcreg_key_item_is_corrupted(
     libcreg_key_item_t *key_item,
     libcerror_error_t **error );

int libcreg_key_item_get_name_size(
     libcreg_key_item_t *key_item,
     size_t *name_size,
     libcerror_error_t **error );

int libcreg_key_item_get_name(
     libcreg_key_item_t *key_item,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libcreg_key_item_get_utf8_name_size(
     libcreg_key_item_t *key_item,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_item_get_utf8_name(
     libcreg_key_item_t *key_item,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_item_get_utf16_name_size(
     libcreg_key_item_t *key_item,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_item_get_utf16_name(
     libcreg_key_item_t *key_item,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_item_get_number_of_value_entries(
     libcreg_key_item_t *key_item,
     int *number_of_value_entries,
     libcerror_error_t **error );

int libcreg_key_item_get_value_entry_by_index(
     libcreg_key_item_t *key_item,
     int value_index,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error );

int libcreg_key_item_get_value_by_utf8_name(
     libcreg_key_item_t *key_item,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error );

int libcreg_key_item_get_value_by_utf16_name(
     libcreg_key_item_t *key_item,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error );

int libcreg_key_item_get_number_of_sub_key_descriptors(
     libcreg_key_item_t *key_item,
     int *number_of_sub_key_descriptors,
     libcerror_error_t **error );

int libcreg_key_item_get_sub_key_descriptor_by_index(
     libcreg_key_item_t *key_item,
     int sub_key_descriptor_index,
     libcreg_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error );

int libcreg_key_item_get_sub_key_descriptor_by_utf8_name(
     libcreg_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcreg_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error );

int libcreg_key_item_get_sub_key_descriptor_by_utf16_name(
     libcreg_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcreg_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_KEY_ITEM_H ) */

