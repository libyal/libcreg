/*
 * Data block entry functions
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

#if !defined( _LIBCREG_DATA_BLOCK_ENTRY_H )
#define _LIBCREG_DATA_BLOCK_ENTRY_H

#include <common.h>
#include <types.h>

#include "libcreg_libcerror.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct libcreg_data_block_entry libcreg_data_block_entry_t;

struct libcreg_data_block_entry
{
	/* The data of the entry
	 */
	uint8_t *data;

	/* The offset of the entry
	 */
	size_t offset;

	/* The size of the entry
	 */
	size_t size;
};

int libcreg_data_block_entry_initialize(
     libcreg_data_block_entry_t **data_block_entry,
     libcerror_error_t **error );

int libcreg_data_block_entry_free(
     libcreg_data_block_entry_t **data_block_entry,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_DATA_BLOCK_ENTRY_H ) */

