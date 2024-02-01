/*
 * The file header definition of a Windows 9x/Me Registry File (CREG)
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

#if !defined( _CREG_FILE_HEADER_H )
#define _CREG_FILE_HEADER_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct creg_file_header creg_file_header_t;

struct creg_file_header
{
	/* The signature
	 * Consists of 4 bytes
	 * Contains: "CREG"
	 */
	uint8_t signature[ 4 ];

	/* The minor version number
	 * Consists of 2 bytes
	 */
	uint8_t minor_version[ 2 ];

	/* The major version number
	 * Consists of 2 bytes
	 */
	uint8_t major_version[ 2 ];

	/* The data blocks list offset
	 * Consists of 4 bytes
	 */
	uint8_t data_blocks_list_offset[ 4 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown1[ 4 ];

	/* The number of data blocks
	 * Consists of 2 bytes
	 */
	uint8_t number_of_data_blocks[ 2 ];

	/* Unknown
	 * Consists of 2 bytes
	 */
	uint8_t unknown2[ 2 ];

	/* Unknown
	 * Consists of 4 bytes
	 */
	uint8_t unknown3[ 4 ];

	/* Unknown
	 * Consists of 8 bytes
	 */
	uint8_t unknown4[ 8 ];
};

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _CREG_FILE_HEADER_H ) */

