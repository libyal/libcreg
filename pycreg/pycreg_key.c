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

#include <common.h>
#include <narrow_string.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pycreg_error.h"
#include "pycreg_integer.h"
#include "pycreg_key.h"
#include "pycreg_keys.h"
#include "pycreg_libcerror.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"
#include "pycreg_unused.h"
#include "pycreg_value.h"
#include "pycreg_values.h"

PyMethodDef pycreg_key_object_methods[] = {

	{ "is_corrupted",
	  (PyCFunction) pycreg_key_is_corrupted,
	  METH_NOARGS,
	  "is_corrupted() -> Boolean\n"
	  "\n"
	  "Indicates if the key is corrupted." },

	{ "get_offset",
	  (PyCFunction) pycreg_key_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the offset." },

	{ "get_name",
	  (PyCFunction) pycreg_key_get_name,
	  METH_NOARGS,
	  "get_name() -> Unicode string\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_number_of_values",
	  (PyCFunction) pycreg_key_get_number_of_values,
	  METH_NOARGS,
	  "get_number_of_values() -> Integer\n"
	  "\n"
	  "Retrieves the number of values." },

	{ "get_value",
	  (PyCFunction) pycreg_key_get_value,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value(value_index) -> Object\n"
	  "\n"
	  "Retrieves the value specified by the index." },

	{ "get_value_by_name",
	  (PyCFunction) pycreg_key_get_value_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_value_by_name(name) -> Object or None\n"
	  "\n"
	  "Retrieves the value specified by the name.\n"
	  "Use an empty string to retrieve the default value." },

	{ "get_number_of_sub_keys",
	  (PyCFunction) pycreg_key_get_number_of_sub_keys,
	  METH_NOARGS,
	  "get_number_of_sub_keys() -> Integer\n"
	  "\n"
	  "Retrieves the number of sub keys." },

	{ "get_sub_key",
	  (PyCFunction) pycreg_key_get_sub_key,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_key(sub_key_index) -> Object\n"
	  "\n"
	  "Retrieves the sub key specified by the index." },

	{ "get_sub_key_by_name",
	  (PyCFunction) pycreg_key_get_sub_key_by_name,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_key_by_name(name) -> Object or None\n"
	  "\n"
	  "Retrieves the sub key specified by the name." },

	{ "get_sub_key_by_path",
	  (PyCFunction) pycreg_key_get_sub_key_by_path,
	  METH_VARARGS | METH_KEYWORDS,
	  "get_sub_key_by_path(path) -> Object or None\n"
	  "\n"
	  "Retrieves the sub key specified by the path." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pycreg_key_object_get_set_definitions[] = {

	{ "corrupted",
	  (getter) pycreg_key_is_corrupted,
	  (setter) 0,
	  "Value to indicate the key is corrupted.",
	  NULL },

	{ "offset",
	  (getter) pycreg_key_get_offset,
	  (setter) 0,
	  "The offset.",
	  NULL },

	{ "name",
	  (getter) pycreg_key_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "number_of_values",
	  (getter) pycreg_key_get_number_of_values,
	  (setter) 0,
	  "The number of values.",
	  NULL },

	{ "values",
	  (getter) pycreg_key_get_values,
	  (setter) 0,
	  "The values.",
	  NULL },

	{ "number_of_sub_keys",
	  (getter) pycreg_key_get_number_of_sub_keys,
	  (setter) 0,
	  "The number of sub keys.",
	  NULL },

	{ "sub_keys",
	  (getter) pycreg_key_get_sub_keys,
	  (setter) 0,
	  "The sub keys.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pycreg_key_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pycreg.key",
	/* tp_basicsize */
	sizeof( pycreg_key_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pycreg_key_free,
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
	0,
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
	Py_TPFLAGS_DEFAULT,
	/* tp_doc */
	"pycreg key object (wraps libcreg_key_t)",
	/* tp_traverse */
	0,
	/* tp_clear */
	0,
	/* tp_richcompare */
	0,
	/* tp_weaklistoffset */
	0,
	/* tp_iter */
	0,
	/* tp_iternext */
	0,
	/* tp_methods */
	pycreg_key_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pycreg_key_object_get_set_definitions,
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
	(initproc) pycreg_key_init,
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

/* Creates a new key object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_new(
           libcreg_key_t *key,
           PyObject *parent_object )
{
	pycreg_key_t *pycreg_key = NULL;
	static char *function    = "pycreg_key_new";

	if( key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	/* PyObject_New does not invoke tp_init
	 */
	pycreg_key = PyObject_New(
	              struct pycreg_key,
	              &pycreg_key_type_object );

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize key.",
		 function );

		goto on_error;
	}
	pycreg_key->key           = key;
	pycreg_key->parent_object = parent_object;

	if( pycreg_key->parent_object != NULL )
	{
		Py_IncRef(
		 pycreg_key->parent_object );
	}
	return( (PyObject *) pycreg_key );

on_error:
	if( pycreg_key != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_key );
	}
	return( NULL );
}

/* Initializes a key object
 * Returns 0 if successful or -1 on error
 */
int pycreg_key_init(
     pycreg_key_t *pycreg_key )
{
	static char *function = "pycreg_key_init";

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	/* Make sure libcreg key is set to NULL
	 */
	pycreg_key->key = NULL;

	PyErr_Format(
	 PyExc_NotImplementedError,
	 "%s: initialize of key not supported.",
	 function );

	return( -1 );
}

/* Frees a key object
 */
void pycreg_key_free(
      pycreg_key_t *pycreg_key )
{
	struct _typeobject *ob_type = NULL;
	libcerror_error_t *error    = NULL;
	static char *function       = "pycreg_key_free";
	int result                  = 0;

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pycreg_key );

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
	if( pycreg_key->key != NULL )
	{
		Py_BEGIN_ALLOW_THREADS

		result = libcreg_key_free(
		          &( pycreg_key->key ),
		          &error );

		Py_END_ALLOW_THREADS

		if( result != 1 )
		{
			pycreg_error_raise(
			 error,
			 PyExc_MemoryError,
			 "%s: unable to free libcreg key.",
			 function );

			libcerror_error_free(
			 &error );
		}
	}
	if( pycreg_key->parent_object != NULL )
	{
		Py_DecRef(
		 pycreg_key->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pycreg_key );
}

/* Determines if the key is corrupted
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_is_corrupted(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pycreg_key_is_corrupted";
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_is_corrupted(
	          pycreg_key->key,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if key is corrupted.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( result != 0 )
	{
		Py_IncRef(
		 (PyObject *) Py_True );

		return( Py_True );
	}
	Py_IncRef(
	 (PyObject *) Py_False );

	return( Py_False );
}

/* Retrieves the offset
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_offset(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pycreg_key_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_offset(
	          pycreg_key->key,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve offset.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	integer_object = pycreg_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_name(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	PyObject *string_object  = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pycreg_key_get_name";
	char *utf8_string        = NULL;
	size_t utf8_string_size  = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_utf8_name_size(
	          pycreg_key->key,
	          &utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine size of name as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( utf8_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	utf8_string = (char *) PyMem_Malloc(
	                        sizeof( char ) * utf8_string_size );

	if( utf8_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create UTF-8 string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_utf8_name(
	          pycreg_key->key,
	          (uint8_t *) utf8_string,
	          utf8_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name as UTF-8 string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8 otherwise it makes
	 * the end of string character is part of the string.
	 */
	string_object = PyUnicode_DecodeUTF8(
	                 utf8_string,
	                 (Py_ssize_t) utf8_string_size - 1,
	                 NULL );

	if( string_object == NULL )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: unable to convert UTF-8 string into Unicode object.",
		 function );

		goto on_error;
	}
	PyMem_Free(
	 utf8_string );

	return( string_object );

