/*
 * Key name entry functions
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

#if !defined( _LIBCREG_KEY_NAME_ENTRY_H )
#define _LIBCREG_KEY_NAME_ENTRY_H

#include <common.h>
#include <types.h>

#include "libcreg_libcdata.h"
#include "libcreg_libcerror.h"
#include "libcreg_value_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_key_name_entry libcreg_key_name_entry_t;

struct libcreg_key_name_entry
{
	/* The offset
	 */
	uint32_t offset;

	/* The size
	 */
	uint32_t size;

	/* The index
	 */
	uint16_t index;

	/* The name hash
	 */
	uint32_t name_hash;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The value entries array
	 */
	libcdata_array_t *entries_array;

	/* Various flags
	 */
	uint8_t flags;
};

int libcreg_key_name_entry_initialize(
     libcreg_key_name_entry_t **key_name_entry,
     libcerror_error_t **error );

int libcreg_key_name_entry_free(
     libcreg_key_name_entry_t **key_name_entry,
     libcerror_error_t **error );

int libcreg_key_name_entry_read_data(
     libcreg_key_name_entry_t *key_name_entry,
     const uint8_t *data,
     size_t data_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_read_values(
     libcreg_key_name_entry_t *key_name_entry,
     int number_of_values,
     uint32_t file_offset,
     const uint8_t *value_entries_data,
     size_t value_entries_data_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_name_size(
     libcreg_key_name_entry_t *key_name_entry,
     size_t *name_size,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_name(
     libcreg_key_name_entry_t *key_name_entry,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_utf8_name_size(
     libcreg_key_name_entry_t *key_name_entry,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_utf8_name(
     libcreg_key_name_entry_t *key_name_entry,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_utf16_name_size(
     libcreg_key_name_entry_t *key_name_entry,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_utf16_name(
     libcreg_key_name_entry_t *key_name_entry,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_number_of_entries(
     libcreg_key_name_entry_t *key_name_entry,
     int *number_of_entries,
     libcerror_error_t **error );

int libcreg_key_name_entry_get_entry_by_index(
     libcreg_key_name_entry_t *key_name_entry,
     int entry_index,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error );

int libcreg_key_name_entry_compare_name_with_utf8_string(
     libcreg_key_name_entry_t *key_name_entry,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_key_name_entry_compare_name_with_utf16_string(
     libcreg_key_name_entry_t *key_name_entry,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_KEY_NAME_ENTRY_H ) */

