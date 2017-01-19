/*
 * Python object definition of the keys sequence and iterator
 *
 * Copyright (C) 2013-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYCREG_KEYS_H )
#define _PYCREG_KEYS_H

#include <common.h>
#include <types.h>

#include "pycreg_key.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pycreg_keys pycreg_keys_t;

struct pycreg_keys
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The pycreg key object
	 */
	pycreg_key_t *key_object;

	/* The get sub key by index callback function
	 */
	PyObject* (*get_sub_key_by_index)(
	             pycreg_key_t *key_object,
	             int sub_key_index );

	/* The (current) sub key index
	 */
	int sub_key_index;

	/* The number of sub keys
	 */
	int number_of_sub_keys;
};

extern PyTypeObject pycreg_keys_type_object;

PyObject *pycreg_keys_new(
           pycreg_key_t *key_object,
           PyObject* (*get_sub_key_by_index)(
                        pycreg_key_t *key_object,
                        int sub_key_index ),
           int number_of_sub_keys );

int pycreg_keys_init(
     pycreg_keys_t *pycreg_keys );

void pycreg_keys_free(
      pycreg_keys_t *pycreg_keys );

Py_ssize_t pycreg_keys_len(
            pycreg_keys_t *pycreg_keys );

PyObject *pycreg_keys_getitem(
           pycreg_keys_t *pycreg_keys,
           Py_ssize_t item_index );

PyObject *pycreg_keys_iter(
           pycreg_keys_t *pycreg_keys );

PyObject *pycreg_keys_iternext(
           pycreg_keys_t *pycreg_keys );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_KEYS_H ) */

