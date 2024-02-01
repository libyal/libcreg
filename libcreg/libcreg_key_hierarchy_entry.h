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

#if !defined( _LIBCREG_KEY_HIERARCHY_ENTRY_H )
#define _LIBCREG_KEY_HIERARCHY_ENTRY_H

#include <common.h>
#include <types.h>

#include "libcreg_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_key_hierarchy_entry libcreg_key_hierarchy_entry_t;

struct libcreg_key_hierarchy_entry
{
	/* The data offset
	 */
	size_t data_offset;

	/* The name hash
	 */
	uint32_t name_hash;

	/* The key name entry number
	 */
	uint16_t key_name_entry_number;

	/* The data block number
	 */
	uint16_t data_block_number;

	/* The parent key (hierarchy entry) offset
	 */
	uint32_t parent_key_offset;

	/* The sub key (hierarchy entry) offset
	 */
	uint32_t sub_key_offset;

	/* The next key (hierarchy entry) offset
	 */
	uint32_t next_key_offset;
};

int libcreg_key_hierarchy_entry_initialize(
     libcreg_key_hierarchy_entry_t **key_hierarchy_entry,
     libcerror_error_t **error );

int libcreg_key_hierarchy_entry_free(
     libcreg_key_hierarchy_entry_t **key_hierarchy_entry,
     libcerror_error_t **error );

int libcreg_key_hierarchy_entry_read_data(
     libcreg_key_hierarchy_entry_t *key_hierarchy_entry,
     const uint8_t *data,
     size_t data_size,
     size_t data_offset,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_KEY_HIERARCHY_ENTRY_H ) */

