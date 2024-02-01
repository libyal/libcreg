/*
 * Key hierarchy entry functions
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

#include "libcreg_key_hierarchy_entry.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_unused.h"

#include "creg_key_navigation.h"

/* Creates a key hierarchy entry
 * Make sure the value key_hierarchy_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_hierarchy_entry_initialize(
     libcreg_key_hierarchy_entry_t **key_hierarchy_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_hierarchy_entry_initialize";

	if( key_hierarchy_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key hierarchy entry.",
		 function );

		return( -1 );
	}
	if( *key_hierarchy_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key hierarchy entry value already set.",
		 function );

		return( -1 );
	}
	*key_hierarchy_entry = memory_allocate_structure(
	                        libcreg_key_hierarchy_entry_t );

	if( *key_hierarchy_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key hierarchy entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_hierarchy_entry,
	     0,
	     sizeof( libcreg_key_hierarchy_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key hierarchy entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_hierarchy_entry != NULL )
	{
		memory_free(
		 *key_hierarchy_entry );

		*key_hierarchy_entry = NULL;
	}
	return( -1 );
}

/* Frees a key hierarchy entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_hierarchy_entry_free(
     libcreg_key_hierarchy_entry_t **key_hierarchy_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_hierarchy_entry_free";

	if( key_hierarchy_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key hierarchy entry.",
		 function );

		return( -1 );
	}
	if( *key_hierarchy_entry != NULL )
	{
		memory_free(
		 *key_hierarchy_entry );

		*key_hierarchy_entry = NULL;
	}
	return( 1 );
}

/* Reads key hierarchy entry data
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_hierarchy_entry_read_data(
     libcreg_key_hierarchy_entry_t *key_hierarchy_entry,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_hierarchy_entry_read_data";

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit  = 0;
#else
	LIBCREG_UNREFERENCED_PARAMETER( data_offset );
#endif

	if( key_hierarchy_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key hierarchy entry.",
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
	if( ( data_size < sizeof( creg_key_hierarchy_entry_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key hierarchy entry:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( creg_key_hierarchy_entry_t ),
		 0 );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_key_hierarchy_entry_t *) data )->name_hash,
	 key_hierarchy_entry->name_hash );

	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_key_hierarchy_entry_t *) data )->parent_key_offset,
	 key_hierarchy_entry->parent_key_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_key_hierarchy_entry_t *) data )->sub_key_offset,
	 key_hierarchy_entry->sub_key_offset );

	byte_stream_copy_to_uint32_little_endian(
	 ( (creg_key_hierarchy_entry_t *) data )->next_key_offset,
	 key_hierarchy_entry->next_key_offset );

	byte_stream_copy_to_uint16_little_endian(
	 ( (creg_key_hierarchy_entry_t *) data )->key_name_entry_number,
	 key_hierarchy_entry->key_name_entry_number );

	byte_stream_copy_to_uint16_little_endian(
	 ( (creg_key_hierarchy_entry_t *) data )->data_block_number,
	 key_hierarchy_entry->data_block_number );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: offset\t\t\t\t: 0x%08" PRIzx "\n",
		 function,
		 data_offset );

		byte_stream_copy_to_uint32_little_endian(
		 ( (creg_key_hierarchy_entry_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: name hash\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 key_hierarchy_entry->name_hash );

		byte_stream_copy_to_uint32_little_endian(
		 ( (creg_key_hierarchy_entry_t *) data )->unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: parent key offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 key_hierarchy_entry->parent_key_offset );

		libcnotify_printf(
		 "%s: sub key offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 key_hierarchy_entry->sub_key_offset );

		libcnotify_printf(
		 "%s: next key offset\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 key_hierarchy_entry->next_key_offset );

		libcnotify_printf(
		 "%s: key name entry number\t\t: %" PRIi16 "\n",
		 function,
		 (int16_t) key_hierarchy_entry->key_name_entry_number );

		libcnotify_printf(
		 "%s: data block number\t\t: %" PRIi16 "\n",
		 function,
		 (int16_t) key_hierarchy_entry->data_block_number );

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	key_hierarchy_entry->data_offset = data_offset;

	return( 1 );
}

