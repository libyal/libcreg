/*
 * Key name entry functions
 *
 * Copyright (C) 2013-2016, Joachim Metz <joachim.metz@gmail.com>
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libcreg_key_name_entry.h"
#include "libcreg_libcdata.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_libuna.h"
#include "libcreg_value_entry.h"

#include "creg_data_block.h"

/* Creates a key name entry
 * Make sure the value key_name_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_initialize(
     libcreg_key_name_entry_t **key_name_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_name_entry_initialize";

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( *key_name_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key name entry value already set.",
		 function );

		return( -1 );
	}
	*key_name_entry = memory_allocate_structure(
	                   libcreg_key_name_entry_t );

	if( *key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key name entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_name_entry,
	     0,
	     sizeof( libcreg_key_name_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key name entry.",
		 function );

		memory_free(
		 *key_name_entry );

		*key_name_entry = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *key_name_entry )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create value entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_name_entry != NULL )
	{
		memory_free(
		 *key_name_entry );

		*key_name_entry = NULL;
	}
	return( -1 );
}

/* Frees a key name entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_free(
     libcreg_key_name_entry_t **key_name_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_name_entry_free";
	int result            = 1;

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( *key_name_entry != NULL )
	{
		if( ( *key_name_entry )->name != NULL )
		{
			memory_free(
			 ( *key_name_entry )->name );
		}
		if( libcdata_array_free(
		     &( ( *key_name_entry )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_value_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the value entries array.",
			 function );

			result = -1;
		}
		memory_free(
		 *key_name_entry );

		*key_name_entry = NULL;
	}
	return( result );
}

/* Reads a key name entry size
 * Callback function for the data block
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_read_entry_size(
     const uint8_t *data,
     size_t data_size,
     size_t *entry_size,
     libcerror_error_t **error )
{
	static char *function        = "libcreg_key_name_entry_read_entry_size";
	uint32_t key_name_entry_size = 0;

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
	if( data_size < sizeof( creg_key_name_entry_t ) )
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
	if( entry_size == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid entry size.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_key_name_entry_t *) data )->size,
	 key_name_entry_size );

#if SIZEOF_SIZE_T <= 4
	if( ( key_name_entry_size < sizeof( creg_key_name_entry_t ) )
	 || ( key_name_entry_size > (size_t) SSIZE_MAX ) )
#else
	if( ( key_name_entry_size < sizeof( creg_key_name_entry_t ) )
	 || ( key_name_entry_size > (uint32_t) SSIZE_MAX ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid key name entry data size value out of bounds.",
		 function );

		return( -1 );
	}
	*entry_size = (size_t) key_name_entry_size;

	return( 1 );
}

/* Reads a key name entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_read(
     libcreg_key_name_entry_t *key_name_entry,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function                       = "libcreg_key_name_entry_read";
	size_t values_data_offset                   = 0;
	uint32_t used_size                          = 0;
	uint16_t number_of_values                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcstring_system_character_t *value_string = NULL;
	size_t value_entries_data_size              = 0;
	size_t value_string_size                    = 0;
	uint32_t value_32bit                        = 0;
	uint16_t value_16bit                        = 0;
/* TODO set codepage */
	int ascii_codepage                          = 1252;
	int result                                  = 0;
#endif

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
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
	if( data_size < sizeof( creg_key_name_entry_t ) )
	{
fprintf( stderr, "X: %zd\n", data_size );
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
	 ( (creg_key_name_entry_t *) data )->size,
	 key_name_entry->size );

#if SIZEOF_SIZE_T <= 4
	if( ( key_name_entry->size < sizeof( creg_key_name_entry_t ) )
	 || ( key_name_entry->size > (size_t) SSIZE_MAX ) )
