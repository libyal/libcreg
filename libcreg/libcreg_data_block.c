/*
 * Data block functions
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
#include "libcreg_key_name_entry.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcdata.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_unused.h"

#include "creg_data_block.h"

const char *creg_data_block_signature = "RGDB";

/* Creates a data block
 * Make sure the value data_block is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_initialize(
     libcreg_data_block_t **data_block,
     libcerror_error_t **error )
{
	static char *function = "libcreg_data_block_initialize";

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( *data_block != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block value already set.",
		 function );

		return( -1 );
	}
	*data_block = memory_allocate_structure(
	               libcreg_data_block_t );

	if( *data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data block.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *data_block,
	     0,
	     sizeof( libcreg_data_block_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear data block.",
		 function );

		memory_free(
		 *data_block );

		*data_block = NULL;

		return( -1 );
	}
	if( libcdata_array_initialize(
	     &( ( *data_block )->entries_array ),
	     0,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create entries array.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *data_block != NULL )
	{
		memory_free(
		 *data_block );

		*data_block = NULL;
	}
	return( -1 );
}

/* Frees a data block
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_free(
     libcreg_data_block_t **data_block,
     libcerror_error_t **error )
{
	static char *function = "libcreg_data_block_free";
	int result            = 1;

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( *data_block != NULL )
	{
		if( libcdata_array_free(
		     &( ( *data_block )->entries_array ),
		     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_key_name_entry_free,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free the entries array.",
			 function );

			result = -1;
		}
		if( ( *data_block )->data != NULL )
		{
			memory_free(
			 ( *data_block )->data );
		}
		memory_free(
		 *data_block );

		*data_block = NULL;
	}
	return( result );
}

/* Reads a data block header
 * Returns 1 if successful, 0 if no data block signature was found or -1 on error
 */
int libcreg_data_block_read_header(
     libcreg_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	creg_data_block_header_t data_block_header;

	static char *function = "libcreg_data_block_read_header";
	ssize_t read_count    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint16_t value_16bit  = 0;
#endif

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	data_block->offset = file_offset;

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading data block header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &data_block_header,
	              sizeof( creg_data_block_header_t ),
	              file_offset,
	              error );

	if( read_count != (ssize_t) sizeof( creg_data_block_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block header data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: data block header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &data_block_header,
		 sizeof( creg_data_block_header_t ),
		 0 );
	}
