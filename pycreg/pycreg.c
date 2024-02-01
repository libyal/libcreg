/*
 * Python bindings module for libcreg (pycreg)
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

#include "pycreg.h"
#include "pycreg_error.h"
#include "pycreg_file.h"
#include "pycreg_file_object_io_handle.h"
#include "pycreg_key.h"
#include "pycreg_keys.h"
#include "pycreg_libbfio.h"
#include "pycreg_libcerror.h"
#include "pycreg_libcreg.h"
#include "pycreg_python.h"
#include "pycreg_unused.h"
#include "pycreg_value.h"
#include "pycreg_value_types.h"
#include "pycreg_values.h"

#if !defined( LIBCREG_HAVE_BFIO )

LIBCREG_EXTERN \
int libcreg_check_file_signature_file_io_handle(
     libbfio_handle_t *file_io_handle,
     libcreg_error_t **error );

#endif /* !defined( LIBCREG_HAVE_BFIO ) */

/* The pycreg module methods
 */
PyMethodDef pycreg_module_methods[] = {
	{ "get_version",
	  (PyCFunction) pycreg_get_version,
	  METH_NOARGS,
	  "get_version() -> String\n"
	  "\n"
	  "Retrieves the version." },

	{ "check_file_signature",
	  (PyCFunction) pycreg_check_file_signature,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature(filename) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Windows 9x/Me Registry File (CREG) signature." },

	{ "check_file_signature_file_object",
	  (PyCFunction) pycreg_check_file_signature_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "check_file_signature_file_object(file_object) -> Boolean\n"
	  "\n"
	  "Checks if a file has a Windows 9x/Me Registry File (CREG) signature using a file-like object." },

	{ "open",
	  (PyCFunction) pycreg_open_new_file,
	  METH_VARARGS | METH_KEYWORDS,
	  "open(filename, mode='r') -> Object\n"
	  "\n"
	  "Opens a file." },

	{ "open_file_object",
	  (PyCFunction) pycreg_open_new_file_with_file_object,
	  METH_VARARGS | METH_KEYWORDS,
	  "open_file_object(file_object, mode='r') -> Object\n"
	  "\n"
	  "Opens a file using a file-like object." },

	/* Sentinel */
	{ NULL, NULL, 0, NULL }
};

/* Retrieves the pycreg/libcreg version
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_get_version(
           PyObject *self PYCREG_ATTRIBUTE_UNUSED,
           PyObject *arguments PYCREG_ATTRIBUTE_UNUSED )
{
	const char *version_string   = NULL;
	size_t version_string_length = 0;

	PYCREG_UNREFERENCED_PARAMETER( self )
	PYCREG_UNREFERENCED_PARAMETER( arguments )

	Py_BEGIN_ALLOW_THREADS

	version_string = libcreg_get_version();

	Py_END_ALLOW_THREADS

	version_string_length = narrow_string_length(
	                         version_string );

	/* Pass the string length to PyUnicode_DecodeUTF8
	 * otherwise it makes the end of string character is part
	 * of the string
	 */
	return( PyUnicode_DecodeUTF8(
	         version_string,
	         (Py_ssize_t) version_string_length,
	         NULL ) );
}

