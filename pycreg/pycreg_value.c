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

#include <common.h>
#include <types.h>

#if defined( HAVE_STDLIB_H ) || defined( HAVE_WINAPI )
#include <stdlib.h>
#endif

#include "pycreg_error.h"
#include "pycreg_integer.h"
#include "pycreg_libcerror.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"
#include "pycreg_unused.h"
#include "pycreg_value.h"

PyMethodDef pycreg_value_object_methods[] = {

	/* Functions to access the value values */

	{ "is_corrupted",
	  (PyCFunction) pycreg_value_is_corrupted,
	  METH_NOARGS,
	  "is_corrupted() -> Boolean\n"
	  "\n"
	  "Indicates if the value is corrupted." },

	{ "get_offset",
	  (PyCFunction) pycreg_value_get_offset,
	  METH_NOARGS,
	  "get_offset() -> Integer\n"
	  "\n"
	  "Retrieves the offset." },

	{ "get_name",
	  (PyCFunction) pycreg_value_get_name,
	  METH_NOARGS,
	  "get_name -> Unicode string or None\n"
	  "\n"
	  "Retrieves the name." },

	{ "get_type",
	  (PyCFunction) pycreg_value_get_type,
	  METH_NOARGS,
	  "get_type -> Integer or None\n"
	  "\n"
	  "Retrieves the type." },

	{ "get_data_size",
	  (PyCFunction) pycreg_value_get_data_size,
	  METH_NOARGS,
	  "get_data -> String or None\n"
	  "\n"
	  "Retrieves the size of the data as a binary string." },

	{ "get_data",
	  (PyCFunction) pycreg_value_get_data,
	  METH_NOARGS,
	  "get_data -> String or None\n"
	  "\n"
	  "Retrieves the data as a binary string." },

	{ "get_data_as_integer",
	  (PyCFunction) pycreg_value_get_data_as_integer,
	  METH_NOARGS,
	  "get_data_as_integer -> Integer\n"
	  "\n"
	  "Retrieves the data as an integer ." },

	{ "get_data_as_string",
	  (PyCFunction) pycreg_value_get_data_as_string,
	  METH_NOARGS,
	  "get_data_as_string -> Unicode string or None\n"
	  "\n"
	  "Retrieves the data as a string." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

PyGetSetDef pycreg_value_object_get_set_definitions[] = {

	{ "corrupted",
	  (getter) pycreg_value_is_corrupted,
	  (setter) 0,
	  "Value to indicate the value is corrupted.",
	  NULL },

	{ "offset",
	  (getter) pycreg_value_get_offset,
	  (setter) 0,
	  "The offset.",
	  NULL },

	{ "name",
	  (getter) pycreg_value_get_name,
	  (setter) 0,
	  "The name.",
	  NULL },

	{ "type",
	  (getter) pycreg_value_get_type,
	  (setter) 0,
	  "The type.",
	  NULL },

	{ "data_size",
	  (getter) pycreg_value_get_data_size,
	  (setter) 0,
	  "The data size.",
	  NULL },

	{ "data",
	  (getter) pycreg_value_get_data,
	  (setter) 0,
	  "The data.",
	  NULL },

	{ "data_as_integer",
	  (getter) pycreg_value_get_data_as_integer,
	  (setter) 0,
	  "The data represented as an integer.",
	  NULL },

	{ "data_as_string",
	  (getter) pycreg_value_get_data_as_string,
	  (setter) 0,
	  "The data represented as a string.",
	  NULL },

	/* Sentinel */
	{ NULL, NULL, NULL, NULL, NULL }
};

PyTypeObject pycreg_value_type_object = {
	PyVarObject_HEAD_INIT( NULL, 0 )

	/* tp_name */
	"pycreg.value",
	/* tp_basicsize */
	sizeof( pycreg_value_t ),
	/* tp_itemsize */
	0,
	/* tp_dealloc */
	(destructor) pycreg_value_free,
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
	"pycreg value object (wraps libcreg_value_t)",
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
	pycreg_value_object_methods,
	/* tp_members */
	0,
	/* tp_getset */
	pycreg_value_object_get_set_definitions,
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
	(initproc) pycreg_value_init,
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

/* Creates a new value object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_new(
           libcreg_value_t *value,
           PyObject *parent_object )
{
	pycreg_value_t *pycreg_value = NULL;
	static char *function        = "pycreg_value_new";

	if( value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	pycreg_value = PyObject_New(
	                struct pycreg_value,
	                &pycreg_value_type_object );

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize value.",
		 function );

		goto on_error;
	}
	if( pycreg_value_init(
	     pycreg_value ) != 0 )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to initialize value.",
		 function );

		goto on_error;
	}
	pycreg_value->parent_object = parent_object;
	pycreg_value->value         = value;

	Py_IncRef(
	 (PyObject *) pycreg_value->parent_object );

	return( (PyObject *) pycreg_value );

on_error:
	if( pycreg_value != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_value );
	}
	return( NULL );
}

/* Initializes an value object
 * Returns 0 if successful or -1 on error
 */
int pycreg_value_init(
     pycreg_value_t *pycreg_value )
{
	static char *function = "pycreg_value_init";

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	/* Make sure libcreg value is set to NULL
	 */
	pycreg_value->value = NULL;

	return( 0 );
}

/* Frees an value object
 */
void pycreg_value_free(
      pycreg_value_t *pycreg_value )
{
	libcerror_error_t *error    = NULL;
	struct _typeobject *ob_type = NULL;
	static char *function       = "pycreg_value_free";

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return;
	}
	if( pycreg_value->value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value - missing libcreg value.",
		 function );

		return;
	}
	ob_type = Py_TYPE(
	           pycreg_value );

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
	if( libcreg_value_free(
	     &( pycreg_value->value ),
	     &error ) != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to free libcreg value.",
		 function );

		libcerror_error_free(
		 &error );
	}
	if( pycreg_value->parent_object != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_value->parent_object );
	}
	ob_type->tp_free(
	 (PyObject*) pycreg_value );
}

