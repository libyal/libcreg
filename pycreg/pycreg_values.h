/*
 * Python object definition of the values sequence and iterator
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

#if !defined( _PYCREG_VALUES_H )
#define _PYCREG_VALUES_H

#include <common.h>
#include <types.h>

#include "pycreg_key.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pycreg_values pycreg_values_t;

struct pycreg_values
{
	/* Python object initialization
	 */
	PyObject_HEAD

	/* The pycreg key object
	 */
	pycreg_key_t *key_object;

	/* The get value by index callback function
	 */
	PyObject* (*get_value_by_index)(
	             pycreg_key_t *key_object,
	             int value_index );

	/* The (current) value index
	 */
	int value_index;

	/* The number of values
	 */
	int number_of_values;
};

extern PyTypeObject pycreg_values_type_object;

PyObject *pycreg_values_new(
           pycreg_key_t *key_object,
           PyObject* (*get_value_by_index)(
                        pycreg_key_t *key_object,
                        int value_index ),
           int number_of_values );

int pycreg_values_init(
     pycreg_values_t *pycreg_values );

void pycreg_values_free(
      pycreg_values_t *pycreg_values );

Py_ssize_t pycreg_values_len(
            pycreg_values_t *pycreg_values );

PyObject *pycreg_values_getitem(
           pycreg_values_t *pycreg_values,
           Py_ssize_t item_index );

PyObject *pycreg_values_iter(
           pycreg_values_t *pycreg_values );

PyObject *pycreg_values_iternext(
           pycreg_values_t *pycreg_values );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_VALUES_H ) */

