/*
 * Value functions
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
#include <memory.h>
#include <types.h>

#include "libcreg_definitions.h"
#include "libcreg_io_handle.h"
#include "libcreg_libcerror.h"
#include "libcreg_libuna.h"
#include "libcreg_value.h"
#include "libcreg_value_entry.h"
#include "libcreg_value_type.h"

/* Creates a value
 * Make sure the value value is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_initialize(
     libcreg_value_t **value,
     libcreg_io_handle_t *io_handle,
     libcreg_value_entry_t *value_entry,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_initialize";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value value already set.",
		 function );

		return( -1 );
	}
	if( value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entry.",
		 function );

		return( -1 );
	}
	internal_value = memory_allocate_structure(
	                  libcreg_internal_value_t );

	if( internal_value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal value.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_value,
	     0,
	     sizeof( libcreg_internal_value_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal value.",
		 function );

		memory_free(
		 internal_value );

		return( -1 );
	}
	internal_value->io_handle   = io_handle;
	internal_value->value_entry = value_entry;

	*value = (libcreg_value_t *) internal_value;

	return( 1 );

on_error:
	if( internal_value != NULL )
	{
		memory_free(
		 internal_value );
	}
	return( -1 );
}

/* Frees a value
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_free(
     libcreg_value_t **value,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_free";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( *value != NULL )
	{
		internal_value = (libcreg_internal_value_t *) *value;
		*value         = NULL;

		/* The io_handle and value_entry references are freed elsewhere
		 */
		memory_free(
		 internal_value );
	}
	return( 1 );
}

