/*
 * Item functions
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

#if !defined( _LIBCREG_INTERNAL_KEY_H )
#define _LIBCREG_INTERNAL_KEY_H

#include <common.h>
#include <types.h>

#include "libcreg_extern.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_item.h"
#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcthreads.h"
#include "libcreg_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_internal_key libcreg_internal_key_t;

struct libcreg_internal_key
{
	/* The IO handle
	 */
	libcreg_io_handle_t *io_handle;

	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The key offset
	 */
	off64_t key_offset;

	/* The key item
	 */
	libcreg_key_item_t *key_item;

	/* The key navigation
	 */
	libcreg_key_navigation_t *key_navigation;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

int libcreg_key_initialize(
     libcreg_key_t **key,
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     uint32_t key_offset,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_free(
     libcreg_key_t **key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_is_corrupted(
     libcreg_key_t *key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_offset(
     libcreg_key_t *key,
     off64_t *offset,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_name_size(
     libcreg_key_t *key,
     size_t *string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_name(
     libcreg_key_t *key,
     uint8_t *string,
     size_t string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_utf8_name_size(
     libcreg_key_t *key,
     size_t *utf8_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_utf8_name(
     libcreg_key_t *key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_utf16_name_size(
     libcreg_key_t *key,
     size_t *utf16_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_utf16_name(
     libcreg_key_t *key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_number_of_values(
     libcreg_key_t *key,
     int *number_of_values,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_value_by_index(
     libcreg_key_t *key,
     int value_index,
     libcreg_value_t **value,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_value_by_utf8_name(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_value_t **value,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_value_by_utf16_name(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_value_t **value,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_number_of_sub_keys(
     libcreg_key_t *key,
     int *number_of_sub_keys,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_index(
     libcreg_key_t *key,
     int sub_key_index,
     libcreg_key_t **sub_key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf8_name(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf8_path(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf16_name(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf16_path(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_INTERNAL_KEY_H ) */