#else
	if( ( key_name_entry->size < sizeof( creg_key_name_entry_t ) )
	 || ( key_name_entry->size > (uint32_t) SSIZE_MAX ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid entry data size value out of bounds.",
		 function );

		goto on_error;
	}
	if( data_size < key_name_entry->size )
	{
fprintf( stderr, "Y: %zd\n", data_size );
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
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
		 data,
		 (size_t) key_name_entry->size,
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_key_name_entry_t *) data )->used_size,
	 used_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (creg_key_name_entry_t *) data )->name_size,
	 key_name_entry->name_size );

	byte_stream_copy_to_uint16_little_endian(
	 ( (creg_key_name_entry_t *) data )->number_of_values,
	 number_of_values );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 key_name_entry->size );

		byte_stream_copy_to_uint16_little_endian(
		 ( (creg_key_name_entry_t *) data )->index,
		 value_16bit );
		libcnotify_printf(
		 "%s: index\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 ( (creg_key_name_entry_t *) data )->unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: used size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 used_size );

		libcnotify_printf(
		 "%s: name size\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 key_name_entry->name_size );

		libcnotify_printf(
		 "%s: number of values\t\t\t\t: %" PRIu16 "\n",
		 function,
		 number_of_values );

		byte_stream_copy_to_uint32_little_endian(
		 ( (creg_key_name_entry_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );
	}
#endif
#if SIZEOF_SIZE_T <= 4
	if( ( used_size < sizeof( creg_key_name_entry_t ) )
	 || ( used_size > (size_t) SSIZE_MAX ) )
#else
	if( ( used_size < sizeof( creg_key_name_entry_t ) )
	 || ( used_size > (uint32_t) SSIZE_MAX ) )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid used size value out of bounds.",
		 function );

		goto on_error;
	}
	if( data_size < used_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid data size value too small.",
		 function );

		goto on_error;
	}
	values_data_offset = sizeof( creg_key_name_entry_t );

	if( key_name_entry->name_size > 0 )
	{
		if( key_name_entry->name_size > ( key_name_entry->size - values_data_offset ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			goto on_error;
		}
		key_name_entry->name = (uint8_t *) memory_allocate(
		                                    sizeof( uint8_t ) * (size_t) key_name_entry->name_size );

		if( key_name_entry->name == NULL )
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
		     key_name_entry->name,
		     &( data[ values_data_offset ] ),
		     (size_t) key_name_entry->name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy name.",
			 function );

			goto on_error;
		}
		key_name_entry->name_hash = 0;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_size_from_byte_stream(
				  &( data[ values_data_offset ] ),
				  (size_t) key_name_entry->name_size,
				  ascii_codepage,
				  &value_string_size,
				  error );
#else
			result = libuna_utf8_string_size_from_byte_stream(
				  &( data[ values_data_offset ] ),
				  (size_t) key_name_entry->name_size,
				  ascii_codepage,
				  &value_string_size,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name string size.",
				 function );

				goto on_error;
			}
			value_string = libcstring_system_string_allocate(
			                value_string_size );

			if( value_string == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create name string.",
				 function );

				goto on_error;
			}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_utf16_string_copy_from_byte_stream(
				  (libuna_utf16_character_t *) value_string,
				  value_string_size,
				  &( data[ values_data_offset ] ),
				  (size_t) key_name_entry->name_size,
				  ascii_codepage,
				  error );
#else
			result = libuna_utf8_string_copy_from_byte_stream(
				  (libuna_utf8_character_t *) value_string,
				  value_string_size,
				  &( data[ values_data_offset ] ),
				  (size_t) key_name_entry->name_size,
				  ascii_codepage,
				  error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve name string.",
				 function );

				goto on_error;
			}
			libcnotify_printf(
			 "%s: name\t\t\t\t\t: %" PRIs_LIBCSTRING_SYSTEM "\n",
			 function,
			 value_string );

			memory_free(
			 value_string );

			value_string = NULL;
		}
#endif
		values_data_offset += key_name_entry->name_size;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		value_entries_data_size = (size_t) used_size - values_data_offset;

		libcnotify_printf(
		 "%s: values data:\n",
		 function );
		libcnotify_print_data(
		 &( data[ values_data_offset ] ),
		 value_entries_data_size,
		 0 );

		if( libcreg_key_name_entry_read_values(
		     key_name_entry,
		     number_of_values,
		     &( data[ values_data_offset ] ),
		     value_entries_data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read value entries.",
			 function );

			return( -1 );
		}
	}
#endif
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( used_size < key_name_entry->size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( data[ used_size ] ),
			 key_name_entry->size - used_size,
			 0 );
		}
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( value_string != NULL )
	{
		memory_free(
		 value_string );
	}
#endif
	if( key_name_entry->name != NULL )
	{
		memory_free(
		 key_name_entry->name );

		key_name_entry->name = NULL;
	}
	key_name_entry->name_size = 0;

	return( -1 );
}

