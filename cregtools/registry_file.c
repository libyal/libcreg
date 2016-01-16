/*
 * Registry file
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
#include <memory.h>
#include <types.h>

#include "cregtools_libcerror.h"
#include "cregtools_libcstring.h"
#include "cregtools_libcreg.h"
#include "registry_file.h"

/* Creates a registry file
 * Make sure the value registry_file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int registry_file_initialize(
     registry_file_t **registry_file,
     libcerror_error_t **error )
{
	static char *function = "registry_file_initialize";

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
	if( *registry_file != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid registry file value already set.",
		 function );

		return( -1 );
	}
	*registry_file = memory_allocate_structure(
	                  registry_file_t );

	if( *registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create registry file.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *registry_file,
	     0,
	     sizeof( registry_file_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear registry file.",
		 function );

		memory_free(
		 *registry_file );

		*registry_file = NULL;

		return( -1 );
	}
	if( libcreg_file_initialize(
	     &( ( *registry_file )->creg_file ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize CREG file.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *registry_file != NULL )
	{
		memory_free(
		 *registry_file );

		*registry_file = NULL;
	}
	return( -1 );
}

/* Frees a registry file
 * Returns 1 if successful or -1 on error
 */
int registry_file_free(
     registry_file_t **registry_file,
     libcerror_error_t **error )
{
	static char *function = "registry_file_free";
	int result            = 1;

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
	if( *registry_file != NULL )
	{
		if( ( *registry_file )->is_open != 0 )
		{
			if( registry_file_close(
			     *registry_file,
			     error ) != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_CLOSE_FAILED,
				 "%s: unable to close registry file.",
				 function );

				result = -1;
			}
		}
		if( libcreg_file_free(
		     &( ( *registry_file )->creg_file ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free CREG file.",
			 function );

			result = -1;
		}
		memory_free(
		 *registry_file );

		*registry_file = NULL;
	}
	return( result );
}

/* Signals the registry file to abort
 * Returns 1 if successful or -1 on error
 */
