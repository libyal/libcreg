/*
 * Value entry functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libcreg_data_type.h"
#include "libcreg_debug.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_libuna.h"
#include "libcreg_unused.h"
#include "libcreg_value_entry.h"

#include "creg_data_block.h"

/* Creates a value entry
 * Make sure the value value_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_entry_initialize(
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_value_entry_initialize";

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
	if( *value_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid value entry value already set.",
		 function );

		return( -1 );
	}
	*value_entry = memory_allocate_structure(
	                libcreg_value_entry_t );

	if( *value_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create value entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *value_entry,
	     0,
	     sizeof( libcreg_value_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear value entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *value_entry != NULL )
	{
		memory_free(
		 *value_entry );

		*value_entry = NULL;
	}
	return( -1 );
}

/* Frees a value entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_entry_free(
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_value_entry_free";

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
	if( *value_entry != NULL )
	{
		if( ( *value_entry )->data != NULL )
		{
			memory_free(
			 ( *value_entry )->data );
		}
		if( ( *value_entry )->name != NULL )
		{
			memory_free(
			 ( *value_entry )->name );
		}
		memory_free(
		 *value_entry );

		*value_entry = NULL;
	}
	return( 1 );
}

/* Reads a value entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_entry_read_data(
     libcreg_value_entry_t *value_entry,
     const uint8_t *data,
     size_t data_size,
     int ascii_codepage LIBCREG_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	static char *function    = "libcreg_value_entry_read_data";
	size_t value_data_offset = 0; 

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit     = 0; 
#endif

	LIBCREG_UNREFERENCED_PARAMETER( ascii_codepage )

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size < sizeof( creg_value_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
	if( data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_value_entry_t *) data )->data_type,
	 value_entry->type );

	byte_stream_copy_to_uint16_little_endian(
	 ( (creg_value_entry_t *) data )->name_size,
	 value_entry->name_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (creg_value_entry_t *) data )->data_size,
	 value_entry->data_size );

	value_entry->size = sizeof( creg_value_entry_t ) + value_entry->name_size + value_entry->data_size;

#if SIZEOF_SIZE_T <= 4
	if( value_entry->size > (size_t) SSIZE_MAX )
#else
	if( (uint32_t) value_entry->size > (uint32_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid value entry size value out of bounds.",
		 function );

		return( -1 );
	}
	if( data_size < (size_t) value_entry->size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 value_entry->size,
		 0 );
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data type\t\t\t\t: 0x%08" PRIx32 " (%s) %s\n",
		 function,
		 value_entry->type,
		 libcreg_data_type_get_identifier(
		  value_entry->type ),
		 libcreg_data_type_get_description(
		  value_entry->type ) );

		byte_stream_copy_to_uint32_little_endian(
		 ( (creg_value_entry_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: name size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_entry->name_size );

		libcnotify_printf(
		 "%s: data size\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_entry->data_size );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	value_data_offset = sizeof( creg_value_entry_t );

	if( value_entry->name_size > 0 )
	{
		if( value_entry->name_size > ( value_entry->size - value_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
		value_entry->name = (uint8_t *) memory_allocate(
		                                 sizeof( uint8_t ) * (size_t) value_entry->name_size );

		if( value_entry->name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create name.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     value_entry->name,
		     &( data[ value_data_offset ] ),
		     (size_t) value_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
		value_entry->name_hash = 0;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			if( libcreg_debug_print_string_value(
			     function,
			     "name\t\t\t\t\t",
			     &( data[ value_data_offset ] ),
			     (size_t) value_entry->name_size,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print string value.",
				 function );

				goto on_error;
			}
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		value_data_offset += value_entry->name_size;
	}
	if( value_entry->data_size > 0 )
	{
		if( value_entry->data_size > ( value_entry->size - value_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid data size value out of bounds.",
			 function );

			goto on_error;
		}
		value_entry->data = (uint8_t *) memory_allocate(
		                                 sizeof( uint8_t ) * (size_t) value_entry->data_size );

		if( value_entry->data == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create data.",
			 function );

			goto on_error;
		}
		if( memory_copy(
		     value_entry->data,
		     &( data[ value_data_offset ] ),
		     (size_t) value_entry->data_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ value_data_offset ] ),
			 (size_t) value_entry->data_size,
			 0 );
		}
#endif
	}
/* TODO trailing data */
	return( 1 );

