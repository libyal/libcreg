/*
 * Python object definition of the libcreg value types
 *
 * Copyright (C) 2013-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _PYCREG_VALUE_TYPES_H )
#define _PYCREG_VALUE_TYPES_H

#include <common.h>
#include <types.h>

#include "pycreg_libcreg.h"
#include "pycreg_python.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct pycreg_value_types pycreg_value_types_t;

struct pycreg_value_types
{
	/* Python object initialization
	 */
	PyObject_HEAD
};

extern PyTypeObject pycreg_value_types_type_object;

int pycreg_value_types_init_type(
     PyTypeObject *type_object );

PyObject *pycreg_value_types_new(
           void );

int pycreg_value_types_init(
     pycreg_value_types_t *definitions_object );

void pycreg_value_types_free(
      pycreg_value_types_t *definitions_object );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _PYCREG_VALUE_TYPES_H ) */

