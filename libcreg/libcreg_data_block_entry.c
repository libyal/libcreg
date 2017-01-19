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

#include <common.h>
#include <memory.h>
#include <types.h>

#include "libcreg_data_block_entry.h"
#include "libcreg_libcerror.h"

/* Creates a data block entry
 * Make sure the value data_block_entry is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_entry_initialize(
     libcreg_data_block_entry_t **data_block_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_data_block_entry_initialize";

	if( data_block_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block entry.",
		 function );

		return( -1 );
	}
	if( *data_block_entry != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block entry value already set.",
		 function );

		return( -1 );
	}
	*data_block_entry = memory_allocate_structure(
	                     libcreg_data_block_entry_t );

	if( *data_block_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data block entry.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_block_entry,
	     0,
	     sizeof( libcreg_data_block_entry_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data block entry.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_block_entry != NULL )
	{
		memory_free(
		 *data_block_entry );

		*data_block_entry = NULL;
	}
	return( -1 );
}

/* Frees a data block entry
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_entry_free(
     libcreg_data_block_entry_t **data_block_entry,
     libcerror_error_t **error )
{
	static char *function = "libcreg_data_block_entry_free";

	if( data_block_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block entry.",
		 function );

		return( -1 );
	}
	if( *data_block_entry != NULL )
	{
		memory_free(
		 *data_block_entry );

		*data_block_entry = NULL;
	}
	return( 1 );
}

