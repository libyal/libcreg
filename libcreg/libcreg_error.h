/*
 * Error functions
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

#if !defined( _LIBCREG_INTERNAL_ERROR_H )
#define _LIBCREG_INTERNAL_ERROR_H

#include <common.h>
#include <file_stream.h>
#include <types.h>

#if !defined( HAVE_LOCAL_LIBCREG )
#include <libcreg/error.h>
#endif

#include "libcreg_extern.h"

#if defined( __cplusplus )
extern "C" {
#endif

#if !defined( HAVE_LOCAL_LIBCREG )

LIBCREG_EXTERN \
void libcreg_error_free(
      libcreg_error_t **error );

LIBCREG_EXTERN \
int libcreg_error_fprint(
     libcreg_error_t *error,
     FILE *stream );

LIBCREG_EXTERN \
int libcreg_error_sprint(
     libcreg_error_t *error,
     char *string,
     size_t size );

LIBCREG_EXTERN \
int libcreg_error_backtrace_fprint(
     libcreg_error_t *error,
     FILE *stream );

LIBCREG_EXTERN \
int libcreg_error_backtrace_sprint(
     libcreg_error_t *error,
     char *string,
     size_t size );

#endif /* !defined( HAVE_LOCAL_LIBCREG ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_INTERNAL_ERROR_H ) */

