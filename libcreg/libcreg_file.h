/*
 * File functions
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

#if !defined( _LIBCREG_INTERNAL_FILE_H )
#define _LIBCREG_INTERNAL_FILE_H

#include <common.h>
#include <types.h>

#include "libcreg_extern.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcthreads.h"
#include "libcreg_libfcache.h"
#include "libcreg_types.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_internal_file libcreg_internal_file_t;

struct libcreg_internal_file
{
	/* The file IO handle
	 */
	libbfio_handle_t *file_io_handle;

	/* The IO handle
	 */
	libcreg_io_handle_t *io_handle;

	/* The key navigation
	 */
	libcreg_key_navigation_t *key_navigation;

	/* Value to indicate if the file IO handle was created inside the library
	 */
	uint8_t file_io_handle_created_in_library;

	/* Value to indicate if the file IO handle was opened inside the library
	 */
	uint8_t file_io_handle_opened_in_library;

	/* Value to indicate if abort was signalled
	 */
	int abort;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	/* The read/write lock
	 */
	libcthreads_read_write_lock_t *read_write_lock;
#endif
};

LIBCREG_EXTERN \
int libcreg_file_initialize(
     libcreg_file_t **file,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_free(
     libcreg_file_t **file,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_signal_abort(
     libcreg_file_t *file,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_open(
     libcreg_file_t *file,
     const char *filename,
     int access_flags,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBCREG_EXTERN \
int libcreg_file_open_wide(
     libcreg_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBCREG_EXTERN \
int libcreg_file_open_file_io_handle(
     libcreg_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_close(
     libcreg_file_t *file,
     libcerror_error_t **error );

int libcreg_internal_file_open_read(
     libcreg_internal_file_t *internal_file,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_is_corrupted(
     libcreg_file_t *file,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_get_ascii_codepage(
     libcreg_file_t *file,
     int *ascii_codepage,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_set_ascii_codepage(
     libcreg_file_t *file,
     int ascii_codepage,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_get_format_version(
     libcreg_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_get_type(
     libcreg_file_t *file,
     uint32_t *file_type,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_get_root_key(
     libcreg_file_t *file,
     libcreg_key_t **root_key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_get_key_by_utf8_path(
     libcreg_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **key,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_file_get_key_by_utf16_path(
     libcreg_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **key,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_INTERNAL_FILE_H ) */

