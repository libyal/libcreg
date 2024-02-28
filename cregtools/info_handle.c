/*
 * Info handle
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#include "cregtools_libcerror.h"
#include "cregtools_libclocale.h"
#include "cregtools_libcreg.h"
#include "cregtools_libuna.h"
#include "info_handle.h"

#define INFO_HANDLE_NOTIFY_STREAM	stdout

/* Creates an info handle
 * Make sure the value info_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int info_handle_initialize(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_initialize";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid info handle value already set.",
		 function );

		return( -1 );
	}
	*info_handle = memory_allocate_structure(
	                info_handle_t );

	if( *info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create info handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *info_handle,
	     0,
	     sizeof( info_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear info handle.",
		 function );

		goto on_error;
	}
	if( libcreg_file_initialize(
	     &( ( *info_handle )->input_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize input file.",
		 function );

		goto on_error;
	}
	( *info_handle )->ascii_codepage = LIBCREG_CODEPAGE_WINDOWS_1252;
	( *info_handle )->notify_stream  = INFO_HANDLE_NOTIFY_STREAM;

	return( 1 );

on_error:
	if( *info_handle != NULL )
	{
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( -1 );
}

/* Frees an info handle
 * Returns 1 if successful or -1 on error
 */
int info_handle_free(
     info_handle_t **info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_free";
	int result            = 1;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( *info_handle != NULL )
	{
		if( libcreg_file_free(
		     &( ( *info_handle )->input_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free input file.",
			 function );

			result = -1;
		}
		memory_free(
		 *info_handle );

		*info_handle = NULL;
	}
	return( result );
}

/* Signals the info handle to abort
 * Returns 1 if successful or -1 on error
 */
int info_handle_signal_abort(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_signal_abort";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	info_handle->abort = 1;

	if( info_handle->input_file != NULL )
	{
		if( libcreg_file_signal_abort(
		     info_handle->input_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal input file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int info_handle_set_ascii_codepage(
     info_handle_t *info_handle,
     const system_character_t *string,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_set_ascii_codepage";
	size_t string_length   = 0;
	uint32_t feature_flags = 0;
	int result             = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	feature_flags = LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_KOI8
	              | LIBCLOCALE_CODEPAGE_FEATURE_FLAG_HAVE_WINDOWS;

	string_length = system_string_length(
	                 string );

#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libclocale_codepage_copy_from_string_wide(
	          &( info_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#else
	result = libclocale_codepage_copy_from_string(
	          &( info_handle->ascii_codepage ),
	          string,
	          string_length,
	          feature_flags,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine ASCII codepage.",
		 function );

		return( -1 );
	}
	return( result );
}

/* Opens the input
 * Returns 1 if successful or -1 on error
 */
int info_handle_open_input(
     info_handle_t *info_handle,
     const system_character_t *filename,
     libcerror_error_t **error )
{
	static char *function = "info_handle_open_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libcreg_file_set_ascii_codepage(
	     info_handle->input_file,
	     info_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in input file.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcreg_file_open_wide(
	     info_handle->input_file,
	     filename,
	     LIBCREG_OPEN_READ,
	     error ) != 1 )
#else
	if( libcreg_file_open(
	     info_handle->input_file,
	     filename,
	     LIBCREG_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open input file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Closes the input
 * Returns the 0 if succesful or -1 on error
 */
int info_handle_close_input(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function = "info_handle_close_input";

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libcreg_file_close(
	     info_handle->input_file,
	     error ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_CLOSE_FAILED,
		 "%s: unable to close input file.",
		 function );

		return( -1 );
	}
	return( 0 );
}

/* Prints a file entry or data stream name
 * Returns 1 if successful or -1 on error
 */
int info_handle_name_value_fprint(
     info_handle_t *info_handle,
     const system_character_t *value_string,
     size_t value_string_length,
     libcerror_error_t **error )
{
	system_character_t *escaped_value_string     = NULL;
	static char *function                        = "info_handle_name_value_fprint";
	libuna_unicode_character_t unicode_character = 0;
	size_t escaped_value_string_index            = 0;
	size_t escaped_value_string_size             = 0;
	size_t value_string_index                    = 0;
	int print_count                              = 0;
	int result                                   = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value string.",
		 function );

		return( -1 );
	}
	/* To ensure normalization in the escaped string is handled correctly
	 * it stored in a temporary variable. Note that there is a worst-case of
	 * a 1 to 4 ratio for each escaped character.
	 */
	if( value_string_length > (size_t) ( ( SSIZE_MAX - 1 ) / ( sizeof( system_character_t ) * 4 ) ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid value string length value exceeds maximum.",
		 function );

		return( -1 );
	}
	escaped_value_string_size = ( value_string_length * 4 ) + 1;

	escaped_value_string = (system_character_t *) memory_allocate(
	                                               sizeof( system_character_t ) * escaped_value_string_size );

	if( escaped_value_string == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create escaped value string.",
		 function );

		goto on_error;
	}
	while( value_string_index < value_string_length )
	{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libuna_unicode_character_copy_from_utf16(
		          &unicode_character,
		          (libuna_utf16_character_t *) value_string,
		          value_string_length,
		          &value_string_index,
		          error );
#else
		result = libuna_unicode_character_copy_from_utf8(
		          &unicode_character,
		          (libuna_utf8_character_t *) value_string,
		          value_string_length,
		          &value_string_index,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_CONVERSION,
			 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
			 "%s: unable to copy Unicode character from value string.",
			 function );

			goto on_error;
		}
		/* Replace:
		 *   values <= 0x1f and 0x7f by \x##
		 */
		if( ( unicode_character <= 0x1f )
		 || ( unicode_character == 0x7f ) )
		{
			print_count = system_string_sprintf(
			               &( escaped_value_string[ escaped_value_string_index ] ),
			               escaped_value_string_size - escaped_value_string_index,
			               "\\x%02" PRIx32 "",
			               unicode_character );

			if( print_count < 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy escaped Unicode character to escaped value string.",
				 function );

				goto on_error;
			}
			escaped_value_string_index += print_count;
		}
		else
		{
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libuna_unicode_character_copy_to_utf16(
			          unicode_character,
			          (libuna_utf16_character_t *) escaped_value_string,
			          escaped_value_string_size,
			          &escaped_value_string_index,
			          error );
#else
			result = libuna_unicode_character_copy_to_utf8(
			          unicode_character,
			          (libuna_utf8_character_t *) escaped_value_string,
			          escaped_value_string_size,
			          &escaped_value_string_index,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_CONVERSION,
				 LIBCERROR_CONVERSION_ERROR_INPUT_FAILED,
				 "%s: unable to copy Unicode character to escaped value string.",
				 function );

				goto on_error;
			}
		}
	}
	escaped_value_string[ escaped_value_string_index ] = 0;

	fprintf(
	 info_handle->notify_stream,
	 "%" PRIs_SYSTEM "",
	 escaped_value_string );

	memory_free(
	 escaped_value_string );

	return( 1 );

on_error:
	if( escaped_value_string != NULL )
	{
		memory_free(
		 escaped_value_string );
	}
	return( -1 );
}

