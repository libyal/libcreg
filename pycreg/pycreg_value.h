/*
 * Python object wrapper of libcreg_value_t
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

#if !defined( _PYCREG_VALUE_H )
#define _PYCREG_VALUE_H

#include <common.h>
#include <types.h>

#include "pycreg_libcreg.h"
#include "pycreg_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pycreg_value pycreg_value_t;

struct pycreg_value
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libcreg value
	 */
	libcreg_value_t *value;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pycreg_value_object_methods[];
extern PyTypeObject pycreg_value_type_object;

PyObject *pycreg_value_new(
           libcreg_value_t *value,
           PyObject *parent_object );

int pycreg_value_init(
     pycreg_value_t *pycreg_value );

void pycreg_value_free(
      pycreg_value_t *pycreg_value );

PyObject *pycreg_value_is_corrupted(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_offset(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_name(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_type(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_data_size(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_data(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_data_as_integer(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

PyObject *pycreg_value_get_data_as_string(
           pycreg_value_t *pycreg_value,
           PyObject *arguments );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_VALUE_H ) */