on_error:
	if( utf8_string != NULL )
	{
		PyMem_Free(
		 utf8_string );
	}
	return( NULL );
}

/* Retrieves the number of values
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_number_of_values(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pycreg_key_get_number_of_values";
	int number_of_values     = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_number_of_values(
	          pycreg_key->key,
	          &number_of_values,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of values.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_values );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_values );
#endif
	return( integer_object );
}

/* Retrieves a specific value by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_value_by_index(
           PyObject *pycreg_key,
           int value_index )
{
	PyObject *value_object   = NULL;
	libcerror_error_t *error = NULL;
	libcreg_value_t *value   = NULL;
	static char *function    = "pycreg_key_get_value_by_index";
	int result               = 0;

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_value_by_index(
	          ( (pycreg_key_t *) pycreg_key )->key,
	          value_index,
	          &value,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value: %d.",
		 function,
		 value_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	value_object = pycreg_value_new(
	                value,
	                pycreg_key );

	if( value_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create value object.",
		 function );

		goto on_error;
	}
	return( value_object );

on_error:
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific value
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_value(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *value_object      = NULL;
	static char *keyword_list[] = { "value_index", NULL };
	int value_index             = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &value_index ) == 0 )
	{
		return( NULL );
	}
	value_object = pycreg_key_get_value_by_index(
	                (PyObject *) pycreg_key,
	                value_index );

	return( value_object );
}

/* Retrieves a sequence and iterator object for the values
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_values(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pycreg_key_get_values";
	int number_of_values      = 0;
	int result                = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_number_of_values(
	          pycreg_key->key,
	          &number_of_values,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of values.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pycreg_values_new(
	                   (PyObject *) pycreg_key,
	                   &pycreg_key_get_value_by_index,
	                   number_of_values );

	if( sequence_object == NULL )
	{
		pycreg_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Retrieves the value specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_value_by_name(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords )
{
	libcerror_error_t *error    = NULL;
	libcreg_value_t *value      = NULL;
	PyObject *value_object      = NULL;
	char *value_name            = NULL;
	static char *keyword_list[] = { "value_name", NULL };
	static char *function       = "pycreg_key_get_value_by_name";
	size_t value_name_length    = 0;
	int result                  = 0;

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &value_name ) == 0 )
	{
		goto on_error;
	}
	value_name_length = narrow_string_length(
	                     value_name );

	if( value_name_length == 0 )
	{
		value_name = NULL;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_value_by_utf8_name(
	           pycreg_key->key,
	           (uint8_t *) value_name,
	           value_name_length,
	           &value,
	           &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Check if the value is present
	 */
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_object = pycreg_value_new(
	                value,
	                (PyObject *) pycreg_key );

	if( value_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create value object.",
		 function );

		goto on_error;
	}
	return( value_object );

