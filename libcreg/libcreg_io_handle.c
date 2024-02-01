/*
 * Input/Output (IO) handle
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

#include "libcreg_codepage.h"
#include "libcreg_data_block.h"
#include "libcreg_data_type.h"
#include "libcreg_debug.h"
#include "libcreg_definitions.h"
#include "libcreg_io_handle.h"
#include "libcreg_key_hierarchy_entry.h"
#include "libcreg_key_item.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcnotify.h"
#include "libcreg_libfdata.h"
#include "libcreg_libuna.h"
#include "libcreg_unused.h"

#include "creg_file_header.h"
#include "creg_key_navigation.h"

const char *creg_file_signature = "CREG";

/* Creates an IO handle
 * Make sure the value io_handle is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_io_handle_initialize(
     libcreg_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libcreg_io_handle_initialize";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid IO handle value already set.",
		 function );

		return( -1 );
	}
	*io_handle = memory_allocate_structure(
	              libcreg_io_handle_t );

	if( *io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create IO handle.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *io_handle,
	     0,
	     sizeof( libcreg_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		goto on_error;
	}
	( *io_handle )->ascii_codepage = LIBCREG_CODEPAGE_WINDOWS_1252;

	return( 1 );

on_error:
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( -1 );
}

/* Frees an IO handle
 * Returns 1 if successful or -1 on error
 */
int libcreg_io_handle_free(
     libcreg_io_handle_t **io_handle,
     libcerror_error_t **error )
{
	static char *function = "libcreg_io_handle_free";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( *io_handle != NULL )
	{
		memory_free(
		 *io_handle );

		*io_handle = NULL;
	}
	return( 1 );
}

/* Clears the IO handle
 * Returns 1 if successful or -1 on error
 */
