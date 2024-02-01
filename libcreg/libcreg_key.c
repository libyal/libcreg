/*
 * Key functions
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
#include <memory.h>
#include <types.h>

#include "libcreg_definitions.h"
#include "libcreg_io_handle.h"
#include "libcreg_key.h"
#include "libcreg_key_descriptor.h"
#include "libcreg_key_item.h"
#include "libcreg_key_name_entry.h"
#include "libcreg_key_navigation.h"
#include "libcreg_key_tree.h"
#include "libcreg_libbfio.h"
#include "libcreg_libcerror.h"
#include "libcreg_libcthreads.h"
#include "libcreg_libuna.h"
#include "libcreg_types.h"
#include "libcreg_value.h"
#include "libcreg_value_entry.h"
#include "libcreg_value_type.h"

/* Creates a key
 * Make sure the value key is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_initialize(
     libcreg_key_t **key,
     libcreg_io_handle_t *io_handle,
     libbfio_handle_t *file_io_handle,
     libcreg_key_navigation_t *key_navigation,
     uint32_t key_offset,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_initialize";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid key value already set.",
		 function );

		return( -1 );
	}
	internal_key = memory_allocate_structure(
	                libcreg_internal_key_t );

	if( internal_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create internal key.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     internal_key,
	     0,
	     sizeof( libcreg_internal_key_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear internal key.",
		 function );

		memory_free(
		 internal_key );

		return( -1 );
	}
	if( libcreg_key_item_initialize(
	     &( internal_key->key_item ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to create key item.",
		 function );

		goto on_error;
	}
	if( libcreg_key_item_read(
	     internal_key->key_item,
	     file_io_handle,
	     key_navigation,
	     (off64_t) key_offset,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read key item at offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 key_offset,
		 key_offset );

		goto on_error;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_initialize(
	     &( internal_key->read_write_lock ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize read/write lock.",
		 function );

		goto on_error;
	}
#endif
	internal_key->file_io_handle = file_io_handle;
	internal_key->io_handle      = io_handle;
	internal_key->key_navigation = key_navigation;
	internal_key->key_offset     = key_offset;

	*key = (libcreg_key_t *) internal_key;

	return( 1 );

on_error:
	if( internal_key != NULL )
	{
		if( internal_key->key_item != NULL )
		{
			libcreg_key_item_free(
			 &( internal_key->key_item ),
			 NULL );
		}
		memory_free(
		 internal_key );
	}
	return( -1 );
}

/* Frees a key
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_free(
     libcreg_key_t **key,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_free";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	if( *key != NULL )
	{
		internal_key = (libcreg_internal_key_t *) *key;
		*key         = NULL;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
		if( libcthreads_read_write_lock_free(
		     &( internal_key->read_write_lock ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free read/write lock.",
			 function );

			result = -1;
		}
#endif
		/* The io_handle and file_io_handle references are freed elsewhere
		 */
		if( libcreg_key_item_free(
		     &( internal_key->key_item ),
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key item.",
			 function );

			result = -1;
		}
		memory_free(
		 internal_key );
	}
	return( result );
}

/* Determine if the key corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
int libcreg_key_is_corrupted(
     libcreg_key_t *key,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_is_corrupted";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->key_item == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - key item.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_item_is_corrupted(
	          internal_key->key_item,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to determine if key item is corruped.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the offset
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_offset(
     libcreg_key_t *key,
     off64_t *offset,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_offset";

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( offset == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid offset.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	/* The offset is relative from the start of the data blocks list
	 * and points to the start of the corresponding key name entry
	 */
	*offset = internal_key->key_offset + internal_key->io_handle->data_blocks_list_offset + 4;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( 1 );
}