#endif
	if( memory_compare(
	     data_block_header.signature,
	     creg_data_block_signature,
	     4 ) != 0 )
	{
		return( 0 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 data_block_header.size,
	 data_block->size );

	byte_stream_copy_to_uint32_little_endian(
	 data_block_header.unused_size,
	 data_block->unused_size );

	byte_stream_copy_to_uint32_little_endian(
	 data_block_header.used_size,
	 data_block->used_size );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 data_block_header.signature[ 0 ],
		 data_block_header.signature[ 1 ],
		 data_block_header.signature[ 2 ],
		 data_block_header.signature[ 3 ] );

		libcnotify_printf(
		 "%s: size\t\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_block->size );

		libcnotify_printf(
		 "%s: unused size\t\t\t\t: %" PRIu32 "\n",
		 function,
		 data_block->unused_size );

		byte_stream_copy_to_uint16_little_endian(
		 data_block_header.unknown1,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 data_block_header.index,
		 value_16bit );
		libcnotify_printf(
		 "%s: index\t\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: used size\t\t\t\t: %" PRIi32 "\n",
		 function,
		 (int32_t) data_block->used_size );

		byte_stream_copy_to_uint16_little_endian(
		 data_block_header.unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint16_little_endian(
		 data_block_header.unknown3,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: %" PRIu16 "\n",
		 function,
		 value_16bit );

		libcnotify_printf(
		 "%s: unknown3:\n",
		 function );
		libcnotify_print_data(
		 data_block_header.unknown4,
		 8,
		 0 );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

/* TODO check if unused_size + used_size == size */
	return( 1 );
}

/* Reads a data block and determines its entries
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_read_entries(
     libcreg_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libcreg_key_name_entry_t *key_name_entry = NULL;
	static char *function                    = "libcreg_data_block_read_entries";
	size_t data_offset                       = 0;
	size_t key_name_entry_size               = 0;
	ssize_t read_count                       = 0;
	int entry_index                          = 0;
	int result                               = 0;

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data_block->data != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid data block - data already set.",
		 function );

		return( -1 );
	}
	if( ( data_block->size == 0 )
	 || ( data_block->size > (uint32_t) MEMORY_MAXIMUM_ALLOCATION_SIZE ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data block - size value out of bounds.",
		 function );

		return( -1 );
	}
	data_block->data_size = (size_t) data_block->size - sizeof( creg_data_block_header_t );

	data_block->data = (uint8_t *) memory_allocate(
	                                sizeof( uint8_t ) * data_block->data_size );
	
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create data block data.",
		 function );

		goto on_error;
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              data_block->data,
	              data_block->data_size,
	              error );

	if( read_count != (ssize_t) data_block->data_size )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block data.",
		 function );

		goto on_error;
	}
	while( data_offset < data_block->data_size )
	{
		if( libcreg_key_name_entry_initialize(
		     &key_name_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create key name entry.",
			 function );

			goto on_error;
		}
		key_name_entry->offset = data_block->offset + sizeof( creg_data_block_header_t ) + data_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: reading key name entry at offset: %" PRIu32 " (0x%08" PRIx32 ")\n",
			 function,
			 key_name_entry->offset,
			 key_name_entry->offset );
		}
#endif
		result = libcreg_key_name_entry_read_data(
		          key_name_entry,
		          &( ( data_block->data )[ data_offset ] ),
		          data_block->data_size - data_offset,
		          ascii_codepage,
		          error );

		if( result == -1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_IO,
			 LIBCERROR_IO_ERROR_READ_FAILED,
			 "%s: unable to read key name entry at offset: %" PRIu32 " (0x%08" PRIx32 ").",
			 function,
			 key_name_entry->offset,
			 key_name_entry->offset );

			goto on_error;
		}
		key_name_entry_size = key_name_entry->size;

		if( result == 0 )
		{
			if( key_name_entry_size <= ( data_block->data_size - data_offset ) )
			{
				result = 1;
			}
			if( libcreg_key_name_entry_free(
			     &key_name_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key name entry.",
				 function );

				goto on_error;
			}
		}
		if( result == 0 )
		{
			break;
		}
		data_offset += key_name_entry_size;

		if( key_name_entry != NULL )
		{
			if( libcdata_array_append_entry(
			     data_block->entries_array,
			     &entry_index,
			     (intptr_t *) key_name_entry,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
				 "%s: unable to append key name entry: %d.",
				 function,
				 key_name_entry );

				goto on_error;
			}
			key_name_entry = NULL;
		}
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		if( data_offset < data_block->data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( ( data_block->data )[ data_offset ] ),
			 data_block->data_size - data_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );

on_error:
	if( key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 &key_name_entry,
		 NULL );
	}
	if( data_block->entries_array != NULL )
	{
		libcdata_array_clear(
		 data_block->entries_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_key_name_entry_free,
		 NULL );
	}
	if( data_block->data != NULL )
	{
		memory_free(
		 data_block->data );

		data_block->data = NULL;
	}
	data_block->data_size = 0;

	return( -1 );
}

/* Retrieves the number of key name entries
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_get_number_of_entries(
     libcreg_data_block_t *data_block,
     int *number_of_entries,
     libcerror_error_t **error )
{
	static char *function = "libcreg_data_block_get_number_of_entries";

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     data_block->entries_array,
	     number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Retrieves a specific key name entry
 * Returns 1 if successful, 0 if no such entry or -1 on error
 */
int libcreg_data_block_get_entry_by_identifier(
     libcreg_data_block_t *data_block,
     uint16_t identifier,
     libcreg_key_name_entry_t **key_name_entry,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libcreg_key_name_entry_t *safe_key_name_entry = NULL;
	static char *function                         = "libcreg_data_block_get_entry_by_identifier";
	int entry_index                               = 0;
	int number_of_entries                         = 0;

	if( data_block == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid data block.",
		 function );

		return( -1 );
	}
	if( data_block->data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing data.",
		 function );

		return( -1 );
	}
	if( key_name_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key name entry.",
		 function );

		return( -1 );
	}
	if( libcdata_array_get_number_of_entries(
	     data_block->entries_array,
	     &number_of_entries,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of entries.",
		 function );

		return( -1 );
	}
	for( entry_index = 0;
	     entry_index < number_of_entries;
	     entry_index++ )
	{
		if( libcdata_array_get_entry_by_index(
		     data_block->entries_array,
		     entry_index,
		     (intptr_t **) &safe_key_name_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key name entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( safe_key_name_entry == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid data block - missing key name entry: %d.",
			 function,
			 entry_index );

			return( -1 );
		}
		if( safe_key_name_entry->index == identifier )
		{
			*key_name_entry = safe_key_name_entry;

			return( 1 );
		}
	}
	return( 0 );
}

