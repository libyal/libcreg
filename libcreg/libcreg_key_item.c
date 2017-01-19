/*
 * Key item values functions
 *
 * Copyright (C) 2013-2017, Joachim Metz <joachim.metz@gmail.com>
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

#if defined( HAVE_WCTYPE_H )
#include <wctype.h>
#endif

#include "libcreg_data_block.h"
#include "libcreg_debug.h"
#include "libcreg_definitions.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_item.h"
#include "libcreg_key_hierarchy_entry.h"
#include "libcreg_key_name_entry.h"
#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_libfcache.h"
#include "libcreg_libfdata.h"
#include "libcreg_libuna.h"
#include "libcreg_unused.h"

/* Reads a key
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_read_node_data(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int node_data_file_index LIBCREG_ATTRIBUTE_UNUSED,
     off64_t node_data_offset,
     size64_t node_data_size LIBCREG_ATTRIBUTE_UNUSED,
     uint32_t node_data_flags LIBCREG_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBCREG_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libcreg_data_block_t *data_block                   = NULL;
	libcreg_key_hierarchy_entry_t *key_hierarchy_entry = NULL;
	libcreg_key_name_entry_t *key_name_entry           = NULL;
	static char *function                              = "libcreg_key_item_read_node_data";
	int32_t sub_key_offset                             = 0;
	int result                                         = 0;

	LIBCREG_UNREFERENCED_PARAMETER( node_data_file_index )
	LIBCREG_UNREFERENCED_PARAMETER( node_data_size )
	LIBCREG_UNREFERENCED_PARAMETER( node_data_flags )
	LIBCREG_UNREFERENCED_PARAMETER( read_flags )

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
	     node_data_offset,
	     &key_hierarchy_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key hierarchy entry for offset: %" PRIi64 ".",
		 function,
		 node_data_offset );

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
		 node_data_offset );

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
		if( libcreg_data_block_get_entry_by_index(
		     data_block,
		     (int) key_hierarchy_entry->key_name_entry_number,
		     &key_name_entry,
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
		if( key_name_entry == NULL )
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
	sub_key_offset = (int32_t) key_hierarchy_entry->sub_key_offset;

	if( sub_key_offset >= 0 )
	{
		result = libfdata_tree_node_sub_nodes_data_range_is_set(
		          node,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to determine if sub nodes data range is set.",
			 function );

			goto on_error;
		}
		else if( result == 0 )
		{
			if( libfdata_tree_node_set_sub_nodes_data_range(
			     node,
			     0,
			     (off64_t) sub_key_offset,
			     0,
			     0,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set first sub key as sub nodes range.",
				 function );

				goto on_error;
			}
		}
	}
	if( libfdata_tree_node_set_node_value(
	     node,
	     cache,
	     (intptr_t *) key_name_entry,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_key_name_entry_free,
	     LIBFDATA_TREE_NODE_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set key name entry as node value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 &key_name_entry,
		 NULL );
	}
	return( -1 );
}

/* Reads the sub keys
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_item_read_sub_nodes(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache LIBCREG_ATTRIBUTE_UNUSED,
     int sub_nodes_data_file_index LIBCREG_ATTRIBUTE_UNUSED,
     off64_t sub_nodes_data_offset,
     size64_t sub_nodes_data_size LIBCREG_ATTRIBUTE_UNUSED,
     uint32_t sub_nodes_data_flags LIBCREG_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBCREG_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libcreg_key_hierarchy_entry_t *key_hierarchy_entry = NULL;
	static char *function                              = "libcreg_key_item_read_sub_nodes";
	int32_t next_key_offset                            = 0;
	int sub_node_index                                 = 0;

	LIBCREG_UNREFERENCED_PARAMETER( cache )
	LIBCREG_UNREFERENCED_PARAMETER( sub_nodes_data_file_index )
	LIBCREG_UNREFERENCED_PARAMETER( sub_nodes_data_size )
	LIBCREG_UNREFERENCED_PARAMETER( sub_nodes_data_flags )
	LIBCREG_UNREFERENCED_PARAMETER( read_flags )

	if( sub_nodes_data_offset > (off64_t) INT32_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid sub nodes data offset value out of bounds.",
		 function );

		return( -1 );
	}
	next_key_offset = (int32_t) sub_nodes_data_offset;

	while( next_key_offset >= 0 )
	{
		if( libfdata_tree_node_append_sub_node(
		     node,
		     &sub_node_index,
		     0,
		     (off64_t) next_key_offset,
		     0,
		     0,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append sub node for offset: 0x%08" PRIx32 ".",
			 function,
			 next_key_offset );

			return( -1 );
		}
		if( libcreg_key_navigation_get_key_hierarchy_entry_at_offset(
		     key_navigation,
		     file_io_handle,
		     (off64_t) next_key_offset,
		     &key_hierarchy_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key hierarchy entry for offset: 0x%08" PRIx32 ".",
			 function,
			 next_key_offset );

			return( -1 );
		}
		if( key_hierarchy_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: missing key hierarchy entry for offset: 0x%08" PRIx32 ".",
			 function,
			 next_key_offset );

			return( -1 );
		}
		next_key_offset = (int32_t) key_hierarchy_entry->next_key_offset;
	}
	return( 1 );
}

