/*
 * Key item functions
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

#if !defined( _LIBCREG_KEY_ITEM_H )
#define _LIBCREG_KEY_ITEM_H

#include <common.h>
#include <types.h>

#include "libcreg_key_navigation.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libfcache.h"
#include "libcreg_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libcreg_key_item_read_node_data(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int node_data_file_index,
     off64_t node_data_offset,
     size64_t node_data_size,
     uint32_t node_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

int libcreg_key_item_read_sub_nodes(
     libcreg_key_navigation_t *key_navigation,
     libbfio_handle_t *file_io_handle,
     libfdata_tree_node_t *node,
     libfcache_cache_t *cache,
     int sub_nodes_data_file_index,
     off64_t sub_nodes_data_offset,
     size64_t sub_nodes_data_size,
     uint32_t sub_nodes_data_flags,
     uint8_t read_flags,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_KEY_ITEM_H ) */

