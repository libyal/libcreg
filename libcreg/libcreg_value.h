/*
 * Value functions
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

#if !defined( _LIBCREG_INTERNAL_VALUE_H )
#define _LIBCREG_INTERNAL_VALUE_H

#include <common.h>
#include <types.h>

#include "libcreg_extern.h"
#include "libcreg_io_handle.h"
#include "libcreg_libcerror.h"
#include "libcreg_types.h"
#include "libcreg_value_entry.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_internal_value libcreg_internal_value_t;

struct libcreg_internal_value
{
	/* The IO handle
	 */
	libcreg_io_handle_t *io_handle;

	/* The value entry
	 */
	libcreg_value_entry_t *value_entry;
};

int libcreg_value_initialize(
     libcreg_value_t **value,
     libcreg_io_handle_t *io_handle,
     libcreg_value_entry_t *value_entry,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_free(
     libcreg_value_t **value,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_is_corrupted(
     libcreg_value_t *value,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_offset(
     libcreg_value_t *value,
     off64_t *offset,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_name_size(
     libcreg_value_t *value,
     size_t *string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_name(
     libcreg_value_t *value,
     uint8_t *string,
     size_t string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_utf8_name_size(
     libcreg_value_t *value,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_utf8_name(
     libcreg_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_utf16_name_size(
     libcreg_value_t *value,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_utf16_name(
     libcreg_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_type(
     libcreg_value_t *value,
     uint32_t *value_type,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_data_size(
     libcreg_value_t *value,
     size_t *value_data_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_data(
     libcreg_value_t *value,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_32bit(
     libcreg_value_t *value,
     uint32_t *value_32bit,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_64bit(
     libcreg_value_t *value,
     uint64_t *value_64bit,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_utf8_string_size(
     libcreg_value_t *value,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_utf8_string(
     libcreg_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_utf16_string_size(
     libcreg_value_t *value,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_utf16_string(
     libcreg_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_binary_data_size(
     libcreg_value_t *value,
     size_t *size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_value_get_value_binary_data(
     libcreg_value_t *value,
     uint8_t *binary_data,
     size_t size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_INTERNAL_VALUE_H ) */

