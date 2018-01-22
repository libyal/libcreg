/*
 * Support functions
 *
 * Copyright (C) 2013-2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#if !defined( _LIBCREG_SUPPORT_H )
#define _LIBCREG_SUPPORT_H

#include <common.h>
#include <types.h>

#include "libcreg_extern.h"
#include "libcreg_libcerror.h"
#include "libcreg_libbfio.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCREG )

LIBCREG_EXTERN \
const char *libcreg_get_version(
             void );

LIBCREG_EXTERN \
int libcreg_get_access_flags_read(
     void );

LIBCREG_EXTERN \
int libcreg_get_codepage(
     int *codepage,
     libcerror_error_t **error );

LIBCREG_EXTERN \
int libcreg_set_codepage(
     int codepage,
     libcerror_error_t **error );

#endif /* !defined( HAVE_LOCAL_LIBCREG ) */

LIBCREG_EXTERN \
int libcreg_check_file_signature(
     const char *filename,
     libcerror_error_t **error );

#if defined( HAVE_WIDE_CHARACTER_TYPE )

LIBCREG_EXTERN \
int libcreg_check_file_signature_wide(
     const wchar_t *filename,
     libcerror_error_t **error );

#endif /* defined( HAVE_WIDE_CHARACTER_TYPE ) */

LIBCREG_EXTERN \
int libcreg_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_SUPPORT_H ) */