/* Retrieves the key name size
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_name_size(
     libcreg_key_t *key,
     size_t *string_size,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_name_size";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_name_size(
	     internal_key->key_item,
	     string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the key name
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_name(
     libcreg_key_t *key,
     uint8_t *string,
     size_t string_size,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_name(
	     internal_key->key_item,
	     string,
	     string_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_utf8_name_size(
     libcreg_key_t *key,
     size_t *utf8_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_utf8_name_size";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_utf8_name_size(
	     internal_key->key_item,
	     utf8_string_size,
	     internal_key->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-8 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_utf8_name(
     libcreg_key_t *key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_utf8_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_utf8_name(
	     internal_key->key_item,
	     utf8_string,
	     utf8_string_size,
	     internal_key->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-8 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_utf16_name_size(
     libcreg_key_t *key,
     size_t *utf16_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_utf16_name_size";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_utf16_name_size(
	     internal_key->key_item,
	     utf16_string_size,
	     internal_key->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name size.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the UTF-16 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_utf16_name(
     libcreg_key_t *key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_value_get_utf16_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_utf16_name(
	     internal_key->key_item,
	     utf16_string,
	     utf16_string_size,
	     internal_key->io_handle->ascii_codepage,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve UTF-16 name.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_number_of_values(
     libcreg_key_t *key,
     int *number_of_values,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_number_of_values";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_number_of_value_entries(
	     internal_key->key_item,
	     number_of_values,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of value entries.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves a specific value
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_value_by_index(
     libcreg_key_t *key,
     int value_index,
     libcreg_value_t **value,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	libcreg_value_entry_t *value_entry   = NULL;
	static char *function                = "libcreg_key_get_value_by_index";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_value_entry_by_index(
	     internal_key->key_item,
	     value_index,
	     &value_entry,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value entry: %d.",
		 function,
		 value_index );

		result = -1;
	}
	else if( libcreg_value_initialize(
	          value,
	          internal_key->io_handle,
	          value_entry,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize value.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the value for the specific UTF-8 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcreg_key_get_value_by_utf8_name(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_value_t **value,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	libcreg_value_entry_t *value_entry   = NULL;
	static char *function                = "libcreg_key_get_value_by_utf8_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_item_get_value_by_utf8_name(
	          internal_key->key_item,
	          utf8_string,
	          utf8_string_length,
	          internal_key->io_handle->ascii_codepage,
	          &value_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value entry by UTF-8 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libcreg_value_initialize(
		     value,
		     internal_key->io_handle,
		     value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize value.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the value for the specific UTF-16 encoded name
 * To retrieve the default value specify string as NULL
 * and its length as 0
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
int libcreg_key_get_value_by_utf16_name(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_value_t **value,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	libcreg_value_entry_t *value_entry   = NULL;
	static char *function                = "libcreg_key_get_value_by_utf16_name";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
	if( value == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid value.",
		 function );

		return( -1 );
	}
	if( *value != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: value already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_item_get_value_by_utf16_name(
	          internal_key->key_item,
	          utf16_string,
	          utf16_string_length,
	          internal_key->io_handle->ascii_codepage,
	          &value_entry,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value entry by UTF-16 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( libcreg_value_initialize(
		     value,
		     internal_key->io_handle,
		     value_entry,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize value.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the number of sub keys
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_number_of_sub_keys(
     libcreg_key_t *key,
     int *number_of_sub_keys,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_number_of_sub_keys";
	int result                           = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_number_of_sub_key_descriptors(
	     internal_key->key_item,
	     number_of_sub_keys,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve number of sub key descriptors.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub key for the specific index
 * Returns 1 if successful or -1 on error
 */