/* Reads the values of a key name entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_read_values(
     libcreg_key_name_entry_t *key_name_entry,
     int number_of_values,
     const uint8_t *value_entries_data,
     size_t value_entries_data_size,
     libcerror_error_t **error )
{
	libcreg_value_entry_t *value_entry = NULL;
	static char *function              = "libcreg_key_name_entry_read_values";
	uint32_t value_entries_offset      = 0;
	int entry_index                    = 0;
	int value_entry_index              = 0;

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( number_of_values == 0 )
	{
		return( 1 );
	}
	if( value_entries_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value entries data.",
		 function );

		return( -1 );
	}
	if( value_entries_data_size == 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_TOO_SMALL,
		 "%s: invalid value entries data size value too small.",
		 function );

		return( -1 );
	}
	if( value_entries_data_size > (size_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value entries data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	for( value_entry_index = 0;
	     value_entry_index < number_of_values;
	     value_entry_index++ )
	{
		if( libcreg_value_entry_initialize(
		     &value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create value entry: %d.",
			 function,
			 value_entry_index );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: value entry: %03d at offset\t\t: 0x%08" PRIx32 "\n",
			 function,
			 value_entry_index,
			 value_entries_offset );
		}
#endif
		if( libcreg_value_entry_read(
		     value_entry,
		     &( value_entries_data[ value_entries_offset ] ),
		     (size_t) value_entries_data_size - value_entries_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read value entry: %d.",
			 function,
			 value_entry_index );

			goto on_error;
		}
		value_entry->offset   = key_name_entry->offset + value_entries_offset;
		value_entries_offset += value_entry->size;

		if( libcdata_array_append_entry(
		     key_name_entry->entries_array,
		     &entry_index,
		     (intptr_t *) value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append value entry: %" PRIu16 ".",
			 function,
			 value_entry_index );

			goto on_error;
		}
		value_entry = NULL;
	}
	return( 1 );

on_error:
	if( value_entry != NULL )
	{
		libcreg_value_entry_free(
		 &value_entry,
		 NULL );
	}
	if( key_name_entry->entries_array != NULL )
	{
		libcdata_array_clear(
		 key_name_entry->entries_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_value_entry_free,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the number of value entries
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_get_number_of_entries(
     libcreg_key_name_entry_t *key_name_entry,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_name_entry_get_number_of_entries";

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     key_name_entry->entries_array,
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value entry at the index
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_name_entry_get_entry_by_index(
     libcreg_key_name_entry_t *key_name_entry,
     int entry_index,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_name_entry_get_entry_by_index";

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     key_name_entry->entries_array,
	     entry_index,
	     (intptr_t **) value_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value entry: %" PRIu16 ".",
		 function,
		 entry_index );

		return( -1 );
	}
	return( 1 );
}

/* Compares the key name with UTF-8 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libcreg_key_name_entry_compare_name_with_utf8_string(
     libcreg_key_name_entry_t *key_name_entry,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libcreg_key_name_entry_compare_name_with_utf8_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf8_string_index                    = 0;

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( key_name_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key name entry - missing name.",
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
	 || ( key_name_entry->name_hash == 0 )
	 || ( key_name_entry->name_hash == name_hash ) )
	{
		while( name_index < (size_t) key_name_entry->name_size )
		{
			if( utf8_string_index >= utf8_string_length )
			{
				break;
			}
			if( libuna_unicode_character_copy_from_byte_stream(
			     &name_character,
			     key_name_entry->name,
			     (size_t) key_name_entry->name_size,
			     &name_index,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy key name to Unicode character.",
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
		if( ( name_index == (size_t) key_name_entry->name_size )
		 && ( utf8_string_index == utf8_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

/* Compares the key name with UTF-16 string
 * Returns 1 if the names match, 0 if not or -1 on error
 */
int libcreg_key_name_entry_compare_name_with_utf16_string(
     libcreg_key_name_entry_t *key_name_entry,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function                       = "libcreg_key_name_entry_compare_name_with_utf16_string";
	libuna_unicode_character_t name_character   = 0;
	libuna_unicode_character_t string_character = 0;
	size_t name_index                           = 0;
	size_t utf16_string_index                   = 0;

	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( key_name_entry->name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key name entry - missing name.",
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
	 || ( key_name_entry->name_hash == 0 )
	 || ( key_name_entry->name_hash == name_hash ) )
	{
		while( name_index < (size_t) key_name_entry->name_size )
		{
			if( utf16_string_index >= utf16_string_length )
			{
				break;
			}
			if( libuna_unicode_character_copy_from_byte_stream(
			     &name_character,
			     key_name_entry->name,
			     (size_t) key_name_entry->name_size,
			     &name_index,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_COPY_FAILED,
				 "%s: unable to copy key name to Unicode character.",
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
		if( ( name_index == (size_t) key_name_entry->name_size )
		 && ( utf16_string_index == utf16_string_length ) )
		{
			return( 1 );
		}
	}
	return( 0 );
}