/* Prints key information
 * Returns 1 if successful or -1 on error
 */
int info_handle_key_fprint(
     info_handle_t *info_handle,
     libcreg_key_t *key,
     const system_character_t *key_path,
     size_t key_path_length,
     libcreg_error_t **error )
{
	libcreg_key_t *sub_key           = NULL;
	libcreg_value_t *value           = NULL;
	system_character_t *key_name     = NULL;
	system_character_t *sub_key_path = NULL;
	system_character_t *value_name   = NULL;
	static char *function            = "info_handle_key_fprint";
	size_t key_name_length           = 0;
	size_t key_name_size             = 0;
	size_t sub_key_path_size         = 0;
	size_t value_name_size           = 0;
	int number_of_sub_keys           = 0;
	int number_of_values             = 0;
	int result                       = 0;
	int sub_key_index                = 0;
	int value_index                  = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcreg_key_get_utf16_name_size(
	          key,
	          &key_name_size,
	          error );
#else
	result = libcreg_key_get_utf8_name_size(
	          key,
	          &key_name_size,
	          error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key name size.",
		 function );

		goto on_error;
	}
	if( key_name_size > 0 )
	{
		if( key_name_size > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid key name size value exceeds maximum allocation size.",
			 function );

			goto on_error;
		}
		key_name = system_string_allocate(
		            key_name_size );

		if( key_name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create key name string.",
			 function );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcreg_key_get_utf16_name(
		          key,
		          (uint16_t *) key_name,
		          key_name_size,
		          error );
