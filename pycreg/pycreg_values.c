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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pycreg_key.h"
#include "pycreg_libcerror.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"
#include "pycreg_value.h"
#include "pycreg_values.h"

PySequenceMethods pycreg_values_sequence_methods = {
	/* sq_length */
	(lenfunc) pycreg_values_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pycreg_values_getitem,
	/* sq_slice */
	0,
	/* sq_ass_item */
	0,
	/* sq_ass_slice */
	0,
	/* sq_contains */
	0,
	/* sq_inplace_concat */
	0,
	/* sq_inplace_repeat */
	0
};

PyTypeObject pycreg_values_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pycreg._values",
	/* tp_basicsize */
	sizeof( pycreg_values_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pycreg_values_free,
	/* tp_print */
	0,
	/* tp_getattr */
	0,
	/* tp_setattr */
	0,
	/* tp_compare */
	0,
	/* tp_repr */
	0,
	/* tp_as_number */
	0,
	/* tp_as_sequence */
	&pycreg_values_sequence_methods,
	/* tp_as_mapping */
	0,
	/* tp_hash */
	0,
	/* tp_call */
	0,
	/* tp_str */
	0,
	/* tp_getattro */
	0,
	/* tp_setattro */
	0,
	/* tp_as_buffer */
	0,
	/* tp_flags */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_HAVE_ITER,
	/* tp_doc */
	"internal pycreg values sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pycreg_values_iter,
	/* tp_iternext */
	(iternextfunc) pycreg_values_iternext,
	/* tp_methods */
	0,
	/* tp_members */
	0,
	/* tp_getset */
	0,
	/* tp_base */
	0,
	/* tp_dict */
	0,
	/* tp_descr_get */
	0,
	/* tp_descr_set */
	0,
	/* tp_dictoffset */
	0,
	/* tp_init */
	(initproc) pycreg_values_init,
	/* tp_alloc */
	0,
	/* tp_new */
	0,
	/* tp_free */
	0,
	/* tp_is_gc */
	0,
	/* tp_bases */
	NULL,
	/* tp_mro */
	NULL,
	/* tp_cache */
	NULL,
	/* tp_subclasses */
	NULL,
	/* tp_weaklist */
	NULL,
	/* tp_del */
	0
};

/* Creates a new values object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_values_new(
           pycreg_key_t *key_object,
           PyObject* (*get_value_by_index)(
                        pycreg_key_t *key_object,
                        int value_index ),
           int number_of_values )
{
	pycreg_values_t *pycreg_values = NULL;
	static char *function          = "pycreg_values_new";

	if( key_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key object.",
		 function );

		return( NULL );
	}
	if( get_value_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get value by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the values values are initialized
	 */
	pycreg_values = PyObject_New(
	                 struct pycreg_values,
	                 &pycreg_values_type_object );

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize values.",
		 function );

		goto on_error;
	}
	if( pycreg_values_init(
	     pycreg_values ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize values.",
		 function );

		goto on_error;
	}
	pycreg_values->key_object         = key_object;
	pycreg_values->get_value_by_index = get_value_by_index;
	pycreg_values->number_of_values   = number_of_values;

	Py_IncRef(
	 (PyObject *) pycreg_values->key_object );

	return( (PyObject *) pycreg_values );

on_error:
	if( pycreg_values != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_values );
	}
	return( NULL );
}

/* Intializes a values object
 * Returns 0 if successful or -1 on error
 */
int pycreg_values_init(
     pycreg_values_t *pycreg_values )
{
	static char *function = "pycreg_values_init";

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values.",
		 function );

		return( -1 );
	}
	/* Make sure the values values are initialized
	 */
	pycreg_values->key_object         = NULL;
	pycreg_values->get_value_by_index = NULL;
	pycreg_values->value_index        = 0;
	pycreg_values->number_of_values   = 0;

	return( 0 );
}

/* Frees a values object
 */
void pycreg_values_free(
      pycreg_values_t *pycreg_values )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pycreg_values_free";

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pycreg_values );

	if( ob_type == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: missing ob_type.",
		 function );

		return;
	}
	if( ob_type->tp_free == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid ob_type - missing tp_free.",
		 function );

		return;
	}
	if( pycreg_values->key_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_values->key_object );
	}
	ob_type->tp_free(
	 (PyObject*) pycreg_values );
}

/* The values len() function
 */
Py_ssize_t pycreg_values_len(
            pycreg_values_t *pycreg_values )
{
	static char *function = "pycreg_values_len";

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pycreg_values->number_of_values );
}

/* The values getitem() function
 */
PyObject *pycreg_values_getitem(
           pycreg_values_t *pycreg_values,
           Py_ssize_t item_index )
{
	PyObject *value_object = NULL;
	static char *function  = "pycreg_values_getitem";

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values.",
		 function );

		return( NULL );
	}
	if( pycreg_values->get_value_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values - missing get value by index function.",
		 function );

		return( NULL );
	}
	if( pycreg_values->number_of_values < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values - invalid number of -1ecords.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pycreg_values->number_of_values ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	value_object = pycreg_values->get_value_by_index(
	                pycreg_values->key_object,
	                (int) item_index );

	return( value_object );
}

/* The values iter() function
 */
PyObject *pycreg_values_iter(
           pycreg_values_t *pycreg_values )
{
	static char *function = "pycreg_values_iter";

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pycreg_values );

	return( (PyObject *) pycreg_values );
}

/* The values iternext() function
 */
PyObject *pycreg_values_iternext(
           pycreg_values_t *pycreg_values )
{
	PyObject *value_object = NULL;
	static char *function  = "pycreg_values_iternext";

	if( pycreg_values == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values.",
		 function );

		return( NULL );
	}
	if( pycreg_values->get_value_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values - missing get value by index function.",
		 function );

		return( NULL );
	}
	if( pycreg_values->value_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values - invalid value index.",
		 function );

		return( NULL );
	}
	if( pycreg_values->number_of_values < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid values - invalid number of values.",
		 function );

		return( NULL );
	}
	if( pycreg_values->value_index >= pycreg_values->number_of_values )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	value_object = pycreg_values->get_value_by_index(
	                pycreg_values->key_object,
	                pycreg_values->value_index );

	if( value_object != NULL )
	{
		pycreg_values->value_index++;
	}
	return( value_object );
}

