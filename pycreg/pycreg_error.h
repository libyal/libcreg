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

#if !defined( _PYCREG_ERROR_H )
#define _PYCREG_ERROR_H

#include <common.h>
#include <types.h>

#include "pycreg_libcerror.h"
#include "pycreg_python.h"

#define PYCREG_ERROR_STRING_SIZE	2048

#if defined( __cplusplus )
extern "C" {
#endif

void pycreg_error_fetch(
      libcerror_error_t **error,
      int error_domain,
      int error_code,
      const char *format_string,
      ... );

void pycreg_error_fetch_and_raise(
      PyObject *exception_object,
      const char *format_string,
      ... );

void pycreg_error_raise(
      libcerror_error_t *error,
      PyObject *exception_object,
      const char *format_string,
      ... );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_ERROR_H ) */