/* Determines if the value is corrupted
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_is_corrupted(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "pycreg_value_is_corrupted";
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_ValueError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_is_corrupted(
	          pycreg_value->value,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to determine if value is corrupted.",
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
PyObject *pycreg_value_get_offset(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pycreg_value_get_offset";
	off64_t offset           = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_offset(
	          pycreg_value->value,
	          &offset,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
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
	integer_object = pycreg_integer_signed_new_from_64bit(
	                  (int64_t) offset );

	return( integer_object );
}

/* Retrieves the name
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_get_name(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *name            = NULL;
	static char *function    = "pycreg_value_get_name";
	size_t name_size         = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_utf8_name_size(
	          pycreg_value->value,
	          &name_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( name_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	name = (uint8_t *) PyMem_Malloc(
	                    sizeof( uint8_t ) * name_size );

	if( name == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create name.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_utf8_name(
		  pycreg_value->value,
		  name,
		  name_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve name.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) name,
			 (Py_ssize_t) name_size - 1,
			 NULL );

	PyMem_Free(
	 name );

	return( string_object );

on_error:
	if( name != NULL )
	{
		PyMem_Free(
		 name );
	}
	return( NULL );
}

/* Retrieves the value type
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_get_type(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pycreg_value_get_type";
	uint32_t value_type      = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_type(
	          pycreg_value->value,
	          &value_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pycreg_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_type );

	return( integer_object );
}

/* Retrieves the data size
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_get_data_size(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pycreg_value_get_data_size";
	size_t value_data_size   = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_data_size(
	          pycreg_value->value,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value data size.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pycreg_integer_unsigned_new_from_64bit(
	                  (uint64_t) value_data_size );

	return( integer_object );
}

/* Retrieves the data
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_get_data(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_data      = NULL;
	static char *function    = "pycreg_value_get_data";
	size_t value_data_size   = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_data_size(
	          pycreg_value->value,
	          &value_data_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value data size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_data_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_data = (uint8_t *) PyMem_Malloc(
	                          sizeof( uint8_t ) * value_data_size );

	if( value_data == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create value data.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_data(
		  pycreg_value->value,
		  value_data,
		  value_data_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value data.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
#if PY_MAJOR_VERSION >= 3
	string_object = PyBytes_FromStringAndSize(
			 (char *) value_data,
			 (Py_ssize_t) value_data_size );
#else
	string_object = PyString_FromStringAndSize(
			 (char *) value_data,
			 (Py_ssize_t) value_data_size );
#endif
	PyMem_Free(
	 value_data );

	return( string_object );

on_error:
	if( value_data != NULL )
	{
		PyMem_Free(
		 value_data );
	}
	return( NULL );
}

/* Retrieves the data represented as an integer
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_get_data_as_integer(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *integer_object = NULL;
	static char *function    = "pycreg_value_get_data_as_integer";
	uint64_t value_64bit     = 0;
	int64_t integer_value    = 0;
	uint32_t value_32bit     = 0;
	uint32_t value_type      = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_type(
	          pycreg_value->value,
	          &value_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	switch( value_type )
	{
		case LIBCREG_VALUE_TYPE_INTEGER_32BIT_LITTLE_ENDIAN:
		case LIBCREG_VALUE_TYPE_INTEGER_32BIT_BIG_ENDIAN:
			Py_BEGIN_ALLOW_THREADS

			result = libcreg_value_get_value_32bit(
				  pycreg_value->value,
				  &value_32bit,
				  &error );

			Py_END_ALLOW_THREADS

			/* Interpret the 32-bit value as signed
			 */
			integer_value = (int32_t) value_32bit;

			break;

		case LIBCREG_VALUE_TYPE_INTEGER_64BIT_LITTLE_ENDIAN:
			Py_BEGIN_ALLOW_THREADS

			result = libcreg_value_get_value_64bit(
				  pycreg_value->value,
				  &value_64bit,
				  &error );

			Py_END_ALLOW_THREADS

			/* Interpret the 64-bit value as signed
			 */
			integer_value = (int64_t) value_64bit;

			break;

		default:
			PyErr_Format(
			 PyExc_IOError,
			 "%s: value is not an integer type.",
			 function );

			return( NULL );
	}
	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve integer value.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	integer_object = pycreg_integer_signed_new_from_64bit(
	                  integer_value );

	return( integer_object );
}

