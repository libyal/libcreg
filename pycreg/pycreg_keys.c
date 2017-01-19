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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pycreg_key.h"
#include "pycreg_keys.h"
#include "pycreg_libcerror.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"

PySequenceMethods pycreg_keys_sequence_methods = {
	/* sq_length */
	(lenfunc) pycreg_keys_len,
	/* sq_concat */
	0,
	/* sq_repeat */
	0,
	/* sq_item */
	(ssizeargfunc) pycreg_keys_getitem,
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

PyTypeObject pycreg_keys_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pycreg._keys",
	/* tp_basicsize */
	sizeof( pycreg_keys_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pycreg_keys_free,
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
	&pycreg_keys_sequence_methods,
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
	"internal pycreg keys sequence and iterator object",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	(getiterfunc) pycreg_keys_iter,
	/* tp_iternext */
	(iternextfunc) pycreg_keys_iternext,
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
	(initproc) pycreg_keys_init,
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

/* Creates a new keys object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_keys_new(
           pycreg_key_t *key_object,
           PyObject* (*get_sub_key_by_index)(
                        pycreg_key_t *key_object,
                        int sub_key_index ),
           int number_of_sub_keys )
{
	pycreg_keys_t *pycreg_keys = NULL;
	static char *function      = "pycreg_keys_new";

	if( key_object == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key object.",
		 function );

		return( NULL );
	}
	if( get_sub_key_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid get sub key by index function.",
		 function );

		return( NULL );
	}
	/* Make sure the keys values are initialized
	 */
	pycreg_keys = PyObject_New(
	               struct pycreg_keys,
	               &pycreg_keys_type_object );

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize keys.",
		 function );

		goto on_error;
	}
	if( pycreg_keys_init(
	     pycreg_keys ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize keys.",
		 function );

		goto on_error;
	}
	pycreg_keys->key_object           = key_object;
	pycreg_keys->get_sub_key_by_index = get_sub_key_by_index;
	pycreg_keys->number_of_sub_keys   = number_of_sub_keys;

	Py_IncRef(
	 (PyObject *) pycreg_keys->key_object );

	return( (PyObject *) pycreg_keys );

on_error:
	if( pycreg_keys != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_keys );
	}
	return( NULL );
}

/* Intializes a keys object
 * Returns 0 if successful or -1 on error
 */
int pycreg_keys_init(
     pycreg_keys_t *pycreg_keys )
{
	static char *function = "pycreg_keys_init";

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys.",
		 function );

		return( -1 );
	}
	/* Make sure the keys values are initialized
	 */
	pycreg_keys->key_object           = NULL;
	pycreg_keys->get_sub_key_by_index = NULL;
	pycreg_keys->sub_key_index        = 0;
	pycreg_keys->number_of_sub_keys   = 0;

	return( 0 );
}

/* Frees a keys object
 */
void pycreg_keys_free(
      pycreg_keys_t *pycreg_keys )
{
	struct _typeobject *ob_type = NULL;
	static char *function       = "pycreg_keys_free";

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pycreg_keys );

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
	if( pycreg_keys->key_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_keys->key_object );
	}
	ob_type->tp_free(
	 (PyObject*) pycreg_keys );
}

/* The keys len() function
 */
Py_ssize_t pycreg_keys_len(
            pycreg_keys_t *pycreg_keys )
{
	static char *function = "pycreg_keys_len";

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys.",
		 function );

		return( -1 );
	}
	return( (Py_ssize_t) pycreg_keys->number_of_sub_keys );
}

/* The keys getitem() function
 */
PyObject *pycreg_keys_getitem(
           pycreg_keys_t *pycreg_keys,
           Py_ssize_t item_index )
{
	PyObject *key_object  = NULL;
	static char *function = "pycreg_keys_getitem";

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys.",
		 function );

		return( NULL );
	}
	if( pycreg_keys->get_sub_key_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys - missing get sub key by index function.",
		 function );

		return( NULL );
	}
	if( pycreg_keys->number_of_sub_keys < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys - invalid number of sub keys.",
		 function );

		return( NULL );
	}
	if( ( item_index < 0 )
	 || ( item_index >= (Py_ssize_t) pycreg_keys->number_of_sub_keys ) )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid invalid item index value out of bounds.",
		 function );

		return( NULL );
	}
	key_object = pycreg_keys->get_sub_key_by_index(
	              pycreg_keys->key_object,
	              (int) item_index );

	return( key_object );
}

/* The keys iter() function
 */
PyObject *pycreg_keys_iter(
           pycreg_keys_t *pycreg_keys )
{
	static char *function = "pycreg_keys_iter";

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys.",
		 function );

		return( NULL );
	}
	Py_IncRef(
	 (PyObject *) pycreg_keys );

	return( (PyObject *) pycreg_keys );
}

/* The keys iternext() function
 */
PyObject *pycreg_keys_iternext(
           pycreg_keys_t *pycreg_keys )
{
	PyObject *key_object  = NULL;
	static char *function = "pycreg_keys_iternext";

	if( pycreg_keys == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys.",
		 function );

		return( NULL );
	}
	if( pycreg_keys->get_sub_key_by_index == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys - missing get sub key by index function.",
		 function );

		return( NULL );
	}
	if( pycreg_keys->sub_key_index < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys - invalid sub key index.",
		 function );

		return( NULL );
	}
	if( pycreg_keys->number_of_sub_keys < 0 )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid keys - invalid number of sub keys.",
		 function );

		return( NULL );
	}
	if( pycreg_keys->sub_key_index >= pycreg_keys->number_of_sub_keys )
	{
		PyErr_SetNone(
		 PyExc_StopIteration );

		return( NULL );
	}
	key_object = pycreg_keys->get_sub_key_by_index(
	              pycreg_keys->key_object,
	              pycreg_keys->sub_key_index );

	if( key_object != NULL )
	{
		pycreg_keys->sub_key_index++;
	}
	return( key_object );
}