#else
		result = libcreg_key_get_utf8_name(
		          key,
		          (uint8_t *) key_name,
		          key_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key name.",
			 function );

			goto on_error;
		}
		key_name_length = key_name_size - 1;

		if( info_handle_name_value_fprint(
		     info_handle,
		     key_path,
		     key_path_length,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print key path string.",
			 function );

			goto on_error;
		}
		if( key_name != NULL )
		{
			if( info_handle_name_value_fprint(
			     info_handle,
			     key_name,
			     key_name_length,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print key name string.",
				 function );

				goto on_error;
			}
		}
		fprintf(
		 info_handle->notify_stream,
		 "\n" );
	}
	if( libcreg_key_get_number_of_values(
	     key,
	     &number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of values.",
		 function );

		goto on_error;
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libcreg_key_get_value_by_index(
		     key,
		     value_index,
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcreg_value_get_utf16_name_size(
		          value,
		          &value_name_size,
		          error );
#else
		result = libcreg_value_get_utf8_name_size(
		          value,
		          &value_name_size,
		          error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value name size.",
			 function );

			goto on_error;
		}
		if( value_name_size > 0 )
		{
			if( value_name_size > ( (size_t) MEMORY_MAXIMUM_ALLOCATION_SIZE / sizeof( system_character_t ) ) )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
				 "%s: invalid value name size value exceeds maximum.",
				 function );

				goto on_error;
			}
			value_name = system_string_allocate(
			              value_name_size );

			if( value_name == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
				 "%s: unable to create value name string.",
				 function );

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libcreg_value_get_utf16_name(
			          value,
			          (uint16_t *) value_name,
			          value_name_size,
			          error );
#else
			result = libcreg_value_get_utf8_name(
			          value,
			          (uint8_t *) value_name,
			          value_name_size,
			          error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value name.",
				 function );

				goto on_error;
			}
			fprintf(
			 info_handle->notify_stream,
			 "\t%" PRIs_SYSTEM "\n",
			 value_name );

			memory_free(
			 value_name );

			value_name = NULL;
		}
		else
		{
			fprintf(
			 info_handle->notify_stream,
			 "\t(default)\n" );
		}
		if( libcreg_value_free(
		     &value,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value: %d.",
			 function,
			 value_index );

			goto on_error;
		}
	}
	if( libcreg_key_get_number_of_sub_keys(
	     key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys.",
		 function );

		goto on_error;
	}
	if( number_of_sub_keys > 0 )
	{
		sub_key_path_size = key_path_length + 1;

		if( key_name != NULL )
		{
			sub_key_path_size += key_name_size;
		}
		sub_key_path = system_string_allocate(
		                sub_key_path_size );

		if( sub_key_path == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create sub key path.",
			 function );

			goto on_error;
		}
		if( system_string_copy(
		     sub_key_path,
		     key_path,
		     key_path_length ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy key path to sub key path.",
			 function );

			goto on_error;
		}
		if( key_name != NULL )
		{
			if( system_string_copy(
			     &( sub_key_path[ key_path_length ] ),
			     key_name,
			     key_name_size - 1 ) == NULL )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_MEMORY,
				 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
				 "%s: unable to copy key name to sub key path.",
				 function );

				goto on_error;
			}
			sub_key_path[ sub_key_path_size - 2 ] = (system_character_t) LIBCREG_SEPARATOR;
		}
		sub_key_path[ sub_key_path_size - 1 ] = (system_character_t) 0;

		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libcreg_key_get_sub_key_by_index(
			     key,
			     sub_key_index,
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( info_handle_key_fprint(
			     info_handle,
			     sub_key,
			     sub_key_path,
			     sub_key_path_size - 1,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
				 "%s: unable to print sub key: %d info.",
				 function,
				 sub_key_index );

				goto on_error;
			}
			if( libcreg_key_free(
			     &sub_key,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				goto on_error;
			}
		}
		if( sub_key_path != NULL )
		{
			memory_free(
			 sub_key_path );

			sub_key_path = NULL;
		}
	}
	if( key_name != NULL )
	{
		memory_free(
		 key_name );

		key_name = NULL;
	}
	return( 1 );

