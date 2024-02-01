/*
 * Key item values functions
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

#if defined( HAVE_WCTYPE_H ) || defined( HAVE_WINAPI )
#include <wctype.h>
#endif

#include "libcreg_data_block.h"
#include "libcreg_definitions.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_descriptor.h"
#include "libcreg_key_hierarchy_entry.h"
#include "libcreg_key_item.h"
#include "libcreg_key_name_entry.h"
#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcdata.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_libuna.h"
#include "libcreg_value_entry.h"

#include "creg_key_navigation.h"

/* Creates a key item
 * Make sure the value key_item is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_initialize(
     libcreg_key_item_t **key_item,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_initialize";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( *key_item != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item value already set.",
		 function );

		return( -1 );
	}
	*key_item = memory_allocate_structure(
	             libcreg_key_item_t );

	if( *key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key item.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_item,
	     0,
	     sizeof( libcreg_key_item_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key item.",
		 function );

		memory_free(
		 *key_item );

		*key_item = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *key_item )->sub_key_descriptors_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub key descriptors array.",
		 function );

		goto on_error;
	}
	if( libcdata_range_list_initialize(
	     &( ( *key_item )->sub_key_range_list ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create sub key range list.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_item != NULL )
	{
		if( ( *key_item )->sub_key_descriptors_array != NULL )
		{
			libcdata_array_free(
			 &( ( *key_item )->sub_key_descriptors_array ),
			 NULL,
			 NULL );
		}
		memory_free(
		 *key_item );

		*key_item = NULL;
	}
	return( -1 );
}

/* Frees a key item
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_free(
     libcreg_key_item_t **key_item,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_free";
	int result            = 1;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( *key_item != NULL )
	{
		/* The key_name_entry reference is freed elsewhere */

		if( libcdata_array_free(
		     &( ( *key_item )->sub_key_descriptors_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_key_descriptor_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key descriptors array.",
			 function );

			result = -1;
		}
		if( libcdata_range_list_free(
		     &( ( *key_item )->sub_key_range_list ),
		     NULL,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key range list.",
			 function );

			result = -1;
		}
		memory_free(
		 *key_item );

		*key_item = NULL;
	}
	return( result );
}

