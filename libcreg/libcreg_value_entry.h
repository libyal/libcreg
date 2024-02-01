/*
 * Value entry functions
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

#if !defined( _LIBCREG_VALUE_ENTRY_H )
#define _LIBCREG_VALUE_ENTRY_H

#include <common.h>
#include <types.h>

#include "libcreg_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_value_entry libcreg_value_entry_t;

struct libcreg_value_entry
{
	/* The offset
	 */
	uint32_t offset;

	/* The size
	 */
	uint32_t size;

	/* The name hash
	 */
	uint32_t name_hash;

	/* The name
	 */
	uint8_t *name;

	/* The name size
	 */
	uint16_t name_size;

	/* The type
	 */
	uint32_t type;

	/* The data
	 */
	uint8_t *data;

	/* The data size
	 */
	uint16_t data_size;

	/* Various flags
	 */
	uint8_t flags;
};

int libcreg_value_entry_initialize(
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error );

int libcreg_value_entry_free(
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error );

int libcreg_value_entry_read_data(
     libcreg_value_entry_t *value_entry,
     const uint8_t *data,
     size_t data_size,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_value_entry_get_data_size(
     libcreg_value_entry_t *value_entry,
     size_t *data_size,
     libcerror_error_t **error );

int libcreg_value_entry_get_data(
     libcreg_value_entry_t *value_entry,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error );

int libcreg_value_entry_compare_name_with_utf8_string(
     libcreg_value_entry_t *value_entry,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

int libcreg_value_entry_compare_name_with_utf16_string(
     libcreg_value_entry_t *value_entry,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_VALUE_ENTRY_H ) */