on_error:
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	if( sub_key_path != NULL )
	{
		memory_free(
		 sub_key_path );
	}
	if( value_name != NULL )
	{
		memory_free(
		 value_name );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key_name != NULL )
	{
		memory_free(
		 key_name );
	}
	return( -1 );
}

/* Prints the key and value hierarchy information
 * Returns 1 if successful or -1 on error
 */
int info_handle_key_value_hierarchy_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	libcreg_key_t *root_key = NULL;
	static char *function   = "info_handle_file_fprint";
	int result              = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Windows 9x/Me Registry File information:\n\n" );

	fprintf(
	 info_handle->notify_stream,
	 "Key and value hierarchy:\n" );

	result = libcreg_file_get_root_key(
	          info_handle->input_file,
	          &root_key,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root key.",
		 function );

		goto on_error;
	}
	else if( result != 0 )
	{
		if( info_handle_key_fprint(
		     info_handle,
		     root_key,
		     _SYSTEM_STRING( "" ),
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print root key info.",
			 function );

			goto on_error;
		}
		if( libcreg_key_free(
		     &root_key,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free root key.",
			 function );

			goto on_error;
		}
	}
	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	return( 1 );

on_error:
	if( root_key != NULL )
	{
		libcreg_key_free(
		 &root_key,
		 NULL );
	}
	return( -1 );
}

/* Prints the file information
 * Returns 1 if successful or -1 on error
 */
int info_handle_file_fprint(
     info_handle_t *info_handle,
     libcerror_error_t **error )
{
	static char *function  = "info_handle_file_fprint";
	uint16_t major_version = 0;
	uint16_t minor_version = 0;
	int is_corrupted       = 0;

	if( info_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid info handle.",
		 function );

		return( -1 );
	}
	if( libcreg_file_get_format_version(
	     info_handle->input_file,
	     &major_version,
	     &minor_version,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve format version.",
		 function );

		return( -1 );
	}
	fprintf(
	 info_handle->notify_stream,
	 "Windows 9x/Me Registry File information:\n" );

	fprintf(
	 info_handle->notify_stream,
	 "\tVersion:\t%" PRIu16 ".%" PRIu16 "\n",
	 major_version,
	 minor_version );

/* TODO calculate number of keys and values */

	fprintf(
	 info_handle->notify_stream,
	 "\n" );

	is_corrupted = libcreg_file_is_corrupted(
	                info_handle->input_file,
	                error );

	if( is_corrupted == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if file is corrupted.",
		 function );

		return( -1 );
	}
	if( is_corrupted != 0 )
	{
		fprintf(
		 info_handle->notify_stream,
		 "File is corrupted\n\n" );
	}
	return( 1 );
}