/* Determine if the value corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libcreg_value_is_corrupted(
     libcreg_value_t *value,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_is_corrupted";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( ( internal_value->value_entry->flags & LIBCREG_VALUE_ENTRY_FLAG_IS_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_offset(
     libcreg_value_t *value,
     off64_t *offset,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_offset";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
	*offset = internal_value->value_entry->offset;

	return( 1 );
}

/* Retrieves the value name size
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_name_size(
     libcreg_value_t *value,
     size_t *string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_name_size";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( string_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string size.",
		 function );

		return( -1 );
	}
	*string_size = internal_value->value_entry->name_size;

	return( 1 );
}

/* Retrieves the value name
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_name(
     libcreg_value_t *value,
     uint8_t *string,
     size_t string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_name";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid string.",
		 function );

		return( -1 );
	}
	if( string_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid string size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( string_size < internal_value->value_entry->name_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid string size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     string,
	     internal_value->value_entry->name,
	     internal_value->value_entry->name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy name.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_utf8_name_size(
     libcreg_value_t *value,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_utf8_name_size";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->name == NULL )
	{
		if( utf8_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-8 string size.",
			 function );

			return( -1 );
		}
		*utf8_string_size = 0;
	}
	else
	{
		if( libuna_utf8_string_size_from_byte_stream(
		     internal_value->value_entry->name,
		     (size_t) internal_value->value_entry->name_size,
		     internal_value->io_handle->ascii_codepage,
		     utf8_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_utf8_name(
     libcreg_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_utf8_name";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value item - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf8_string_copy_from_byte_stream(
	     utf8_string,
	     utf8_string_size,
	     internal_value->value_entry->name,
	     (size_t) internal_value->value_entry->name_size,
	     internal_value->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_utf16_name_size(
     libcreg_value_t *value,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_utf16_name_size";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->name == NULL )
	{
		if( utf16_string_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid UTF-16 string size.",
			 function );

			return( -1 );
		}
		*utf16_string_size = 0;
	}
	else
	{
		if( libuna_utf16_string_size_from_byte_stream(
		     internal_value->value_entry->name,
		     (size_t) internal_value->value_entry->name_size,
		     internal_value->io_handle->ascii_codepage,
		     utf16_string_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 string size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_utf16_name(
     libcreg_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_utf16_name";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value item - missing name.",
		 function );

		return( -1 );
	}
	if( libuna_utf16_string_copy_from_byte_stream(
	     utf16_string,
	     utf16_string_size,
	     internal_value->value_entry->name,
	     (size_t) internal_value->value_entry->name_size,
	     internal_value->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 string.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value type
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_type(
     libcreg_value_t *value,
     uint32_t *value_type,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_value_type";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( value_type == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value type.",
		 function );

		return( -1 );
	}
	*value_type = internal_value->value_entry->type;

	return( 1 );
}

/* Retrieves the value data size
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_data_size(
     libcreg_value_t *value,
     size_t *value_data_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_value_data_size";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( libcreg_value_entry_get_data_size(
	     internal_value->value_entry,
	     value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value data
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_data(
     libcreg_value_t *value,
     uint8_t *value_data,
     size_t value_data_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *data                            = NULL;
	static char *function                    = "libcreg_value_get_value_data";
	size_t data_size                         = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( value_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value data.",
		 function );

		return( -1 );
	}
	if( value_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &data,
	     &data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size < data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value data size value out of bounds.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     value_data,
	     data,
	     data_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy value data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the 32-bit value
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_32bit(
     libcreg_value_t *value,
     uint32_t *value_32bit,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_32bit";
	size_t value_data_size                   = 0;
	uint8_t byte_order                       = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_INTEGER_32BIT_BIG_ENDIAN )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_INTEGER_32BIT_LITTLE_ENDIAN ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported 32-bit integer value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( internal_value->value_entry->type == LIBCREG_VALUE_TYPE_INTEGER_32BIT_BIG_ENDIAN )
	{
		byte_order = LIBCREG_ENDIAN_BIG;
	}
	else
	{
		byte_order = LIBCREG_ENDIAN_LITTLE;
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size != 4 )
	{
		if( value_data_size > 4 )
		{
			value_data_size = 4;
		}
		internal_value->value_entry->flags |= LIBCREG_VALUE_ENTRY_FLAG_IS_CORRUPTED;
	}
	if( libcreg_value_type_copy_to_32bit(
	     value_data,
	     value_data_size,
	     byte_order,
	     value_32bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set 32-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the 64-bit value
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_64bit(
     libcreg_value_t *value,
     uint64_t *value_64bit,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_64bit";
	size_t value_data_size                   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_INTEGER_64BIT_LITTLE_ENDIAN )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported 64-bit integer value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( value_data_size != 8 )
	{
		if( value_data_size > 8 )
		{
			value_data_size = 8;
		}
		internal_value->value_entry->flags |= LIBCREG_VALUE_ENTRY_FLAG_IS_CORRUPTED;
	}
	if( libcreg_value_type_copy_to_64bit(
	     value_data,
	     value_data_size,
	     LIBCREG_ENDIAN_LITTLE,
	     value_64bit,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set 64-bit value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_utf8_string_size(
     libcreg_value_t *value,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_utf8_string_size";
	size_t value_data_size                   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libcreg_value_type_get_utf8_string_size(
	     value_data,
	     value_data_size,
	     internal_value->io_handle->ascii_codepage,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string size value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_utf8_string(
     libcreg_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_utf8_string";
	size_t value_data_size                   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libcreg_value_type_copy_to_utf8_string(
	     value_data,
	     value_data_size,
	     internal_value->io_handle->ascii_codepage,
	     utf8_string,
	     utf8_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-8 string value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size at a specific value from the referenced value
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_utf16_string_size(
     libcreg_value_t *value,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_utf16_string_size";
	size_t value_data_size                   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libcreg_value_type_get_utf16_string_size(
	     value_data,
	     value_data_size,
	     internal_value->io_handle->ascii_codepage,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-16 string size value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_utf16_string(
     libcreg_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_utf16_string";
	size_t value_data_size                   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing IO handle.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_EXPANDABLE_STRING )
	 && ( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_SYMBOLIC_LINK ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported string value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libcreg_value_type_copy_to_utf16_string(
	     value_data,
	     value_data_size,
	     internal_value->io_handle->ascii_codepage,
	     utf16_string,
	     utf16_string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set UTF-16 string value.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the binary data size
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_binary_data_size(
     libcreg_value_t *value,
     size_t *size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	static char *function                    = "libcreg_value_get_value_binary_data_size";

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_BINARY_DATA )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported binary data value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data_size(
	     internal_value->value_entry,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the binary data value
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_get_value_binary_data(
     libcreg_value_t *value,
     uint8_t *binary_data,
     size_t size,
     libcerror_error_t **error )
{
	libcreg_internal_value_t *internal_value = NULL;
	uint8_t *value_data                      = NULL;
	static char *function                    = "libcreg_value_get_value_binary_data";
	size_t value_data_size                   = 0;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	internal_value = (libcreg_internal_value_t *) value;

	if( internal_value->value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value - missing value entry.",
		 function );

		return( -1 );
	}
	if( internal_value->value_entry->type != LIBCREG_VALUE_TYPE_BINARY_DATA )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: unsupported binary data value type: 0x%04" PRIx32 ".",
		 function,
		 internal_value->value_entry->type );

		return( -1 );
	}
	if( libcreg_value_entry_get_data(
	     internal_value->value_entry,
	     &value_data,
	     &value_data_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data.",
		 function );

		return( -1 );
	}
	if( libcreg_value_type_copy_to_binary_data(
	     value_data,
	     value_data_size,
	     binary_data,
	     size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_CONVERSION,
		 LIBCERROR_CONVERSION_ERROR_GENERIC,
		 "%s: unable to set binary data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