on_error:
	if( value_entry->data != NULL )
	{
		memory_free(
		 value_entry->data );

		value_entry->data = NULL;
	}
	value_entry->data_size = 0;

	if( value_entry->name != NULL )
	{
		memory_free(
		 value_entry->name );

		value_entry->name = NULL;
	}
	value_entry->name_size = 0;

	return( -1 );
}

/* Retrieves the data size
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_entry_get_data_size(
     libcreg_value_entry_t *value_entry,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libcreg_value_entry_get_data_size";

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
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data_size = (size_t) value_entry->data_size;

	return( 1 );
}

/* Retrieves the data
 * Returns 1 if successful or -1 on error
 */
int libcreg_value_entry_get_data(
     libcreg_value_entry_t *value_entry,
     uint8_t **data,
     size_t *data_size,
     libcerror_error_t **error )
{
	static char *function = "libcreg_value_entry_get_data";

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
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( data_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data size.",
		 function );

		return( -1 );
	}
	*data      = value_entry->data;
	*data_size = (size_t) value_entry->data_size;

	return( 1 );
}

/* Compares the value name with UTF-8 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libcreg_value_entry_compare_name_with_utf8_string(
     libcreg_value_entry_t *value_entry,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libcreg_value_entry_compare_name_with_utf8_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf8_string_index                    = 0;

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
	if( value_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value entry - missing name.",
		 function );

		return( -1 );
	}
	if( utf8_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-8 string.",
		 function );

		return( -1 );
	}
	if( utf8_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-8 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Do a full compare if there no name hash was provided or the name hash matches
	 */
	if( ( name_hash == 0 )
	 || ( value_entry->name_hash == 0 )
	 || ( value_entry->name_hash == name_hash ) )
	{
		while( name_index < (size_t) value_entry->name_size )
		{
			if( utf8_string_index >= utf8_string_length )
			{
				break;
			}
			if( libuna_unicode_character_copy_from_byte_stream(
			     &name_character,
			     value_entry->name,
			     (size_t) value_entry->name_size,
			     &name_index,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy value name to Unicode character.",
				 function );

				return( -1 );
			}
			if( libuna_unicode_character_copy_from_utf8(
			     &string_character,
			     utf8_string,
			     utf8_string_length,
			     &utf8_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-8 string to Unicode character.",
				 function );

				return( -1 );
			}
			if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
			{
				break;
			}
		}
		if( ( name_index == (size_t) value_entry->name_size )
		 && ( utf8_string_index == utf8_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Compares the value name with UTF-16 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libcreg_value_entry_compare_name_with_utf16_string(
     libcreg_value_entry_t *value_entry,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libcreg_value_entry_compare_name_with_utf16_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf16_string_index                   = 0;

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
	if( value_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid value entry - missing name.",
		 function );

		return( -1 );
	}
	if( utf16_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid UTF-16 string.",
		 function );

		return( -1 );
	}
	if( utf16_string_length > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid UTF-16 string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	/* Do a full compare if there no name hash was provided or the name hash matches
	 */
	if( ( name_hash == 0 )
	 || ( value_entry->name_hash == 0 )
	 || ( value_entry->name_hash == name_hash ) )
	{
		while( name_index < (size_t) value_entry->name_size )
		{
			if( utf16_string_index >= utf16_string_length )
			{
				break;
			}
			if( libuna_unicode_character_copy_from_byte_stream(
			     &name_character,
			     value_entry->name,
			     (size_t) value_entry->name_size,
			     &name_index,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy value name to Unicode character.",
				 function );

				return( -1 );
			}
			if( libuna_unicode_character_copy_from_utf16(
			     &string_character,
			     utf16_string,
			     utf16_string_length,
			     &utf16_string_index,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy UTF-16 string to Unicode character.",
				 function );

				return( -1 );
			}
			if( towupper( (wint_t) name_character ) != towupper( (wint_t) string_character ) )
			{
				break;
			}
		}
		if( ( name_index == (size_t) value_entry->name_size )
		 && ( utf16_string_index == utf16_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

