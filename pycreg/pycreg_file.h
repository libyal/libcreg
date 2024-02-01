/*
 * Python object wrapper of libcreg_file_t
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

#if !defined( _PYCREG_FILE_H )
#define _PYCREG_FILE_H

#include <common.h>
#include <types.h>

#include "pycreg_libbfio.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pycreg_file pycreg_file_t;

struct pycreg_file
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libcreg file
	 */
	libcreg_file_t *file;

	/* The libbfio file IO handle
	 */
	libbfio_handle_t *file_io_handle;
};

extern PyMethodDef pycreg_file_object_methods[];
extern PyTypeObject pycreg_file_type_object;

int pycreg_file_init(
     pycreg_file_t *pycreg_file );

void pycreg_file_free(
      pycreg_file_t *pycreg_file );

PyObject *pycreg_file_signal_abort(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

PyObject *pycreg_file_open(
           pycreg_file_t *pycreg_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pycreg_file_open_file_object(
           pycreg_file_t *pycreg_file,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pycreg_file_close(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

PyObject *pycreg_file_is_corrupted(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

PyObject *pycreg_file_get_ascii_codepage(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

int pycreg_file_set_ascii_codepage_from_string(
     pycreg_file_t *pycreg_file,
     const char *codepage_string );

PyObject *pycreg_file_set_ascii_codepage(
           pycreg_file_t *pycreg_file,
           PyObject *arguments,
           PyObject *keywords );

int pycreg_file_set_ascii_codepage_setter(
     pycreg_file_t *pycreg_file,
     PyObject *string_object,
     void *closure );

PyObject *pycreg_file_get_format_version(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

PyObject *pycreg_file_get_type(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

PyObject *pycreg_file_get_root_key(
           pycreg_file_t *pycreg_file,
           PyObject *arguments );

PyObject *pycreg_file_get_key_by_path(
           pycreg_file_t *pycreg_file,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_FILE_H ) */