/* Retrieves the data represented as a string
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_value_get_data_as_string(
           pycreg_value_t *pycreg_value,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	PyObject *string_object  = NULL;
	uint8_t *value_string    = NULL;
	static char *function    = "pycreg_value_get_data_as_string";
	size_t value_string_size = 0;
	uint32_t value_type      = 0;
	int result               = 0;

	PYCREG_UNREFERENCED_PARAMETER( arguments )

	if( pycreg_value == NULL )
	{
		PyErr_Format(
		 PyExc_TypeError,
		 "%s: invalid value.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_type(
	          pycreg_value->value,
	          &value_type,
	          &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value type.",
		 function );

		libcerror_error_free(
		 &error );

		return( NULL );
	}
	if( ( value_type != LIBCREG_VALUE_TYPE_STRING )
	 && ( value_type != LIBCREG_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( value_type != LIBCREG_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		PyErr_Format(
		 PyExc_IOError,
		 "%s: value is not a string type.",
		 function );

		return( NULL );
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_utf8_string_size(
	          pycreg_value->value,
	          &value_string_size,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value string size.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	else if( ( result == 0 )
	      || ( value_string_size == 0 ) )
	{
		Py_IncRef(
		 Py_None );

		return( Py_None );
	}
	value_string = (uint8_t *) PyMem_Malloc(
	                            sizeof( uint8_t ) * value_string_size );

	if( value_string == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create value string.",
		 function );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_value_get_value_utf8_string(
		  pycreg_value->value,
		  value_string,
		  value_string_size,
		  &error );

	Py_END_ALLOW_THREADS

	if( result != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to retrieve value string.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	string_object = PyUnicode_DecodeUTF8(
			 (char *) value_string,
			 (Py_ssize_t) value_string_size - 1,
			 NULL );

	PyMem_Free(
	 value_string );

	return( string_object );

on_error:
	if( value_string != NULL )
	{
		PyMem_Free(
		 value_string );
	}
	return( NULL );
}

