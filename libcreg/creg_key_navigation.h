/*
 * The key navigation definition of a Windows 9x/Me Registry File (CREG)
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

#if !defined( _CREG_KEY_NAVIGATION_H )
#define _CREG_KEY_NAVIGATION_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct creg_key_navigation_header creg_key_navigation_header_t;

struct creg_key_navigation_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains: "RGKN"
	 */
	uint8_t signature[ 4 ];

	/* The size of the key navigation
	 * Consists of 4 bytes
	 * Contains the number of bytes
	 */
	uint8_t size[ 4 ];

	/* The key hierarchy entries data offset
	 * Consists of 4 bytes
	 */
	uint8_t key_hierarchy_entries_data_offset[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown4[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown5[ 8 ];
};

typedef struct creg_key_hierarchy_entry creg_key_hierarchy_entry_t;

struct creg_key_hierarchy_entry
{
	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The hash of the name
	 * Consists of 4 bytes
	 */
	uint8_t name_hash[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];

	/* The parent key offset
	 * Consists of 4 bytes
	 */
	uint8_t parent_key_offset[ 4 ];

	/* The first sub key offset
	 * Consists of 4 bytes
	 */
	uint8_t sub_key_offset[ 4 ];

	/* The next key offset
	 * Consists of 4 bytes
	 */
	uint8_t next_key_offset[ 4 ];

	/* The key name entry number
	 * Consists of 2 bytes
	 */
	uint8_t key_name_entry_number[ 2 ];

	/* The data block number
	 * Consists of 2 bytes
	 */
	uint8_t data_block_number[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _CREG_KEY_NAVIGATION_H ) */