/* Reads a key item
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_read(
     libcreg_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     off64_t key_offset,
     libcerror_error_t **error )
{
	libcreg_data_block_t *data_block                   = NULL;
	libcreg_key_descriptor_t *sub_key_descriptor       = NULL;
	libcreg_key_hierarchy_entry_t *key_hierarchy_entry = NULL;
	static char *function                              = "libcreg_key_item_read";
	off64_t sub_key_offset                             = 0;
	int entry_index                                    = 0;
	int recursion_depth                                = 0;
	int result                                         = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key item - key name entry value already set.",
		 function );

		return( -1 );
	}
	if( key_navigation == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key navigation.",
		 function );

		return( -1 );
	}
	if( key_navigation->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key navigation - missing IO handle.",
		 function );

		return( -1 );
	}
	if( libcreg_key_navigation_get_key_hierarchy_entry_at_offset(
	     key_navigation,
	     file_io_handle,
	     key_offset,
	     &key_hierarchy_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key hierarchy entry for offset: %" PRIi64 ".",
		 function,
		 key_offset );

		goto on_error;
	}
	if( key_hierarchy_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: missing key hierarchy entry for offset: %" PRIi64 ".",
		 function,
		 key_offset );

		goto on_error;
	}
	if( (int16_t) key_hierarchy_entry->data_block_number >= 0 )
	{
		if( libcreg_key_navigation_get_data_block_at_index(
		     key_navigation,
		     file_io_handle,
		     (int) key_hierarchy_entry->data_block_number,
		     &data_block,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to retrieve data block number: %" PRIu16 ".",
			 function,
			 key_hierarchy_entry->data_block_number );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: offset\t\t\t\t\t: %" PRIzd " (0x%08" PRIzx ")\n",
			 function,
			 key_hierarchy_entry->data_offset,
			 key_hierarchy_entry->data_offset );

			libcnotify_printf(
			 "%s: key name entry number\t\t\t: %" PRIi16 "\n",
			 function,
			 (int16_t) key_hierarchy_entry->key_name_entry_number );

			libcnotify_printf(
			 "%s: data block number\t\t\t: %" PRIi16 "\n",
			 function,
			 (int16_t) key_hierarchy_entry->data_block_number );

			libcnotify_printf(
			 "\n" );
		}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

		if( libcreg_data_block_get_entry_by_identifier(
		     data_block,
		     key_hierarchy_entry->key_name_entry_number,
		     &( key_item->key_name_entry ),
		     key_navigation->io_handle->ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to retrieve key entry element number: %" PRIu16 ".",
			 function,
			 key_hierarchy_entry->key_name_entry_number );

			goto on_error;
		}
		if( key_item->key_name_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing key name entry: %" PRIu16 ".",
			 function,
			 key_hierarchy_entry->key_name_entry_number );

			goto on_error;
		}
	}
	sub_key_offset = key_hierarchy_entry->sub_key_offset;

	while( ( sub_key_offset != 0 )
	    && ( sub_key_offset != 0xffffffffUL ) )
	{
		if( recursion_depth > LIBCREG_MAXIMUM_SUB_KEY_RECURSION_DEPTH )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid recursion depth value out of bounds.",
			 function );

			goto on_error;
		}
		if( libcreg_key_navigation_get_key_hierarchy_entry_at_offset(
		     key_navigation,
		     file_io_handle,
		     sub_key_offset,
		     &key_hierarchy_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key hierarchy entry for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 sub_key_offset,
			 sub_key_offset );

			goto on_error;
		}
		if( key_hierarchy_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing key hierarchy entry for offset: %" PRIi64 " (0x%08" PRIx64 ").",
			 function,
			 sub_key_offset,
			 sub_key_offset );

			goto on_error;
		}
		if( libcreg_key_descriptor_initialize(
		     &sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create sub key descriptor.",
			 function );

			goto on_error;
		}
		sub_key_descriptor->key_offset = (uint32_t) sub_key_offset;

		if( libcdata_array_append_entry(
		     key_item->sub_key_descriptors_array,
		     &entry_index,
		     (intptr_t *) sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sub key descriptor to array.",
			 function );

			goto on_error;
		}
		sub_key_descriptor = NULL;

		if( libcdata_range_list_insert_range(
		     key_item->sub_key_range_list,
		     (uint64_t) sub_key_offset,
		     (uint64_t) sizeof( creg_key_hierarchy_entry_t ),
		     NULL,
		     NULL,
		     NULL,
		     error ) == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to insert sub key navigation record range into range list.",
			 function );

			goto on_error;
		}
		if( ( key_hierarchy_entry->next_key_offset != 0 )
		 && ( key_hierarchy_entry->next_key_offset != 0xffffffffUL ) )
		{
			result = libcdata_range_list_range_has_overlapping_range(
			          key_item->sub_key_range_list,
			          (uint64_t) key_hierarchy_entry->next_key_offset,
			          (uint64_t) sizeof( creg_key_hierarchy_entry_t ),
			          error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to determine if there is an overlapping key navigation record range in the range list.",
				 function );

				goto on_error;
			}
			else if( result != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
				 "%s: invalid next key navigation record offset: %" PRIu32 " (0x%08" PRIu32 ") value already read.",
				 function,
				 key_hierarchy_entry->next_key_offset,
				 key_hierarchy_entry->next_key_offset );

				goto on_error;
			}
		}
		sub_key_offset = (off64_t) key_hierarchy_entry->next_key_offset;

		recursion_depth++;
	}
	return( 1 );

on_error:
	if( sub_key_descriptor != NULL )
	{
		libcreg_key_descriptor_free(
		 &sub_key_descriptor,
		 NULL );
	}
	return( -1 );
}

/* Determines if the key item is corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libcreg_key_item_is_corrupted(
     libcreg_key_item_t *key_item,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_is_corrupted";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( key_item->item_flags & LIBCREG_ITEM_FLAG_IS_CORRUPTED ) != 0 )
	{
		return( 1 );
	}
	return( 0 );
}

/* Retrieves the key name size
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_name_size(
     libcreg_key_item_t *key_item,
     size_t *name_size,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
	{
		if( name_size == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid name size.",
			 function );

			return( -1 );
		}
		*name_size = 0;
	}
	else
	{
		if( libcreg_key_name_entry_get_name_size(
		     key_item->key_name_entry,
		     name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the key name
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_name(
     libcreg_key_item_t *key_item,
     uint8_t *name,
     size_t name_size,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
	{
		if( name == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid name.",
			 function );

			return( -1 );
		}
		if( name_size < 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid name size value out of bounds.",
			 function );

			return( -1 );
		}
		name[ 0 ] = 0;
	}
	else
	{
		if( libcreg_key_name_entry_get_name(
		     key_item->key_name_entry,
		     name,
		     name_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve name.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_utf8_name_size(
     libcreg_key_item_t *key_item,
     size_t *utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_utf8_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
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
		if( libcreg_key_name_entry_get_utf8_name_size(
		     key_item->key_name_entry,
		     utf8_string_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-8 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_utf8_name(
     libcreg_key_item_t *key_item,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_utf8_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
	{
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
		if( utf8_string_size < 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-8 string size value out of bounds.",
			 function );

			return( -1 );
		}
		utf8_string[ 0 ] = 0;
	}
	else
	{
		if( libcreg_key_name_entry_get_utf8_name(
		     key_item->key_name_entry,
		     utf8_string,
		     utf8_string_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-8 name.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_utf16_name_size(
     libcreg_key_item_t *key_item,
     size_t *utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_utf16_name_size";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
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
		if( libcreg_key_name_entry_get_utf16_name_size(
		     key_item->key_name_entry,
		     utf16_string_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name size.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the UTF-16 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_utf16_name(
     libcreg_key_item_t *key_item,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     int ascii_codepage,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_utf16_name";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
	{
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
		if( utf16_string_size < 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
			 "%s: invalid UTF-16 string size value out of bounds.",
			 function );

			return( -1 );
		}
		utf16_string[ 0 ] = 0;
	}
	else
	{
		if( libcreg_key_name_entry_get_utf16_name(
		     key_item->key_name_entry,
		     utf16_string,
		     utf16_string_size,
		     ascii_codepage,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve UTF-16 name.",
			 function );

			return( -1 );
		}
	}
	return( 1 );
}

/* Retrieves the number of value entries
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_number_of_value_entries(
     libcreg_key_item_t *key_item,
     int *number_of_value_entries,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_number_of_value_entries";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( key_item->key_name_entry == NULL )
	{
		if( number_of_value_entries == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid number of value entries.",
			 function );

			return( -1 );
		}
		*number_of_value_entries = 0;
	}
	else
	{
		if( libcreg_key_name_entry_get_number_of_entries(
		     key_item->key_name_entry,
		     number_of_value_entries,
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
	}
	return( 1 );
}

/* Retrieves a specific value entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_value_entry_by_index(
     libcreg_key_item_t *key_item,
     int value_index,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_value_entry_by_index";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libcreg_key_name_entry_get_entry_by_index(
	     key_item->key_name_entry,
	     value_index,
	     value_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value entry: %d.",
		 function,
		 value_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the value entry for the specific UTF-8 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcreg_key_item_get_value_by_utf8_name(
     libcreg_key_item_t *key_item,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	libcreg_value_entry_t *safe_value_entry      = NULL;
	static char *function                        = "libcreg_key_item_get_value_by_utf8_name";
	libuna_unicode_character_t unicode_character = 0;
	size_t utf8_string_index                     = 0;
	uint32_t name_hash                           = 0;
	int number_of_values                         = 0;
	int result                                   = 0;
	int value_index                              = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( utf8_string == NULL )
	 && ( utf8_string_length != 0 ) )
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
	if( libcreg_key_name_entry_get_number_of_entries(
	     key_item->key_name_entry,
	     &number_of_values,
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
	if( number_of_values == 0 )
	{
		return( 0 );
	}
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

			return( -1 );
		}
		name_hash *= 37;
		name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libcreg_key_name_entry_get_entry_by_index(
		     key_item->key_name_entry,
		     value_index,
		     &safe_value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value entry: %d.",
			 function,
			 value_index );

			return( -1 );
		}
		if( safe_value_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entry: %d.",
			 function,
			 value_index );

			return( -1 );
		}
		if( safe_value_entry->name == NULL )
		{
			if( ( utf8_string == NULL )
			 && ( utf8_string_length == 0 ) )
			{
				result = 1;
			}
		}
		else if( utf8_string == NULL )
		{
			continue;
		}
		else
		{
			result = libcreg_value_entry_compare_name_with_utf8_string(
				  safe_value_entry,
				  name_hash,
				  utf8_string,
				  utf8_string_length,
				  ascii_codepage,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare value name with UTF-8 string.",
				 function );

				return( -1 );
			}
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( value_index >= number_of_values )
	{
		return( 0 );
	}
	*value_entry = safe_value_entry;

	return( 1 );
}

/* Retrieves the value entry for the specific UTF-16 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcreg_key_item_get_value_by_utf16_name(
     libcreg_key_item_t *key_item,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcreg_value_entry_t **value_entry,
     libcerror_error_t **error )
{
	libcreg_value_entry_t *safe_value_entry      = NULL;
	static char *function                        = "libcreg_key_item_get_value_by_utf16_name";
	libuna_unicode_character_t unicode_character = 0;
	size_t utf16_string_index                    = 0;
	uint32_t name_hash                           = 0;
	int number_of_values                         = 0;
	int result                                   = 0;
	int value_index                              = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( ( utf16_string == NULL )
	 && ( utf16_string_length != 0 ) )
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
	if( libcreg_key_name_entry_get_number_of_entries(
	     key_item->key_name_entry,
	     &number_of_values,
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
	if( number_of_values == 0 )
	{
		return( 0 );
	}
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

			return( -1 );
		}
		name_hash *= 37;
		name_hash += (uint32_t) towupper( (wint_t) unicode_character );
	}
	for( value_index = 0;
	     value_index < number_of_values;
	     value_index++ )
	{
		if( libcreg_key_name_entry_get_entry_by_index(
		     key_item->key_name_entry,
		     value_index,
		     &safe_value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value entry: %d.",
			 function,
			 value_index );

			return( -1 );
		}
		if( safe_value_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing value entry: %d.",
			 function,
			 value_index );

			return( -1 );
		}
		if( safe_value_entry->name == NULL )
		{
			if( ( utf16_string == NULL )
			 && ( utf16_string_length == 0 ) )
			{
				result = 1;
			}
		}
		else if( utf16_string == NULL )
		{
			continue;
		}
		else
		{
			result = libcreg_value_entry_compare_name_with_utf16_string(
				  safe_value_entry,
				  name_hash,
				  utf16_string,
				  utf16_string_length,
				  ascii_codepage,
				  error );

			if( result == -1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GENERIC,
				 "%s: unable to compare value name with UTF-8 string.",
				 function );

				return( -1 );
			}
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( value_index >= number_of_values )
	{
		return( 0 );
	}
	*value_entry = safe_value_entry;

	return( 1 );
}

/* Retrieves the number of sub key descriptors
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_number_of_sub_key_descriptors(
     libcreg_key_item_t *key_item,
     int *number_of_sub_key_descriptors,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_number_of_sub_key_descriptors";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     key_item->sub_key_descriptors_array,
	     number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub key descriptors from array.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific sub key descriptor
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_get_sub_key_descriptor_by_index(
     libcreg_key_item_t *key_item,
     int sub_key_descriptor_index,
     libcreg_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_item_get_sub_key_descriptor_by_index";

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_entry_by_index(
	     key_item->sub_key_descriptors_array,
             sub_key_descriptor_index,
	     (intptr_t **) sub_key_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key descriptor: %d from array.",
		 function,
		 sub_key_descriptor_index );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the sub key descriptor for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcreg_key_item_get_sub_key_descriptor_by_utf8_name(
     libcreg_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libcreg_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error )
{
	libcreg_key_descriptor_t *safe_sub_key_descriptor = NULL;
	libcreg_key_item_t *sub_key_item                  = NULL;
	static char *function                             = "libcreg_key_item_get_sub_key_descriptor_by_utf8_name";
	int number_of_sub_key_descriptors                 = 0;
	int result                                        = 0;
	int sub_key_descriptor_index                      = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	*sub_key_descriptor = NULL;

/* TODO think of caching and/or optimization
 * e.g. mapping sub key descriptors to name search tree?
 */
	if( libcdata_array_get_number_of_entries(
	     key_item->sub_key_descriptors_array,
	     &number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from sub key descriptors array.",
		 function );

		goto on_error;
	}
	for( sub_key_descriptor_index = 0;
	     sub_key_descriptor_index < number_of_sub_key_descriptors;
	     sub_key_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     key_item->sub_key_descriptors_array,
		     sub_key_descriptor_index,
		     (intptr_t **) &safe_sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key descriptor: %d from array.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( safe_sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor: %d.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
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
		     safe_sub_key_descriptor->key_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sub key item at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 safe_sub_key_descriptor->key_offset,
			 safe_sub_key_descriptor->key_offset );

			goto on_error;
		}
