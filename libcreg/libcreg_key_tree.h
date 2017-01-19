/*
 * Key tree functions
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

#if !defined( _LIBCREG_KEY_TREE_H )
#define _LIBCREG_KEY_TREE_H

#include <common.h>
#include <types.h>

#include "libcreg_key_name_entry.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libfcache.h"
#include "libcreg_libfdata.h"

#if defined( __cplusplus )
extern "C" {
#endif

int libcreg_key_tree_get_sub_key_values_by_utf8_name(
     libfdata_tree_node_t *key_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *key_cache,
     uint32_t name_hash,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     int ascii_codepage,
     libfdata_tree_node_t **key_tree_sub_node,
     libcreg_key_name_entry_t **sub_key_name_entry,
     libcerror_error_t **error );

int libcreg_key_tree_get_sub_key_values_by_utf16_name(
     libfdata_tree_node_t *key_tree_node,
     libbfio_handle_t *file_io_handle,
     libfcache_cache_t *key_cache,
     uint32_t name_hash,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     int ascii_codepage,
     libfdata_tree_node_t **key_tree_sub_node,
     libcreg_key_name_entry_t **sub_key_name_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_KEY_TREE_H ) */