int libcreg_key_get_sub_key_by_index(
     libcreg_key_t *key,
     int sub_key_index,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key         = NULL;
	libcreg_key_descriptor_t *sub_key_descriptor = NULL;
	static char *function                        = "libcreg_key_get_sub_key_by_index";
	int result                                   = 1;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( sub_key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid sub key.",
		 function );

		return( -1 );
	}
	if( *sub_key != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: sub key already set.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	if( libcreg_key_item_get_sub_key_descriptor_by_index(
	     internal_key->key_item,
             sub_key_index,
	     &sub_key_descriptor,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key descriptor: %d.",
		 function,
		 sub_key_index );

		result = -1;
	}
	else if( sub_key_descriptor == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid sub key descriptor: %d.",
		 function,
		 sub_key_index );

		result = -1;
	}
	else if( libcreg_key_initialize(
	          sub_key,
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          internal_key->key_navigation,
	          sub_key_descriptor->key_offset,
	          error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
		 "%s: unable to initialize sub key.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub key for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libcreg_key_get_sub_key_by_utf8_name(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_key_descriptor_t *sub_key_descriptor = NULL;
	libcreg_internal_key_t *internal_key         = NULL;
	static char *function                        = "libcreg_key_get_sub_key_by_utf8_name";
	int result                                   = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_item_get_sub_key_descriptor_by_utf8_name(
	          internal_key->key_item,
	          internal_key->file_io_handle,
	          internal_key->key_navigation,
	          utf8_string,
	          utf8_string_length,
	          internal_key->io_handle->ascii_codepage,
	          &sub_key_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key descriptor by UTF-8 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub key descriptor.",
			 function );

			result = -1;
		}
		else if( libcreg_key_initialize(
			  sub_key,
			  internal_key->io_handle,
			  internal_key->file_io_handle,
			  internal_key->key_navigation,
			  sub_key_descriptor->key_offset,
			  error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub key.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub key for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libcreg_key_get_sub_key_by_utf8_path(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_sub_key_by_utf8_path";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_tree_get_sub_key_by_utf8_path(
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          internal_key->key_navigation,
	          internal_key->key_offset,
	          utf8_string,
	          utf8_string_length,
	          internal_key->io_handle->ascii_codepage,
	          sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-8 path.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub key for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
int libcreg_key_get_sub_key_by_utf16_name(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_key_descriptor_t *sub_key_descriptor = NULL;
	libcreg_internal_key_t *internal_key         = NULL;
	static char *function                        = "libcreg_key_get_sub_key_by_utf16_name";
	int result                                   = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_item_get_sub_key_descriptor_by_utf16_name(
	          internal_key->key_item,
	          internal_key->file_io_handle,
	          internal_key->key_navigation,
	          utf16_string,
	          utf16_string_length,
	          internal_key->io_handle->ascii_codepage,
	          &sub_key_descriptor,
	          error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sub key descriptor by UTF-16 name.",
		 function );

		result = -1;
	}
	else if( result != 0 )
	{
		if( sub_key_descriptor == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
			 "%s: invalid sub key descriptor.",
			 function );

			result = -1;
		}
		else if( libcreg_key_initialize(
			  sub_key,
			  internal_key->io_handle,
			  internal_key->file_io_handle,
			  internal_key->key_navigation,
			  sub_key_descriptor->key_offset,
			  error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_INITIALIZE_FAILED,
			 "%s: unable to initialize sub key.",
			 function );

			result = -1;
		}
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

/* Retrieves the sub key for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
int libcreg_key_get_sub_key_by_utf16_path(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **sub_key,
     libcerror_error_t **error )
{
	libcreg_internal_key_t *internal_key = NULL;
	static char *function                = "libcreg_key_get_sub_key_by_utf16_path";
	int result                           = 0;

	if( key == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid key.",
		 function );

		return( -1 );
	}
	internal_key = (libcreg_internal_key_t *) key;

	if( internal_key->io_handle == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid key - missing IO handle.",
		 function );

		return( -1 );
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_grab_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to grab read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	result = libcreg_key_tree_get_sub_key_by_utf16_path(
	          internal_key->io_handle,
	          internal_key->file_io_handle,
	          internal_key->key_navigation,
	          internal_key->key_offset,
	          utf16_string,
	          utf16_string_length,
	          internal_key->io_handle->ascii_codepage,
	          sub_key,
		  error );

	if( result == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key by UTF-16 path.",
		 function );

		result = -1;
	}
#if defined( HAVE_LIBCREG_MULTI_THREAD_SUPPORT )
	if( libcthreads_read_write_lock_release_for_write(
	     internal_key->read_write_lock,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to release read/write lock for writing.",
		 function );

		return( -1 );
	}
#endif
	return( result );
}

