/*
 * The data block definition of a Windows 9x/Me Registry File (CREG)
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

#if !defined( _CREG_DATA_BLOCK_H )
#define _CREG_DATA_BLOCK_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct creg_data_block_header creg_data_block_header_t;

struct creg_data_block_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains: "RGDB"
	 */
	uint8_t signature[ 4 ];

	/* The size of the data block
	 * Consists of 4 bytes
	 * Contains number of bytes
	 */
	uint8_t size[ 4 ];

	/* The unused size
	 * Consists of 4 bytes
	 */
	uint8_t unused_size[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The index
	 * Consists of 2 bytes
	 */
	uint8_t index[ 2 ];

	/* The used size
	 * Consists of 4 bytes
	 */
	uint8_t used_size[ 4 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown3[ 2 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];
};

typedef struct creg_key_name_entry creg_key_name_entry_t;

struct creg_key_name_entry
{
	/* The size
	 * Consists of 4 bytes
	 * Contains number of bytes
	 */
	uint8_t size[ 4 ];

	/* The index
	 * Consists of 2 bytes
	 */
	uint8_t index[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown1[ 2 ];

	/* The used size
	 * Consists of 4 bytes
	 */
	uint8_t used_size[ 4 ];

	/* The name size
	 * Consists of 2 bytes
	 * Contains number of bytes
	 */
	uint8_t name_size[ 2 ];

	/* The number of values
	 * Consists of 2 bytes
	 */
	uint8_t number_of_values[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown2[ 4 ];
};

typedef struct creg_value_entry creg_value_entry_t;

struct creg_value_entry
{
	/* The data type
	 * Consists of 4 bytes
	 */
	uint8_t data_type[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The name size
	 * Consists of 2 bytes
	 * Contains number of bytes
	 */
	uint8_t name_size[ 2 ];

	/* The data size
	 * Consists of 2 bytes
	 * Contains number of bytes
	 */
	uint8_t data_size[ 2 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _CREG_DATA_BLOCK_H ) */

