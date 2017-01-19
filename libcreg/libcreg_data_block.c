/*
 * Data block functions
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
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libcreg_data_block.h"
#include "libcreg_data_block_entry.h"
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
		     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_data_block_entry_free,
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
	if( libbfio_handle_get_offset(
	     file_io_handle,
	     &( data_block->offset ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve file offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading data block header at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 data_block->offset,
		 data_block->offset );
	}
#endif
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &data_block_header,
	              sizeof( creg_data_block_header_t ),
	              error );

	if( read_count != (ssize_t) sizeof( creg_data_block_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read data block header data.",
		 function );

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

		libcnotify_printf(
		 "\n" );
	}
#endif
/* TODO check if unused_size + used_size == size */
	return( 1 );
}

/* Reads a data block and determines its entries
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_read_entries(
     libcreg_data_block_t *data_block,
     libbfio_handle_t *file_io_handle,
     int (*read_entry_size_function)(
            const uint8_t *data,
            size_t data_size,
            size_t *entry_size,
            libcerror_error_t **error ),
     int ascii_codepage,
     uint8_t have_debug_output LIBCREG_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	libcreg_data_block_entry_t *data_block_entry = NULL;
	uint8_t *data_block_entry_data               = NULL;
	static char *function                        = "libcreg_data_block_read_entries";
	size_t data_block_data_size                  = 0;
	size_t data_block_entry_offset               = 0;
	ssize_t read_count                           = 0;
	int entry_index                              = 0;
	int data_block_entry_index                   = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	libcreg_key_name_entry_t *key_name_entry     = NULL;
#else
	LIBCREG_UNREFERENCED_PARAMETER( have_debug_output );
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
#if SIZEOF_SIZE_T <= 4
	if( data_block->size > (size_t) SSIZE_MAX )
#else
	if( data_block->size > (uint32_t) SSIZE_MAX )
#endif
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid data block - size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( read_entry_size_function == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid read entry size function.",
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
	data_block_entry_data = data_block->data;

	if( (int32_t) data_block->used_size < 0 )
	{
		data_block_data_size = data_block->data_size;
	}
	else
	{
		data_block_data_size = (size_t) data_block->used_size - sizeof( creg_data_block_header_t );
	}
	while( data_block_entry_offset < data_block_data_size )
	{
		if( libcreg_data_block_entry_initialize(
		     &data_block_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to create data block entry.",
			 function );

			goto on_error;
		}
		if( read_entry_size_function(
		     &( data_block_entry_data[ data_block_entry_offset ] ),
		     data_block->data_size - data_block_entry_offset,
		     &( data_block_entry->size ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve data block entry size.",
			 function );

			goto on_error;
		}
#if defined( HAVE_DEBUG_OUTPUT )
		if( libcnotify_verbose != 0 )
		{
			libcnotify_printf(
			 "%s: data block entry: %03d offset\t\t: %" PRIzd "\n",
			 function,
			 data_block_entry_index,
			 data_block_entry_offset );

			libcnotify_printf(
			 "%s: data block entry: %03d size\t\t: %" PRIzd "\n",
			 function,
			 data_block_entry_index,
			 data_block_entry->size );
		}
#endif
		data_block_entry->offset = data_block_entry_offset;

#if defined( HAVE_DEBUG_OUTPUT )
		if( ( libcnotify_verbose != 0 )
		 && ( have_debug_output != 0 ) )
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
			if( libcreg_key_name_entry_read(
			     key_name_entry,
			     &( ( data_block->data )[ data_block_entry->offset ] ),
			     data_block_entry->size,
			     ascii_codepage,
			     error ) != 1 )
			{
				libcerror_error_set(
				 error,
				 LIBCERROR_ERROR_DOMAIN_IO,
				 LIBCERROR_IO_ERROR_READ_FAILED,
				 "%s: unable to read key name entry: %d.",
				 function,
				 data_block_entry_index );

				goto on_error;
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
#endif
		data_block_entry_offset += (size_t) data_block_entry->size;

		if( libcdata_array_append_entry(
		     data_block->entries_array,
		     &entry_index,
		     (intptr_t *) data_block_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_APPEND_FAILED,
			 "%s: unable to append data block entry: %d.",
			 function,
			 data_block_entry_index );

			goto on_error;
		}
		data_block_entry = NULL;

		data_block_entry_index++;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( ( libcnotify_verbose != 0 )
	 && ( have_debug_output != 0 ) )
	{
		if( data_block_entry_offset < data_block->data_size )
		{
			libcnotify_printf(
			 "%s: trailing data:\n",
			 function );
			libcnotify_print_data(
			 &( ( data_block->data )[ data_block_entry_offset ] ),
			 data_block->data_size - data_block_entry_offset,
			 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
		}
		else
		{
			libcnotify_printf(
			 "\n" );
		}
	}
#endif
	return( 1 );

on_error:
#if defined( HAVE_DEBUG_OUTPUT )
	if( key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 &key_name_entry,
		 NULL );
	}
#endif
	if( data_block_entry != NULL )
	{
		libcreg_data_block_entry_free(
		 &data_block_entry,
		 NULL );
	}
	if( data_block->entries_array != NULL )
	{
		libcdata_array_clear(
		 data_block->entries_array,
		 (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_data_block_entry_free,
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

/* Retrieves the key name entry at the index
 * Returns 1 if successful or -1 on error
 */
int libcreg_data_block_get_entry_by_index(
     libcreg_data_block_t *data_block,
     int entry_index,
     libcreg_key_name_entry_t **key_name_entry,
     int ascii_codepage,
     libcerror_error_t **error )
{
	libcreg_data_block_entry_t *data_block_entry = NULL;
	static char *function                        = "libcreg_data_block_get_entry_by_index";

#if defined( HAVE_DEBUG_OUTPUT )
	off64_t file_offset                          = 0;
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
	if( libcdata_array_get_entry_by_index(
	     data_block->entries_array,
	     entry_index,
	     (intptr_t **) &data_block_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve data block entry: %d.",
		 function,
		 entry_index );

		goto on_error;
	}
	if( data_block_entry == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data block - missing data block entry: %d.",
		 function,
		 entry_index );

		goto on_error;
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		file_offset = data_block->offset + sizeof( creg_data_block_header_t ) + data_block_entry->offset;

		libcnotify_printf(
		 "%s: reading key name entry at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libcreg_key_name_entry_initialize(
	     key_name_entry,
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
	if( libcreg_key_name_entry_read(
	     *key_name_entry,
	     &( ( data_block->data )[ data_block_entry->offset ] ),
	     data_block_entry->size,
	     ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key name entry: %d.",
		 function,
		 entry_index );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *key_name_entry != NULL )
	{
		libcreg_key_name_entry_free(
		 key_name_entry,
		 NULL );
	}
	return( -1 );
}