/* Checks if a file has a Windows 9x/Me Registry File (CREG) signature
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_check_file_signature(
           PyObject *self PYCREG_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *string_object     = NULL;
	libcerror_error_t *error    = NULL;
	const char *filename_narrow = NULL;
	static char *function       = "pycreg_check_file_signature";
	static char *keyword_list[] = { "filename", NULL };
	int result                  = 0;

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	const wchar_t *filename_wide = NULL;
#else
	PyObject *utf8_string_object = NULL;
#endif

	PYCREG_UNREFERENCED_PARAMETER( self )

	/* Note that PyArg_ParseTupleAndKeywords with "s" will force Unicode strings to be converted to narrow character string.
	 * On Windows the narrow character strings contains an extended ASCII string with a codepage. Hence we get a conversion
	 * exception. This will also fail if the default encoding is not set correctly. We cannot use "u" here either since that
	 * does not allow us to pass non Unicode string objects and Python (at least 2.7) does not seems to automatically upcast them.
	 */
	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
	     keyword_list,
	     &string_object ) == 0 )
	{
		return( NULL );
	}
	PyErr_Clear();

	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyUnicode_Type );

	if( result == -1 )
	{
		pycreg_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type Unicode.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		filename_wide = (wchar_t *) PyUnicode_AsWideCharString(
		                             string_object,
		                             NULL );
#else
		filename_wide = (wchar_t *) PyUnicode_AsUnicode(
		                             string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libcreg_check_file_signature_wide(
		          filename_wide,
		          &error );

		Py_END_ALLOW_THREADS

#if PY_MAJOR_VERSION >= 3 && PY_MINOR_VERSION >= 3
		PyMem_Free(
		 filename_wide );
#endif
#else
		utf8_string_object = PyUnicode_AsUTF8String(
		                      string_object );

		if( utf8_string_object == NULL )
		{
			pycreg_error_fetch_and_raise(
			 PyExc_RuntimeError,
			 "%s: unable to convert Unicode string to UTF-8.",
			 function );

			return( NULL );
		}
#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   utf8_string_object );
#else
		filename_narrow = PyString_AsString(
		                   utf8_string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libcreg_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		Py_DecRef(
		 utf8_string_object );

#endif /* defined( HAVE_WIDE_SYSTEM_CHARACTER ) */

		if( result == -1 )
		{
			pycreg_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
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
	PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyBytes_Type );
#else
	result = PyObject_IsInstance(
	          string_object,
	          (PyObject *) &PyString_Type );
#endif
	if( result == -1 )
	{
		pycreg_error_fetch_and_raise(
		 PyExc_RuntimeError,
		 "%s: unable to determine if string object is of type string.",
		 function );

		return( NULL );
	}
	else if( result != 0 )
	{
		PyErr_Clear();

#if PY_MAJOR_VERSION >= 3
		filename_narrow = PyBytes_AsString(
		                   string_object );
#else
		filename_narrow = PyString_AsString(
		                   string_object );
#endif
		Py_BEGIN_ALLOW_THREADS

		result = libcreg_check_file_signature(
		          filename_narrow,
		          &error );

		Py_END_ALLOW_THREADS

		if( result == -1 )
		{
			pycreg_error_raise(
			 error,
			 PyExc_IOError,
			 "%s: unable to check file signature.",
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
	PyErr_Format(
	 PyExc_TypeError,
	 "%s: unsupported string object type.",
	 function );

	return( NULL );
}

/* Checks if a file has a Windows 9x/Me Registry File (CREG) signature using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_check_file_signature_file_object(
           PyObject *self PYCREG_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	PyObject *file_object            = NULL;
	libbfio_handle_t *file_io_handle = NULL;
	libcerror_error_t *error         = NULL;
	static char *function            = "pycreg_check_file_signature_file_object";
	static char *keyword_list[]      = { "file_object", NULL };
	int result                       = 0;

	PYCREG_UNREFERENCED_PARAMETER( self )

	if( PyArg_ParseTupleAndKeywords(
	     arguments,
	     keywords,
	     "O|",
	     keyword_list,
	     &file_object ) == 0 )
	{
		return( NULL );
	}
	if( pycreg_file_object_initialize(
	     &file_io_handle,
	     file_object,
	     &error ) != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to initialize file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	Py_BEGIN_ALLOW_THREADS

	result = libcreg_check_file_signature_file_io_handle(
	          file_io_handle,
	          &error );

	Py_END_ALLOW_THREADS

	if( result == -1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_IOError,
		 "%s: unable to check file signature.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
	}
	if( libbfio_handle_free(
	     &file_io_handle,
	     &error ) != 1 )
	{
		pycreg_error_raise(
		 error,
		 PyExc_MemoryError,
		 "%s: unable to free file IO handle.",
		 function );

		libcerror_error_free(
		 &error );

		goto on_error;
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

on_error:
	if( file_io_handle != NULL )
	{
		libbfio_handle_free(
		 &file_io_handle,
		 NULL );
	}
	return( NULL );
}

/* Creates a new file object and opens it
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_open_new_file(
           PyObject *self PYCREG_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pycreg_file_t *pycreg_file = NULL;
	static char *function      = "pycreg_open_new_file";

	PYCREG_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pycreg_file = PyObject_New(
	               struct pycreg_file,
	               &pycreg_file_type_object );

	if( pycreg_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( pycreg_file_init(
	     pycreg_file ) != 0 )
	{
		goto on_error;
	}
	if( pycreg_file_open(
	     pycreg_file,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pycreg_file );

on_error:
	if( pycreg_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_file );
	}
	return( NULL );
}

/* Creates a new file object and opens it using a file-like object
 * Returns a Python object if successful or NULL on error
 */
PyObject *pycreg_open_new_file_with_file_object(
           PyObject *self PYCREG_ATTRIBUTE_UNUSED,
           PyObject *arguments,
           PyObject *keywords )
{
	pycreg_file_t *pycreg_file = NULL;
	static char *function      = "pycreg_open_new_file_with_file_object";

	PYCREG_UNREFERENCED_PARAMETER( self )

	/* PyObject_New does not invoke tp_init
	 */
	pycreg_file = PyObject_New(
	               struct pycreg_file,
	               &pycreg_file_type_object );

	if( pycreg_file == NULL )
	{
		PyErr_Format(
		 PyExc_MemoryError,
		 "%s: unable to create file.",
		 function );

		goto on_error;
	}
	if( pycreg_file_init(
	     pycreg_file ) != 0 )
	{
		goto on_error;
	}
	if( pycreg_file_open_file_object(
	     pycreg_file,
	     arguments,
	     keywords ) == NULL )
	{
		goto on_error;
	}
	return( (PyObject *) pycreg_file );

on_error:
	if( pycreg_file != NULL )
	{
		Py_DecRef(
		 (PyObject *) pycreg_file );
	}
	return( NULL );
}

#if PY_MAJOR_VERSION >= 3

/* The pycreg module definition
 */
PyModuleDef pycreg_module_definition = {
	PyModuleDef_HEAD_INIT,

	/* m_name */
	"pycreg",
	/* m_doc */
	"Python libcreg module (pycreg).",
	/* m_size */
	-1,
	/* m_methods */
	pycreg_module_methods,
	/* m_reload */
	NULL,
	/* m_traverse */
	NULL,
	/* m_clear */
	NULL,
	/* m_free */
	NULL,
};

#endif /* PY_MAJOR_VERSION >= 3 */

/* Initializes the pycreg module
 */
#if PY_MAJOR_VERSION >= 3
PyMODINIT_FUNC PyInit_pycreg(
                void )
#else
PyMODINIT_FUNC initpycreg(
                void )
#endif
{
	PyObject *module           = NULL;
	PyGILState_STATE gil_state = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcreg_notify_set_stream(
	 stderr,
	 NULL );
	libcreg_notify_set_verbose(
	 1 );
#endif

	/* Create the module
	 * This function must be called before grabbing the GIL
	 * otherwise the module will segfault on a version mismatch
	 */
#if PY_MAJOR_VERSION >= 3
	module = PyModule_Create(
	          &pycreg_module_definition );
#else
	module = Py_InitModule3(
	          "pycreg",
	          pycreg_module_methods,
	          "Python libcreg module (pycreg)." );
#endif
	if( module == NULL )
	{
#if PY_MAJOR_VERSION >= 3
		return( NULL );
#else
		return;
#endif
	}
#if PY_VERSION_HEX < 0x03070000
	PyEval_InitThreads();
#endif
	gil_state = PyGILState_Ensure();

	/* Setup the file type object
	 */
	pycreg_file_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pycreg_file_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pycreg_file_type_object );

	PyModule_AddObject(
	 module,
	 "file",
	 (PyObject *) &pycreg_file_type_object );

	/* Setup the key type object
	 */
	pycreg_key_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pycreg_key_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pycreg_key_type_object );

	PyModule_AddObject(
	 module,
	 "key",
	 (PyObject *) &pycreg_key_type_object );

	/* Setup the keys type object
	 */
	pycreg_keys_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pycreg_keys_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pycreg_keys_type_object );

	PyModule_AddObject(
	 module,
	 "keys",
	 (PyObject *) &pycreg_keys_type_object );

	/* Setup the value type object
	 */
	pycreg_value_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pycreg_value_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pycreg_value_type_object );

	PyModule_AddObject(
	 module,
	 "value",
	 (PyObject *) &pycreg_value_type_object );

	/* Setup the value_types type object
	 */
	pycreg_value_types_type_object.tp_new = PyType_GenericNew;

	if( pycreg_value_types_init_type(
	     &pycreg_value_types_type_object ) != 1 )
	{
		goto on_error;
	}
	if( PyType_Ready(
	     &pycreg_value_types_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pycreg_value_types_type_object );

	PyModule_AddObject(
	 module,
	 "value_types",
	 (PyObject *) &pycreg_value_types_type_object );

	/* Setup the values type object
	 */
	pycreg_values_type_object.tp_new = PyType_GenericNew;

	if( PyType_Ready(
	     &pycreg_values_type_object ) < 0 )
	{
		goto on_error;
	}
	Py_IncRef(
	 (PyObject *) &pycreg_values_type_object );

	PyModule_AddObject(
	 module,
	 "values",
	 (PyObject *) &pycreg_values_type_object );

	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( module );
#else
	return;
#endif

on_error:
	PyGILState_Release(
	 gil_state );

#if PY_MAJOR_VERSION >= 3
	return( NULL );
#else
	return;
#endif
}

