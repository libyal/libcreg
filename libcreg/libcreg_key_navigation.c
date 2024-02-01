/*
 * Key navigation functions
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

#include "libcreg_data_block.h"
#include "libcreg_definitions.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_name_entry.h"
#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_libfcache.h"
#include "libcreg_libfdata.h"
#include "libcreg_unused.h"

#include "creg_file_header.h"
#include "creg_key_navigation.h"

const char *creg_key_navigation_signature = "RGKN";

/* Creates a key navigation
 * Make sure the value key_navigation is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_navigation_initialize(
     libcreg_key_navigation_t **key_navigation,
     libcreg_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_navigation_initialize";

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
	if( *key_navigation != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key navigation value already set.",
		 function );

		return( -1 );
	}
	*key_navigation = memory_allocate_structure(
	                   libcreg_key_navigation_t );

	if( *key_navigation == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create key navigation.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *key_navigation,
	     0,
	     sizeof( libcreg_key_navigation_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear key navigation.",
		 function );

		goto on_error;
	}
	( *key_navigation )->io_handle = io_handle;

	return( 1 );

on_error:
	if( *key_navigation != NULL )
	{
		memory_free(
		 *key_navigation );

		*key_navigation = NULL;
	}
	return( -1 );
}

/* Frees a key navigation
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_navigation_free(
     libcreg_key_navigation_t **key_navigation,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_navigation_free";
	int result            = 1;

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
	if( *key_navigation != NULL )
	{
		/* The io_handle reference is freed elsewhere
		 */
		if( ( *key_navigation )->key_hierarchy_area != NULL )
		{
			if( libfdata_area_free(
			     &( ( *key_navigation )->key_hierarchy_area ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key hierarchy area.",
				 function );

				result = -1;
			}
		}
		if( ( *key_navigation )->key_hierarchy_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *key_navigation )->key_hierarchy_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key hierarchy cache.",
				 function );

				result = -1;
			}
		}
		if( ( *key_navigation )->data_blocks_list != NULL )
		{
			if( libfdata_list_free(
			     &( ( *key_navigation )->data_blocks_list ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data blocks list.",
				 function );

				result = -1;
			}
		}
		if( ( *key_navigation )->data_blocks_cache != NULL )
		{
			if( libfcache_cache_free(
			     &( ( *key_navigation )->data_blocks_cache ),
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free data blocks cache.",
				 function );

				result = -1;
			}
		}
		memory_free(
		 *key_navigation );

		*key_navigation = NULL;
	}
	return( result );
}

/* Reads a key navigation record
 * Returns 1 if successful, 0 if no key navigation signature was found or -1 on error
 */