/* TODO remove or pass name_hash */
		result = libcreg_key_name_entry_compare_name_with_utf8_string(
		          sub_key_item->key_name_entry,
		          0,
		          utf8_string,
		          utf8_string_length,
		          ascii_codepage,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare named key with UTF-8 string.",
			 function );

			goto on_error;
		}
		if( libcreg_key_item_free(
		     &sub_key_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key item.",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( sub_key_descriptor_index >= number_of_sub_key_descriptors )
	{
		return( 0 );
	}
	*sub_key_descriptor = safe_sub_key_descriptor;

	return( 1 );

on_error:
	if( sub_key_item != NULL )
	{
		libcreg_key_item_free(
		 &sub_key_item,
		 NULL );
	}
	return( -1 );
}

/* Retrieves the sub key descriptor for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcreg_key_item_get_sub_key_descriptor_by_utf16_name(
     libcreg_key_item_t *key_item,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libcreg_key_descriptor_t **sub_key_descriptor,
     libcerror_error_t **error )
{
	libcreg_key_descriptor_t *safe_sub_key_descriptor = NULL;
	libcreg_key_item_t *sub_key_item                  = NULL;
	static char *function                             = "libcreg_key_item_get_sub_key_descriptor_by_utf16_name";
	int number_of_sub_key_descriptors                 = 0;
	int result                                        = 0;
	int sub_key_descriptor_index                      = 0;

	if( key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key item.",
		 function );

		return( -1 );
	}
	*sub_key_descriptor = NULL;

/* TODO think of caching and/or optimization
 * e.g. mapping sub key descriptors to name search tree?
 */
	if( libcdata_array_get_number_of_entries(
	     key_item->sub_key_descriptors_array,
	     &number_of_sub_key_descriptors,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries from sub key descriptors array.",
		 function );

		goto on_error;
	}
	for( sub_key_descriptor_index = 0;
	     sub_key_descriptor_index < number_of_sub_key_descriptors;
	     sub_key_descriptor_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     key_item->sub_key_descriptors_array,
		     sub_key_descriptor_index,
		     (intptr_t **) &safe_sub_key_descriptor,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve sub key descriptor: %d from array.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
		if( safe_sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing sub key descriptor: %d.",
			 function,
			 sub_key_descriptor_index );

			goto on_error;
		}
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
		     safe_sub_key_descriptor->key_offset,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read sub key item at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 safe_sub_key_descriptor->key_offset,
			 safe_sub_key_descriptor->key_offset );

			goto on_error;
		}
/* TODO remove or pass name_hash */
		result = libcreg_key_name_entry_compare_name_with_utf16_string(
		          sub_key_item->key_name_entry,
		          0,
		          utf16_string,
		          utf16_string_length,
		          ascii_codepage,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GENERIC,
			 "%s: unable to compare named key with UTF-16 string.",
			 function );

			goto on_error;
		}
		if( libcreg_key_item_free(
		     &sub_key_item,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free sub key item.",
			 function );

			goto on_error;
		}
		if( result != 0 )
		{
			break;
		}
	}
	if( sub_key_descriptor_index >= number_of_sub_key_descriptors )
	{
		return( 0 );
	}
	*sub_key_descriptor = safe_sub_key_descriptor;

	return( 1 );

on_error:
	if( sub_key_item != NULL )
	{
		libcreg_key_item_free(
		 &sub_key_item,
		 NULL );
	}
	return( -1 );
}

