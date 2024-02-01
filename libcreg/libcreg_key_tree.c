/*
 * Key tree functions
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

#if defined( HAVE_WCTYPE_H ) || defined( HAVE_WINAPI )
#include <wctype.h>
#endif

#include "libcreg_definitions.h"
#include "libcreg_key.h"
#include "libcreg_key_descriptor.h"
#include "libcreg_key_item.h"
#include "libcreg_key_navigation.h"
#include "libcreg_key_tree.h"
#include "libcreg_libcerror.h"
#include "libcreg_libuna.h"
#include "libcreg_types.h"

/* Retrieves the key for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libcreg_key_tree_get_sub_key_by_utf8_path(
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     uint32_t key_offset,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_key_descriptor_t *sub_key_descriptor = NULL;
	libcreg_key_item_t *sub_key_item             = NULL;
	uint8_t *utf8_string_segment                 = NULL;
	static char *function                        = "libcreg_key_tree_get_sub_key_by_utf8_path";
	libuna_unicode_character_t unicode_character = 0;
	size_t utf8_string_index                     = 0;
	size_t utf8_string_segment_length            = 0;
	uint32_t name_hash                           = 0;
	uint32_t sub_key_offset                      = 0;
	int result                                   = 0;

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
	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
	sub_key_offset = key_offset;

	if( utf8_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf8_string[ utf8_string_index ] == (uint8_t) LIBCREG_SEPARATOR )
		{
			utf8_string_index++;
		}
	}
	/* If the string is empty return the current key
	 */
	if( utf8_string_length == utf8_string_index )
	{
		result = 1;
	}
	else while( utf8_string_index < utf8_string_length )
	{
		utf8_string_segment        = (uint8_t *) &( utf8_string[ utf8_string_index ] );
		utf8_string_segment_length = utf8_string_index;
		name_hash                  = 0;

		while( utf8_string_index < utf8_string_length )
		{
			if( libuna_unicode_character_copy_from_utf8(
			     &unicode_character,
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

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBCREG_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf8_string_segment_length += 1;

				break;
			}
			name_hash *= 37;
			name_hash += (uint32_t) towupper( (wint_t) unicode_character );
		}
		utf8_string_segment_length = utf8_string_index - utf8_string_segment_length;

		if( utf8_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
/* TODO: instead of key item directory read key descriptors ? */
			if( libcreg_key_item_initialize(
			     &sub_key_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub key item.",
				 function );

				goto on_error;
			}
			if( libcreg_key_item_read(
			     sub_key_item,
			     file_io_handle,
			     key_navigation,
			     sub_key_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read sub key item at offset: %" PRIu32 " (0x%08" PRIx32 ").",
				 function,
				 sub_key_offset,
				 sub_key_offset );

				goto on_error;
			}
			result = libcreg_key_item_get_sub_key_descriptor_by_utf8_name(
			          sub_key_item,
			          file_io_handle,
			          key_navigation,
				  utf8_string_segment,
				  utf8_string_segment_length,
				  ascii_codepage,
			          &sub_key_descriptor,
			          error );

			if( result == 1 )
			{
				sub_key_offset = sub_key_descriptor->key_offset;
			}
			if( libcreg_key_item_free(
			     &sub_key_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to free sub key item.",
				 function );

				goto on_error;
			}
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key values by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
	}
	if( result != 0 )
	{
		if( libcreg_key_initialize(
		     sub_key,
		     io_handle,
		     file_io_handle,
		     key_navigation,
		     sub_key_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub key.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( sub_key_item != NULL )
	{
		libcreg_key_item_free(
		 &sub_key_item,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the key for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Creates a new key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libcreg_key_tree_get_sub_key_by_utf16_path(
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     uint32_t key_offset,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_key_descriptor_t *sub_key_descriptor = NULL;
	libcreg_key_item_t *sub_key_item             = NULL;
	uint16_t *utf16_string_segment               = NULL;
	static char *function                        = "libcreg_key_tree_get_sub_key_by_utf16_path";
	libuna_unicode_character_t unicode_character = 0;
	size_t utf16_string_index                    = 0;
	size_t utf16_string_segment_length           = 0;
	uint32_t name_hash                           = 0;
	uint32_t sub_key_offset                      = 0;
	int result                                   = 0;

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
	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
	sub_key_offset = key_offset;

	if( utf16_string_length > 0 )
	{
		/* Ignore a leading separator
		 */
		if( utf16_string[ utf16_string_index ] == (uint16_t) LIBCREG_SEPARATOR )
		{
			utf16_string_index++;
		}
	}
	/* If the string is empty return the current key
	 */
	if( utf16_string_length == utf16_string_index )
	{
		result = 1;
	}
	else while( utf16_string_index < utf16_string_length )
	{
		utf16_string_segment        = (uint16_t *) &( utf16_string[ utf16_string_index ] );
		utf16_string_segment_length = utf16_string_index;
		name_hash                   = 0;

		while( utf16_string_index < utf16_string_length )
		{
			if( libuna_unicode_character_copy_from_utf16(
			     &unicode_character,
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

				goto on_error;
			}
			if( ( unicode_character == (libuna_unicode_character_t) LIBCREG_SEPARATOR )
			 || ( unicode_character == 0 ) )
			{
				utf16_string_segment_length += 1;

				break;
			}
			name_hash *= 37;
			name_hash += (uint32_t) towupper( (wint_t) unicode_character );
		}
		utf16_string_segment_length = utf16_string_index - utf16_string_segment_length;

		if( utf16_string_segment_length == 0 )
		{
			result = 0;
		}
		else
		{
/* TODO: instead of key item directory read key descriptors ? */
			if( libcreg_key_item_initialize(
			     &sub_key_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to create sub key item.",
				 function );

				goto on_error;
			}
			if( libcreg_key_item_read(
			     sub_key_item,
			     file_io_handle,
			     key_navigation,
			     sub_key_offset,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read sub key item at offset: %" PRIu32 " (0x%08" PRIx32 ").",
				 function,
				 sub_key_offset,
				 sub_key_offset );

				goto on_error;
			}
			result = libcreg_key_item_get_sub_key_descriptor_by_utf16_name(
			          sub_key_item,
			          file_io_handle,
			          key_navigation,
				  utf16_string_segment,
				  utf16_string_segment_length,
				  ascii_codepage,
			          &sub_key_descriptor,
			          error );

			if( result == 1 )
			{
				sub_key_offset = sub_key_descriptor->key_offset;
			}
			if( libcreg_key_item_free(
			     &sub_key_item,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
				 "%s: unable to free sub key item.",
				 function );

				goto on_error;
			}
		}
		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key values by name.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			break;
		}
	}
	if( result != 0 )
	{
		if( libcreg_key_initialize(
		     sub_key,
		     io_handle,
		     file_io_handle,
		     key_navigation,
		     sub_key_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub key.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( sub_key_item != NULL )
	{
		libcreg_key_item_free(
		 &sub_key_item,
		 NULL );
	}
	return( -1 );
}

