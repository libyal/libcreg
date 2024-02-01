/*
 * Python object wrapper of libcreg_key_t
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

#if !defined( _PYCREG_KEY_H )
#define _PYCREG_KEY_H

#include <common.h>
#include <types.h>

#include "pycreg_libcreg.h"
#include "pycreg_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pycreg_key pycreg_key_t;

struct pycreg_key
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The libcreg key
	 */
	libcreg_key_t *key;

	/* The parent object
	 */
	PyObject *parent_object;
};

extern PyMethodDef pycreg_key_object_methods[];
extern PyTypeObject pycreg_key_type_object;

PyObject *pycreg_key_new(
           libcreg_key_t *key,
           PyObject *parent_object );

int pycreg_key_init(
     pycreg_key_t *pycreg_key );

void pycreg_key_free(
      pycreg_key_t *pycreg_key );

PyObject *pycreg_key_is_corrupted(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_offset(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_name(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_number_of_values(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_value_by_index(
           PyObject *pycreg_key,
           int value_index );

PyObject *pycreg_key_get_value(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pycreg_key_get_values(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_value_by_name(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pycreg_key_get_number_of_sub_keys(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_sub_key_by_index(
           PyObject *pycreg_key,
           int sub_key_index );

PyObject *pycreg_key_get_sub_key(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pycreg_key_get_sub_keys(
           pycreg_key_t *pycreg_key,
           PyObject *arguments );

PyObject *pycreg_key_get_sub_key_by_name(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords );

PyObject *pycreg_key_get_sub_key_by_path(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_KEY_H ) */