int libcreg_io_handle_clear(
     libcreg_io_handle_t *io_handle,
     libcerror_error_t **error )
{
	static char *function = "libcreg_io_handle_clear";

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( memory_set(
	     io_handle,
	     0,
	     sizeof( libcreg_io_handle_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear IO handle.",
		 function );

		return( -1 );
	}
	io_handle->ascii_codepage = LIBCREG_CODEPAGE_WINDOWS_1252;

	return( 1 );
}

/* Reads the file header
 * Returns 1 if successful or -1 on error
 */
int libcreg_io_handle_read_file_header(
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     uint16_t *number_of_data_blocks,
     libcerror_error_t **error )
{
	creg_file_header_t file_header;

	static char *function       = "libcreg_io_handle_read_file_header";
	ssize_t read_count          = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint32_t value_32bit        = 0;
	uint16_t value_16bit        = 0;
#endif

	if( io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid IO handle.",
		 function );

		return( -1 );
	}
	if( number_of_data_blocks == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of data blocks.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading file header at offset: 0 (0x00000000)\n",
		 function );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &file_header,
	              sizeof( creg_file_header_t ),
	              0,
	              error );

	if( read_count != (ssize_t) sizeof( creg_file_header_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read file header data at offset: 0 (0x00000000).",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: file header:\n",
		 function );
		libcnotify_print_data(
		 (uint8_t *) &file_header,
		 sizeof( creg_file_header_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	if( memory_compare(
	     file_header.signature,
	     creg_file_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid file signature.",
		 function );

		return( -1 );
	}
	byte_stream_copy_to_uint16_little_endian(
	 file_header.minor_version,
	 io_handle->minor_version );

	byte_stream_copy_to_uint16_little_endian(
	 file_header.major_version,
	 io_handle->major_version );

	byte_stream_copy_to_uint32_little_endian(
	 file_header.data_blocks_list_offset,
	 io_handle->data_blocks_list_offset );

	byte_stream_copy_to_uint16_little_endian(
	 file_header.number_of_data_blocks,
	 *number_of_data_blocks );

#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 file_header.signature[ 0 ],
		 file_header.signature[ 1 ],
		 file_header.signature[ 2 ],
		 file_header.signature[ 3 ] );

		libcnotify_printf(
		 "%s: minor version\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->minor_version );

		libcnotify_printf(
		 "%s: major version\t\t\t: %" PRIu16 "\n",
		 function,
		 io_handle->major_version );

		libcnotify_printf(
		 "%s: data blocks list offset\t\t: 0x%08" PRIx32 "\n",
		 function,
		 io_handle->data_blocks_list_offset );

		byte_stream_copy_to_uint32_little_endian(
		 file_header.unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: number of data blocks\t\t: %" PRIu16 "\n",
		 function,
		 *number_of_data_blocks );

		byte_stream_copy_to_uint16_little_endian(
		 file_header.unknown2,
		 value_16bit );
		libcnotify_printf(
		 "%s: unknown2\t\t\t\t: 0x%04" PRIx16 "\n",
		 function,
		 value_16bit );

		byte_stream_copy_to_uint32_little_endian(
		 file_header.unknown3,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown3\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		libcnotify_printf(
		 "%s: unknown4:\n",
		 function );
		libcnotify_print_data(
		 file_header.unknown4,
		 8,
		 0 );
	}
#endif
	return( 1 );
}

/* Reads a key hierarchy entry
 * Callback function for the key hiearachy area
 * Returns 1 if successful or -1 on error
 */
int libcreg_io_handle_read_key_hierarchy_entry(
     intptr_t *data_handle LIBCREG_ATTRIBUTE_UNUSED,
     libbfio_handle_t *file_io_handle,
     libfdata_area_t *area,
     libfdata_cache_t *cache,
     off64_t element_value_offset,
     int element_data_file_index LIBCREG_ATTRIBUTE_UNUSED,
     off64_t element_data_offset,
     size64_t element_data_size,
     uint32_t element_data_flags LIBCREG_ATTRIBUTE_UNUSED,
     uint8_t read_flags LIBCREG_ATTRIBUTE_UNUSED,
     libcerror_error_t **error )
{
	creg_key_hierarchy_entry_t key_hierarchy_entry_data;

	libcreg_key_hierarchy_entry_t *key_hierarchy_entry = NULL;
	static char *function                              = "libcreg_io_handle_read_key_hierarchy_entry";
	ssize_t read_count                                 = 0;

	LIBCREG_UNREFERENCED_PARAMETER( data_handle );
	LIBCREG_UNREFERENCED_PARAMETER( element_data_file_index );
	LIBCREG_UNREFERENCED_PARAMETER( element_data_flags );
	LIBCREG_UNREFERENCED_PARAMETER( read_flags );

	if( element_data_size > (size64_t) SSIZE_MAX )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid element data size value exceeds maximum.",
		 function );

		return( -1 );
	}
	if( element_data_size != (size64_t) sizeof( creg_key_hierarchy_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid element data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading key hierarchy entry at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 element_data_offset,
		 element_data_offset );
	}
#endif
	read_count = libbfio_handle_read_buffer_at_offset(
	              file_io_handle,
	              (uint8_t *) &key_hierarchy_entry_data,
	              sizeof( creg_key_hierarchy_entry_t ),
	              element_data_offset,
	              error );

	if( read_count != (ssize_t) sizeof( creg_key_hierarchy_entry_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key hierarchy entry data at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 element_data_offset,
		 element_data_offset );

		goto on_error;
	}
	if( libcreg_key_hierarchy_entry_initialize(
	     &key_hierarchy_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key hierarchy entry.",
		 function );

		goto on_error;
	}
	if( libcreg_key_hierarchy_entry_read_data(
	     key_hierarchy_entry,
	     (uint8_t *) &key_hierarchy_entry_data,
	     sizeof( creg_key_hierarchy_entry_t ),
	     element_data_offset - 0x20,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key hierarchy entry.",
		 function );

		goto on_error;
	}
	if( libfdata_area_set_element_value_at_offset(
	     area,
	     (intptr_t *) file_io_handle,
	     cache,
	     element_value_offset,
	     (intptr_t *) key_hierarchy_entry,
	     (int (*)(intptr_t **, libcerror_error_t **)) &libcreg_key_hierarchy_entry_free,
	     LIBFDATA_VECTOR_ELEMENT_VALUE_FLAG_MANAGED,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set key hierarchy entry as element value.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( key_hierarchy_entry != NULL )
	{
		libcreg_key_hierarchy_entry_free(
		 &key_hierarchy_entry,
		 NULL );
	}
	return( -1 );
}