int registry_file_signal_abort(
     registry_file_t *registry_file,
     libcerror_error_t **error )
{
	static char *function = "registry_file_signal_abort";

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
	if( registry_file->creg_file != NULL )
	{
		if( libcreg_file_signal_abort(
		     registry_file->creg_file,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to signal CREG file to abort.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Sets the ascii codepage
 * Returns 1 if successful or -1 on error
 */
int registry_file_set_ascii_codepage(
     registry_file_t *registry_file,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function  = "registry_file_set_ascii_codepage";

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
	if( libcreg_file_set_ascii_codepage(
	     registry_file->creg_file,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set ASCII codepage in CREG file.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Opens the registry file
 * Returns 1 if successful or -1 on error
 */
int registry_file_open(
     registry_file_t *registry_file,
     const libcstring_system_character_t *filename,
     libcerror_error_t **error )
{
	libcstring_system_character_t *name = NULL;
	libcreg_key_t *sub_key              = NULL;
	libcreg_value_t *value              = NULL;
	static char *function               = "registry_file_open";
	const char *sub_key_path            = NULL;
	const char *value_name              = NULL;
	size_t name_size                    = 0;
	size_t sub_key_path_length          = 0;
	size_t value_name_length            = 0;
	int number_of_sub_keys              = 0;
	int result                          = 0;

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
	if( registry_file->is_open != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid registry file already open.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	if( libcreg_file_open_wide(
	     registry_file->creg_file,
	     filename,
	     LIBCREG_OPEN_READ,
	     error ) != 1 )
#else
	if( libcreg_file_open(
	     registry_file->creg_file,
	     filename,
	     LIBCREG_OPEN_READ,
	     error ) != 1 )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_OPEN_FAILED,
		 "%s: unable to open CREG file.",
		 function );

		goto on_error;
	}
	if( libcreg_file_get_root_key(
	     registry_file->creg_file,
	     &( registry_file->root_key ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve root key.",
		 function );

		goto on_error;
	}
	if( libcreg_key_get_number_of_sub_keys(
	     registry_file->root_key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys of root key.",
		 function );

		goto on_error;
	}
	result = 0;

	if( number_of_sub_keys == 1 )
	{
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
		result = libcreg_key_get_utf16_name_size(
		          registry_file->root_key,
		          &name_size,
		          error );
#else
		result = libcreg_key_get_utf8_name_size(
		          registry_file->root_key,
		          &name_size,
		          error );
#endif
		if( ( result != 1 )
		 || ( name_size == 0 ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root key name size.",
			 function );

			goto on_error;
		}
		if( ( name_size > (size_t) SSIZE_MAX )
		 || ( ( sizeof( libcstring_system_character_t ) * name_size ) > (size_t) SSIZE_MAX ) )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
			 "%s: invalid name size value exceeds maximum.",
			 function );

			goto on_error;
		}
		name = libcstring_system_string_allocate(
			name_size );

		if( name == NULL )
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
		result = libcreg_key_get_utf16_name(
			  registry_file->root_key,
			  (uint16_t *) name,
			  name_size,
			  error );
#else
		result = libcreg_key_get_utf8_name(
			  registry_file->root_key,
			  (uint8_t *) name,
			  name_size,
			  error );
#endif
		if( result != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve root key name.",
			 function );

			goto on_error;
		}
		result = 0;

/* TODO what about Windows NT4 */
		if( name_size == 13 )
		{
			/* Root key used by Windows 2000, XP, 2003
			 */
			if( libcstring_system_string_compare_no_case(
			     name,
			     _LIBCSTRING_SYSTEM_STRING( "$$$PROTO.HIV" ),
			     12 ) == 0 )
			{
				result = 1;
			}
		}
		else if( name_size == 53 )
		{
			/* Root key used by Windows Vista, 2008, 7
			 */
			if( libcstring_system_string_compare_no_case(
			     name,
			     _LIBCSTRING_SYSTEM_STRING( "CMI-CreateHive{" ),
			     15 ) == 0 )
			{
				if( name[ 51 ] == (libcstring_system_character_t) '}' )
				{
					result = 1;
				}
			}
		}
		else if( name_size == 58 )
		{
			/* Root key used by Windows 8
			 */
			if( libcstring_system_string_compare_no_case(
			     name,
			     _LIBCSTRING_SYSTEM_STRING( "CsiTool-CreateHive-{" ),
			     20 ) == 0 )
			{
				if( name[ 56 ] == (libcstring_system_character_t) '}' )
				{
					result = 1;
				}
			}
		}
		memory_free(
		 name );

		name = NULL;
	}
	if( result != 0 )
	{
		if( libcreg_key_get_sub_key(
		     registry_file->root_key,
		     0,
		     &( registry_file->base_key ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve base key.",
			 function );

			goto on_error;
		}
	}
	else
	{
		/* E.g. for the SAM registry file the root key is the base
		 */
		registry_file->base_key = registry_file->root_key;
	}
	/* Determine the registry file type based on the name of the base key
	 */
/* TODO refactor to separate function */
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcreg_key_get_utf16_name_size(
		  registry_file->base_key,
		  &name_size,
		  error );
#else
	result = libcreg_key_get_utf8_name_size(
		  registry_file->base_key,
		  &name_size,
		  error );
#endif
	if( ( result != 1 )
	 || ( name_size == 0 ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve base key name size.",
		 function );

		goto on_error;
	}
	if( ( name_size > (size_t) SSIZE_MAX )
	 || ( ( sizeof( libcstring_system_character_t ) * name_size ) > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid name size value exceeds maximum.",
		 function );

		goto on_error;
	}
	name = libcstring_system_string_allocate(
		name_size );

	if( name == NULL )
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
	result = libcreg_key_get_utf16_name(
		  registry_file->base_key,
		  (uint16_t *) name,
		  name_size,
		  error );
#else
	result = libcreg_key_get_utf8_name(
		  registry_file->base_key,
		  (uint8_t *) name,
		  name_size,
		  error );
#endif
	if( result != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve base key name.",
		 function );

		goto on_error;
	}
/* TODO add more types */
	if( name_size == 4 )
	{
		if( libcstring_system_string_compare_no_case(
		     name,
		     _LIBCSTRING_SYSTEM_STRING( "SAM" ),
		     3 ) == 0 )
		{
			registry_file->type = REGISTRY_FILE_TYPE_SAM;
		}
	}
	memory_free(
	 name );

	name = NULL;
/* TODO still needed ? */
	if( libcreg_key_get_number_of_sub_keys(
	     registry_file->base_key,
	     &number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub keys of base key.",
		 function );

		goto on_error;
	}
/* TODO refactor to separate function */
	/* Get the current control set from:
	 * SYSTEM\Select\Current
	 */
	sub_key_path = "Select";

	sub_key_path_length = libcstring_narrow_string_length(
	                       sub_key_path );

	result = libcreg_key_get_sub_key_by_utf8_path(
		  registry_file->base_key,
		  (uint8_t *) sub_key_path,
		  sub_key_path_length,
		  &sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %s.",
		 function,
		 sub_key_path );

		goto on_error;
	}
	else if( result != 0 )
	{
		value_name = "Current";

		value_name_length = libcstring_narrow_string_length(
		                     value_name );

		result = libcreg_key_get_value_by_utf8_name(
			  sub_key,
			  (uint8_t *) value_name,
			  value_name_length,
			  &value,
			  error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value: %s.",
			 function,
			 value_name );

			goto on_error;
		}
		else if( result != 0 )
		{
			if( libcreg_value_get_value_32bit(
			     value,
			     &( registry_file->current_control_set ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve 32-bit value: %s.",
				 function,
				 value_name );

				goto on_error;
			}
			if( libcreg_value_free(
			     &value,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				goto on_error;
			}
		}
	}
	if( libcreg_key_free(
	     &sub_key,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free sub key.",
		 function );

		goto on_error;
	}
	/* Retrieve the control set 1 key: SYSTEM\ControlSet001
	 */
	sub_key_path = "ControlSet001";

	sub_key_path_length = libcstring_narrow_string_length(
	                       sub_key_path );

	result = libcreg_key_get_sub_key_by_utf8_path(
		  registry_file->base_key,
		  (uint8_t *) sub_key_path,
		  sub_key_path_length,
		  &( registry_file->control_set1_key ),
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %s.",
		 function,
		 sub_key_path );

		goto on_error;
	}
	/* Retrieve the control set 2 key: SYSTEM\ControlSet002
	 */
	sub_key_path = "ControlSet002";

	sub_key_path_length = libcstring_narrow_string_length(
	                       sub_key_path );

	result = libcreg_key_get_sub_key_by_utf8_path(
		  registry_file->base_key,
		  (uint8_t *) sub_key_path,
		  sub_key_path_length,
		  &( registry_file->control_set2_key ),
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key: %s.",
		 function,
		 sub_key_path );

		goto on_error;
	}
	if( ( registry_file->current_control_set != 0 )
	 || ( registry_file->control_set1_key != NULL )
	 || ( registry_file->control_set2_key != NULL ) )
	{
		if( ( registry_file->current_control_set != 1 )
		 && ( registry_file->current_control_set != 2 ) )
		{
/* TODO print debug notification */
			registry_file->current_control_set = 1;
		}
		if( ( registry_file->current_control_set == 1 )
		 && ( registry_file->control_set1_key != NULL ) )
		{
			registry_file->current_control_set_key = registry_file->control_set1_key;
		}
		else if( ( registry_file->current_control_set == 2 )
		      && ( registry_file->control_set2_key != NULL ) )
		{
			registry_file->current_control_set_key = registry_file->control_set2_key;
		}
	}
	registry_file->is_open = 1;

	return( 1 );

on_error:
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	if( registry_file->control_set2_key != NULL )
	{
		libcreg_key_free(
		 &( registry_file->control_set2_key ),
		 NULL );
	}
	if( registry_file->control_set1_key != NULL )
	{
		libcreg_key_free(
		 &( registry_file->control_set1_key ),
		 NULL );
	}
	if( ( registry_file->base_key != NULL )
	 && ( registry_file->base_key != registry_file->root_key ) )
	{
		libcreg_key_free(
		 &( registry_file->base_key ),
		 NULL );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	if( registry_file->root_key != NULL )
	{
		libcreg_key_free(
		 &( registry_file->root_key ),
		 NULL );
	}
	libcreg_file_close(
	 registry_file->creg_file,
	 NULL );

	return( -1 );
}

/* Closes the registry file
 * Returns the 0 if succesful or -1 on error
 */
int registry_file_close(
     registry_file_t *registry_file,
     libcerror_error_t **error )
{
	static char *function = "registry_file_close";
	int result            = 0;

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
	if( registry_file->is_open != 0 )
	{
		if( registry_file->control_set2_key != NULL )
		{
			if( libcreg_key_free(
			     &( registry_file->control_set2_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free control set 2 key.",
				 function );

				result = -1;
			}
		}
		if( registry_file->control_set1_key != NULL )
		{
			if( libcreg_key_free(
			     &( registry_file->control_set1_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free control set 1 key.",
				 function );

				result = -1;
			}
		}
		if( ( registry_file->base_key != NULL )
		 && ( registry_file->base_key != registry_file->root_key ) )
		{
			if( libcreg_key_free(
			     &( registry_file->base_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free base key.",
				 function );

				result = -1;
			}
		}
		if( registry_file->root_key != NULL )
		{
			if( libcreg_key_free(
			     &( registry_file->root_key ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free root key.",
				 function );

				result = -1;
			}
		}
		if( libcreg_file_close(
		     registry_file->creg_file,
		     error ) != 0 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_CLOSE_FAILED,
			 "%s: unable to close CREG file.",
			 function );

			result = -1;
		}
		registry_file->is_open = 0;
	}
	return( result );
}

/* Retrieves the key specified by the path
 * Returns 1 if successful or -1 on error
 */
int registry_file_get_key_by_path(
     registry_file_t *registry_file,
     const libcstring_system_character_t *key_path,
     size_t key_path_length,
     libcreg_key_t **key,
     libcerror_error_t **error )
{
	static char *function = "registry_file_get_key_by_path";
	int result            = 0;

	if( registry_file == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid registry file.",
		 function );

		return( -1 );
	}
#if defined( LIBCSTRING_HAVE_WIDE_SYSTEM_CHARACTER )
	result = libcreg_key_get_sub_key_by_utf16_path(
	          registry_file->base_key,
	          (uint16_t *) key_path,
	          key_path_length,
	          key,
	          error );
#else
	result = libcreg_key_get_sub_key_by_utf8_path(
	          registry_file->base_key,
	          (uint8_t *) key_path,
	          key_path_length,
	          key,
	          error );
#endif
	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by path: %" PRIs_LIBCSTRING_SYSTEM ".",
		 function,
		 key_path );

		return( -1 );
	}
	return( result );
}