int libcreg_key_navigation_read_file_io_handle(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     libcerror_error_t **error )
{
	creg_key_navigation_header_t key_navigation_header;

	static char *function                      = "libcreg_key_navigation_read_file_io_handle";
	ssize_t read_count                         = 0;
	uint32_t data_size                         = 0;
	uint32_t key_hierarchy_entries_data_offset = 0;
	int entry_index                            = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit                       = 0;
#endif

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
	if( key_navigation->key_hierarchy_area != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key navigation - key hierarchy area already set.",
		 function );

		return( -1 );
	}
	if( key_navigation->key_hierarchy_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key navigation - key hierarchy cache already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading key navigation at offset: %" PRIzd " (0x%08" PRIzx ")\n",
		 function,
		 sizeof( creg_file_header_t ),
		 sizeof( creg_file_header_t ) );
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &key_navigation_header,
	              sizeof( creg_key_navigation_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( creg_key_navigation_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key navigation header data.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: key navigation header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &key_navigation_header,
		 sizeof( creg_key_navigation_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     key_navigation_header.signature,
	     creg_key_navigation_signature,
	     4 ) != 0 )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 key_navigation_header.size,
	 data_size );

	byte_stream_copy_to_uint32_little_endian(
	 key_navigation_header.key_hierarchy_entries_data_offset,
	 key_hierarchy_entries_data_offset );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 key_navigation_header.signature[ 0 ],
		 key_navigation_header.signature[ 1 ],
		 key_navigation_header.signature[ 2 ],
		 key_navigation_header.signature[ 3 ] );

		libcnotify_printf(
		 "%s: size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_size );

		libcnotify_printf(
		 "%s: key hierarchy entries data offset\t: 0x%08" PRIx32 "\n",
		 function,
		 key_hierarchy_entries_data_offset );

		byte_stream_copy_to_uint32_little_endian(
		 key_navigation_header.unknown2,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 key_navigation_header.unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 key_navigation_header.unknown4,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown4\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown5:\n",
		 function );
		libcnotify_print_data(
		 key_navigation_header.unknown5,
		 8,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	if( data_size < sizeof( creg_key_navigation_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		goto on_error;
	}
#if SIZEOF_SIZE_T <= 4
	if( data_size > (size_t) SSIZE_MAX )
#else
	if( data_size > (uint32_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data size value exceeds maximum.",
		 function );

		goto on_error;
	}
	data_size -= sizeof( creg_key_navigation_header_t );

/* TODO clone function ? */
	if( libfdata_area_initialize(
	     &( key_navigation->key_hierarchy_area ),
	     (size64_t) sizeof( creg_key_hierarchy_entry_t ),
	     NULL,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_area_t *, libfdata_cache_t *, off64_t, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libcreg_io_handle_read_key_hierarchy_entry,
	     NULL,
	     LIBFDATA_DATA_HANDLE_FLAG_NON_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key hierarchy area.",
		 function );

		goto on_error;
	}
	if( libfdata_area_append_segment(
	     key_navigation->key_hierarchy_area,
	     &entry_index,
	     0,
	     (off64_t) key_hierarchy_entries_data_offset,
	     (size64_t) data_size,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
		 "%s: unable to append segment to key hierarchy area.",
		 function );

		goto on_error;
	}
	if( libfcache_cache_initialize(
	     &( key_navigation->key_hierarchy_cache ),
	     LIBCREG_MAXIMUM_CACHE_ENTRIES_KEYS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key hierarchy cache.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( key_navigation->key_hierarchy_cache != NULL )
	{
		libfcache_cache_free(
		 &( key_navigation->key_hierarchy_cache ),
		 NULL );
	}
	if( key_navigation->key_hierarchy_area != NULL )
	{
		libfdata_area_free(
		 &( key_navigation->key_hierarchy_area ),
		 NULL );
	}
	return( -1 );
}

/* Reads the data block
 * Returns 1 if successful, 0 if no data block signature was found or -1 on error
 */
int libcreg_key_navigation_read_data_blocks(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     size64_t file_size,
     libcerror_error_t **error )
{
	libcreg_data_block_t *data_block = NULL;
	static char *function            = "libcreg_key_navigation_read_data_blocks";
	int data_block_index             = 0;
	int result                       = 0;

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
	if( key_navigation->data_blocks_list != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key navigation - data blocks list already set.",
		 function );

		return( -1 );
	}
	if( key_navigation->data_blocks_cache != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key navigation - data blocks list already set.",
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
	if( file_offset < 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_LESS_THAN_ZERO,
		 "%s: invalid file offset value less than zero.",
		 function );

		return( -1 );
	}
	if( libfdata_list_initialize(
	     &( key_navigation->data_blocks_list ),
	     (intptr_t *) key_navigation->io_handle,
	     NULL,
	     NULL,
	     (int (*)(intptr_t *, intptr_t *, libfdata_list_element_t *, libfdata_cache_t *, int, off64_t, size64_t, uint32_t, uint8_t, libcerror_error_t **)) &libcreg_key_navigation_read_data_block_element_data,
	     NULL,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data blocks list.",
		 function );

		goto on_error;
	}
	while( (size64_t) file_offset < file_size )
	{
		if( libcreg_data_block_initialize(
		     &data_block,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data block.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading data block: %d at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
			 function,
			 data_block_index,
			 file_offset,
			 file_offset );
		}
#endif
		result = libcreg_data_block_read_header(
		          data_block,
		          file_io_handle,
		          file_offset,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read data block header.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
/* TODO mark file as corrupted */
			if( data_block_index != 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: missing data block at offset: %" PRIi64 ".",
				 function,
				 file_offset );

				goto on_error;
			}
		}
		else
		{
			if( data_block->size == 0 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: invalid data block: %d size.",
				 function,
				 data_block_index );

				goto on_error;
			}
			if( libfdata_list_append_element(
			     key_navigation->data_blocks_list,
			     &data_block_index,
			     0,
			     (off64_t) data_block->offset,
			     (size64_t) data_block->size,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append data block: %d to data list.",
				 function,
				 data_block_index );

				goto on_error;
			}
			file_offset += data_block->size;
		}
		if( libcreg_data_block_free(
		     &data_block,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free data block.",
			 function );

			goto on_error;
		}
		if( result == 0 )
		{
			break;
		}
		data_block_index++;
	}
	if( libfcache_cache_initialize(
	     &( key_navigation->data_blocks_cache ),
	     LIBCREG_MAXIMUM_CACHE_ENTRIES_DATA_BLOCKS,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data blocks cache.",
		 function );

		goto on_error;
	}
	if( ( data_block_index == 0 )
	 && ( result == 0 ) )
	{
		return( 0 );
	}
	return( 1 );

on_error:
	if( data_block != NULL )
	{
		libcreg_data_block_free(
		 &data_block,
		 NULL );
	}
	if( key_navigation->data_blocks_list != NULL )
	{
		libfdata_list_free(
		 &( key_navigation->data_blocks_list ),
		 NULL );
	}
	return( -1 );
}

/* Retrieves the key hierarchy entry at a specific offset
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_navigation_get_key_hierarchy_entry_at_offset(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     off64_t key_hierarchy_entry_offset,
     libcreg_key_hierarchy_entry_t **key_hierarchy_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_navigation_get_key_hierarchy_entry_at_offset";

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
	if( libfdata_area_get_element_value_at_offset(
	     key_navigation->key_hierarchy_area,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) key_navigation->key_hierarchy_cache,
	     key_hierarchy_entry_offset,
	     (intptr_t **) key_hierarchy_entry,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key hierarchy entry at offset: 0x%08" PRIx64 ".",
		 function,
		 key_hierarchy_entry_offset );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves the number of data blocks
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_navigation_get_number_of_data_blocks(
     libcreg_key_navigation_t *key_navigation,
     int *number_of_data_blocks,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_navigation_get_number_of_data_blocks";

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
	if( libfdata_list_get_number_of_elements(
	     key_navigation->data_blocks_list,
	     number_of_data_blocks,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of data blocks list elements.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific data block
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_navigation_get_data_block_at_index(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     int data_block_index,
     libcreg_data_block_t **data_block,
     libcerror_error_t **error )
{
	static char *function = "libcreg_key_navigation_get_data_block_at_index";

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
	if( libfdata_list_get_element_value_by_index(
	     key_navigation->data_blocks_list,
	     (intptr_t *) file_io_handle,
	     (libfdata_cache_t *) key_navigation->data_blocks_cache,
	     data_block_index,
	     (intptr_t **) data_block,
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data block: %d.",
		 function,
		 data_block_index );

		return( -1 );
	}
	return( 1 );
}

/* Reads a data block
 * Callback function for the data blocks list
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_navigation_read_data_block_element_data(
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libfdata_list_element_t *list_element,
     libfdata_cache_t *cache,
     int data_range_file_index LIBCREG_ATTRIBUTE_UNUSED,
     off64_t data_range_offset,
     size64_t data_range_size,
     uint32_t data_range_flags LIBCREG_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBCREG_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libcreg_data_block_t *data_block = NULL;
	static char *function            = "libcreg_key_navigation_read_data_block_element_data";

	LIBCREG_UNREFERENCED_PARAMETER( data_range_file_index )
	LIBCREG_UNREFERENCED_PARAMETER( data_range_flags )
	LIBCREG_UNREFERENCED_PARAMETER( read_flags )

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( libcreg_data_block_initialize(
	     &data_block,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading data block at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 data_range_offset,
		 data_range_offset );
	}
#endif
	if( libcreg_data_block_read_header(
	     data_block,
	     file_io_handle,
	     data_range_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block header.",
		 function );

		goto on_error;
	}
	if( (size64_t) data_block->size != data_range_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: mismatch in data block size (stored: %" PRIu32 " != calculated: %" PRIu64 ").",
		 function,
		 data_block->size,
		 data_range_size );

		goto on_error;
	}
	if( libcreg_data_block_read_entries(
	     data_block,
	     file_io_handle,
	     io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key name entries.",
		 function );

		goto on_error;
	}
	if( libfdata_list_element_set_element_value(
	     list_element,
	     (intptr_t *) file_io_handle,
	     cache,
	     (intptr_t *) data_block,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_data_block_free,
	     LIBFDATA_LIST_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set data block as list element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( data_block != NULL )
	{
		libcreg_data_block_free(
		 &data_block,
		 NULL );
	}
	return( -1 );
}