on_error:
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the number of sub keys
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_number_of_sub_keys(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	PyObject *integer_object = NULL;
	libcerror_error_t *error = NULL;
	static char *function    = "pycreg_key_get_number_of_sub_keys";
	int number_of_sub_keys   = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_number_of_sub_keys(
	          pycreg_key->key,
	          &number_of_sub_keys,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
#if PY_MAJOR_VERSION >= 3
	integer_object = PyLong_FromLong(
	                  (long) number_of_sub_keys );
#else
	integer_object = PyInt_FromLong(
	                  (long) number_of_sub_keys );
#endif
	return( integer_object );
}

/* Retrieves a specific sub key by index
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_sub_key_by_index(
           PyObject *pycreg_key,
           int sub_key_index )
{
	PyObject *key_object     = NULL;
	libcerror_error_t *error = NULL;
	libcreg_key_t *sub_key   = NULL;
	static char *function    = "pycreg_key_get_sub_key_by_index";
	int result               = 0;

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_sub_key_by_index(
	          ( (pycreg_key_t *) pycreg_key )->key,
	          sub_key_index,
	          &sub_key,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub key: %d.",
		 function,
		 sub_key_index );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	key_object = pycreg_key_new(
	              sub_key,
	              ( (pycreg_key_t *) pycreg_key )->parent_object );

	if( key_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub key object.",
		 function );

		goto on_error;
	}
	return( key_object );

on_error:
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	return( NULL );
}

/* Retrieves a specific sub key
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_sub_key(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *key_object        = NULL;
	static char *keyword_list[] = { "sub_key_index", NULL };
	int sub_key_index           = 0;

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "i",
	     keyword_list,
	     &sub_key_index ) == 0 )
	{
		return( NULL );
	}
	key_object = pycreg_key_get_sub_key_by_index(
	              (PyObject *) pycreg_key,
	              sub_key_index );

	return( key_object );
}

/* Retrieves a sequence and iterator object for the sub keys
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_sub_keys(
           pycreg_key_t *pycreg_key,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	PyObject *sequence_object = NULL;
	libcerror_error_t *error  = NULL;
	static char *function     = "pycreg_key_get_sub_keys";
	int number_of_sub_keys    = 0;
	int result                = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_number_of_sub_keys(
	          pycreg_key->key,
	          &number_of_sub_keys,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	sequence_object = pycreg_keys_new(
	                   (PyObject *) pycreg_key,
	                   &pycreg_key_get_sub_key_by_index,
	                   number_of_sub_keys );

	if( sequence_object == NULL )
	{
		pycreg_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to create sequence object.",
		 function );

		return( NULL );
	}
	return( sequence_object );
}

/* Retrieves the sub key specified by the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_sub_key_by_name(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *key_object        = NULL;
	libcerror_error_t *error    = NULL;
	libcreg_key_t *sub_key      = NULL;
	static char *function       = "pycreg_key_get_sub_key_by_name";
	static char *keyword_list[] = { "name", NULL };
	char *utf8_name             = NULL;
	size_t utf8_name_length     = 0;
	int result                  = 0;

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_name ) == 0 )
	{
		goto on_error;
	}
	utf8_name_length = narrow_string_length(
	                    utf8_name );

	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_sub_key_by_utf8_name(
	          pycreg_key->key,
	          (uint8_t *) utf8_name,
	          utf8_name_length,
	          &sub_key,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub key.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	key_object = pycreg_key_new(
	              sub_key,
	              pycreg_key->parent_object );

	if( key_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub key object.",
		 function );

		goto on_error;
	}
	return( key_object );

on_error:
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	return( NULL );
}

/* Retrieves the sub key specified by the path
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_key_get_sub_key_by_path(
           pycreg_key_t *pycreg_key,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *key_object        = NULL;
	libcerror_error_t *error    = NULL;
	libcreg_key_t *sub_key      = NULL;
	static char *function       = "pycreg_key_get_sub_key_by_path";
	static char *keyword_list[] = { "path", NULL };
	char *utf8_path             = NULL;
	size_t utf8_path_length     = 0;
	int result                  = 0;

	if( pycreg_key == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid key.",
		 function );

		return( NULL );
	}
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "s",
	     keyword_list,
	     &utf8_path ) == 0 )
	{
		goto on_error;
	}
	utf8_path_length = narrow_string_length(
	                    utf8_path );

	Py_BEGIN_ALLOW_THREADS

	result = libcreg_key_get_sub_key_by_utf8_path(
	          pycreg_key->key,
	          (uint8_t *) utf8_path,
	          utf8_path_length,
	          &sub_key,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve sub key.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( result == 0 )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	key_object = pycreg_key_new(
	              sub_key,
	              pycreg_key->parent_object );

	if( key_object == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create sub key object.",
		 function );

		goto on_error;
	}
	return( key_object );

on_error:
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	return( NULL );
}

