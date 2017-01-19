/*
 * Mounts a Windows 9x/Me Registry File (CREG)
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
#include <file_stream.h>
#include <memory.h>
#include <narrow_string.h>
#include <system_string.h>
#include <types.h>
#include <wide_string.h>

#if defined( HAVE_ERRNO_H )
#include <errno.h>
#endif

#if defined( HAVE_UNISTD_H )
#include <unistd.h>
#endif

#if defined( HAVE_STDLIB_H ) || defined( WINAPI )
#include <stdlib.h>
#endif

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
#define FUSE_USE_VERSION	26

#if defined( HAVE_LIBFUSE )
#include <fuse.h>

#elif defined( HAVE_LIBOSXFUSE )
#include <osxfuse/fuse.h>
#endif

#elif defined( HAVE_LIBDOKAN )
#include <dokan.h>
#endif

#include "cregtools_getopt.h"
#include "cregtools_libcerror.h"
#include "cregtools_libclocale.h"
#include "cregtools_libcnotify.h"
#include "cregtools_libcreg.h"
#include "cregtools_output.h"
#include "cregtools_signal.h"
#include "cregtools_unused.h"
#include "mount_handle.h"

enum CREGMOUNT_FILE_ENTRY_TYPES
{
	CREGMOUNT_FILE_ENTRY_TYPE_ROOT,
	CREGMOUNT_FILE_ENTRY_TYPE_KEY,
	CREGMOUNT_FILE_ENTRY_TYPE_VALUES,
	CREGMOUNT_FILE_ENTRY_TYPE_VALUE
};

mount_handle_t *cregmount_mount_handle = NULL;
int cregmount_abort                    = 0;

/* Prints the executable usage information
 */
void usage_fprint(
      FILE *stream )
{
	if( stream == NULL )
	{
		return;
	}
	fprintf( stream, "Use cregmount to mount a Windows 9x/Me Registry File (CREG).\n\n" );

	fprintf( stream, "Usage: cregmount [ -c codepage ] [ -X extended_options ]\n"
	                 "                 [ -hvV ] source mount_point\n\n" );

	fprintf( stream, "\tsource:      the source CREG file\n\n" );
	fprintf( stream, "\tmount_point: the directory to serve as mount point\n\n" );

	fprintf( stream, "\t-c:          codepage of ASCII strings, options: ascii,\n"
	                 "\t             windows-874, windows-932, windows-936,\n"
	                 "\t             windows-949, windows-950, windows-1250,\n"
	                 "\t             windows-1251, windows-1252 (default),\n"
	                 "\t             windows-1253, windows-1254, windows-1255,\n"
	                 "\t             windows-1256, windows-1257 or windows-1258\n" );
	fprintf( stream, "\t-h:          shows this help\n" );
	fprintf( stream, "\t-v:          verbose output to stderr\n"
	                 "\t             cregmount will remain running in the foreground\n" );
	fprintf( stream, "\t-V:          print version\n" );
	fprintf( stream, "\t-X:          extended options to pass to sub system\n" );
}

/* Signal handler for cregmount
 */
void cregmount_signal_handler(
      cregtools_signal_t signal CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cregmount_signal_handler";

	CREGTOOLS_UNREFERENCED_PARAMETER( signal )

	cregmount_abort = 1;

	if( cregmount_mount_handle != NULL )
	{
		if( mount_handle_signal_abort(
		     cregmount_mount_handle,
		     &error ) != 1 )
		{
			libcnotify_printf(
			 "%s: unable to signal mount handle to abort.\n",
			 function );

			libcnotify_print_error_backtrace(
			 error );
			libcerror_error_free(
			 &error );
		}
	}
	/* Force stdin to close otherwise any function reading it will remain blocked
	 */
#if defined( WINAPI ) && !defined( __CYGWIN__ )
	if( _close(
	     0 ) != 0 )
#else
	if( close(
	     0 ) != 0 )
#endif
	{
		libcnotify_printf(
		 "%s: unable to close stdin.\n",
		 function );
	}
}

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )

#if ( SIZEOF_OFF_T != 8 ) && ( SIZEOF_OFF_T != 4 )
#error Size of off_t not supported
#endif

#if defined( HAVE_TIME )
time_t cregmount_timestamp = 0;
#endif

/* Opens a file
 * Returns 0 if successful or a negative errno value otherwise
 */
int cregmount_fuse_open(
     const char *path,
     struct fuse_file_info *file_info )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_value_t *value        = NULL;
	static char *function         = "cregmount_fuse_open";
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int result                    = 0;
	uint8_t file_entry_type       = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	normalized_path_length = path_length;

	if( ( normalized_path_length >= 1 )
	 && ( path[ normalized_path_length - 1 ] == '/' ) )
	{
		normalized_path_length -= 1;
	}
	if( ( normalized_path_length >= 9 )
	 && ( path[ normalized_path_length - 9 ] == '/' )
	 && ( system_string_compare(
	       &( path[ normalized_path_length - 8 ] ),
	       _SYSTEM_STRING( "(values)" ),
	       8 ) == 0 ) )
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
	}
	else
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
	}
	if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES )
	{
		result = -ENOENT;

		goto on_error;
	}
	if( mount_handle_get_value_by_path(
	     cregmount_mount_handle,
	     path,
	     path_length,
	     (system_character_t) '/',
	     &key,
	     &value,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value for: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	if( value != NULL )
	{
		if( libcreg_value_free(
		     &value,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	if( key != NULL )
	{
		if( libcreg_key_free(
		     &key,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	if( ( file_info->flags & 0x03 ) != O_RDONLY )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: write access currently not supported.",
		 function );

		result = -EACCES;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns number of bytes read if successful or a negative errno value otherwise
 */
int cregmount_fuse_read(
     const char *path,
     char *buffer,
     size_t size,
     off_t offset,
     struct fuse_file_info *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_value_t *value        = NULL;
	uint8_t *data                 = NULL;
	static char *function         = "cregmount_fuse_read";
	size_t data_size              = 0;
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int read_count                = 0;
	int result                    = 0;
	uint8_t file_entry_type       = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( buffer == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid buffer.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( size > (size_t) INT_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid size value exceeds maximum.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( offset < 0 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid offset value out of bounds.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	normalized_path_length = path_length;

	if( ( normalized_path_length >= 1 )
	 && ( path[ normalized_path_length - 1 ] == '/' ) )
	{
		normalized_path_length -= 1;
	}
	if( ( normalized_path_length >= 9 )
	 && ( path[ normalized_path_length - 9 ] == '/' )
	 && ( system_string_compare(
	       &( path[ normalized_path_length - 8 ] ),
	       _SYSTEM_STRING( "(values)" ),
	       8 ) == 0 ) )
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
	}
	else
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
	}
	if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES )
	{
		result = -ENOENT;

		goto on_error;
	}
	if( mount_handle_get_value_by_path(
	     cregmount_mount_handle,
	     path,
	     path_length,
	     (system_character_t) '/',
	     &key,
	     &value,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value for: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	if( libcreg_value_get_value_data_size(
	     value,
	     &data_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( data_size > 0 )
	{
/* TODO add bounds check */
		data = (uint8_t *) memory_allocate(
		                    sizeof( uint8_t ) * data_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			result = -EIO;

			goto on_error;
		}
		if( libcreg_value_get_value_data(
		     value,
		     data,
		     data_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	if( offset < (off_t) data_size )
	{
		if( ( offset + (off_t) size ) > (off_t) data_size )
		{
			size = data_size - (size_t) offset;
		}
		if( memory_copy(
		     buffer,
		     &( data[ offset ] ),
		     size ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			result = -EIO;

			goto on_error;
		}
		read_count = (int) size;
	}
	if( value != NULL )
	{
		if( libcreg_value_free(
		     &value,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	if( key != NULL )
	{
		if( libcreg_key_free(
		     &key,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			result = -EIO;

			goto on_error;
		}
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	return( read_count );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Sets the values in a stat info structure
 * Returns 1 if successful or -1 on error
 */
int cregmount_fuse_set_stat_info(
     struct stat *stat_info,
     size64_t data_size,
     uint8_t file_entry_type,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function = "cregmount_fuse_set_stat_info";

	if( stat_info == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		return( -1 );
	}
	if( ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUE )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: supported file entry type.",
		 function );

		return( -1 );
	}
#if defined( HAVE_TIME )
	if( use_mount_time != 0 )
	{
		if( cregmount_timestamp == 0 )
		{
			if( time(
			     &cregmount_timestamp ) == (time_t) -1 )
			{
				cregmount_timestamp = 0;
			}
		}
		stat_info->st_atime = cregmount_timestamp;
		stat_info->st_mtime = cregmount_timestamp;
		stat_info->st_ctime = cregmount_timestamp;
	}
#endif
	if( data_size != 0 )
	{
/* TODO check upper bound */
		stat_info->st_size = (off_t) data_size;
	}
	if( ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 || ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 || ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		stat_info->st_mode  = S_IFDIR | 0555;
		stat_info->st_nlink = 2;
	}
	else
	{
		stat_info->st_mode  = S_IFREG | 0444;
		stat_info->st_nlink = 1;
	}
#if defined( HAVE_GETEUID )
	stat_info->st_uid = geteuid();
#endif
#if defined( HAVE_GETEGID )
	stat_info->st_gid = getegid();
#endif
	return( 1 );
}

/* Fills a directory entry
 * Returns 1 if successful or -1 on error
 */
int cregmount_fuse_filldir(
     void *buffer,
     fuse_fill_dir_t filler,
     char *name,
     size_t name_size,
     struct stat *stat_info,
     libcreg_key_t *key,
     libcreg_value_t *value,
     uint8_t file_entry_type,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	char *sanitized_name       = NULL;
	static char *function      = "cregmount_fuse_filldir";
	size_t data_size           = 0;
	size_t sanitized_name_size = 0;

	if( name == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid name.",
		 function );

		return( -1 );
	}
	if( stat_info == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		return( -1 );
	}
	if( ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUE )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: supported file entry type.",
		 function );

		return( -1 );
	}
	if( mount_handle_get_sanitized_filename(
	     cregmount_mount_handle,
	     name,
	     name_size,
	     (system_character_t) '/',
	     &sanitized_name,
	     &sanitized_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sanitized filename.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stat info.",
		 function );

		goto on_error;
	}
	if( value != NULL )
	{
		if( libcreg_value_get_value_data_size(
		     value,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data size.",
			 function );

			goto on_error;
		}
	}
	if( cregmount_fuse_set_stat_info(
	     stat_info,
	     (size64_t) data_size,
	     file_entry_type,
	     use_mount_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set stat info.",
		 function );

		goto on_error;
	}
	if( filler(
	     buffer,
	     sanitized_name,
	     stat_info,
	     0 ) == 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		goto on_error;
	}
	memory_free(
	 sanitized_name );

	sanitized_name = NULL;

	return( 1 );

on_error:
	if( sanitized_name != NULL )
	{
		memory_free(
		 sanitized_name );
	}
	return( -1 );
}

/* Reads a directory
 * Returns 0 if successful or a negative errno value otherwise
 */
int cregmount_fuse_readdir(
     const char *path,
     void *buffer,
     fuse_fill_dir_t filler,
     off_t offset CREGTOOLS_ATTRIBUTE_UNUSED,
     struct fuse_file_info *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	struct stat *stat_info        = NULL;
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_key_t *sub_key        = NULL;
	libcreg_value_t *value        = NULL;
	char *name                    = NULL;
	static char *function         = "cregmount_fuse_readdir";
	size_t name_size              = 0;
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int number_of_sub_keys        = 0;
	int number_of_values          = 0;
	int result                    = 0;
	int sub_key_index             = 0;
	int value_index               = 0;
	uint8_t file_entry_type       = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( offset )
	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	stat_info = memory_allocate_structure(
	             struct stat );

	if( stat_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create stat info.",
		 function );

		result = -EIO;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	normalized_path_length = path_length;

	if( ( normalized_path_length >= 1 )
	 && ( path[ normalized_path_length - 1 ] == '/' ) )
	{
		normalized_path_length -= 1;
	}
	if( ( normalized_path_length >= 9 )
	 && ( path[ normalized_path_length - 9 ] == '/' )
	 && ( system_string_compare(
	       &( path[ normalized_path_length - 8 ] ),
	       _SYSTEM_STRING( "(values)" ),
	       8 ) == 0 ) )
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
	}
	else
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
	}
	if( mount_handle_get_key_by_path(
	     cregmount_mount_handle,
	     path,
	     path_length,
	     (system_character_t) '/',
	     &key,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key for: %s.",
		 function,
		 path );

		result = -ENOENT;

		goto on_error;
	}
	if( cregmount_fuse_filldir(
	     buffer,
	     filler,
	     ".",
	     2,
	     stat_info,
	     key,
	     NULL,
	     CREGMOUNT_FILE_ENTRY_TYPE_KEY,
	     0,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( cregmount_fuse_filldir(
	     buffer,
	     filler,
	     "..",
	     3,
	     stat_info,
	     key,
	     NULL,
	     CREGMOUNT_FILE_ENTRY_TYPE_KEY,
	     0,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		result = -EIO;

		goto on_error;
	}
	if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	{
		if( libcreg_key_get_number_of_sub_keys(
		     key,
		     &number_of_sub_keys,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			result = -EIO;

			goto on_error;
		}
		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libcreg_key_get_sub_key(
			     key,
			     sub_key_index,
			     &sub_key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve sub key: %d.",
				 function,
				 sub_key_index );

				result = -EIO;

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libcreg_key_get_utf16_name_size(
				  sub_key,
				  &name_size,
				  &error );
#else
			result = libcreg_key_get_utf8_name_size(
				  sub_key,
				  &name_size,
				  &error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the sub key name size.",
				 function );

				result = -EIO;

				goto on_error;
			}
			if( name_size > 0 )
			{
				if( name_size > (size_t) SSIZE_MAX )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid name size value exceeds maximum.",
					 function );

					result = -EIO;

					goto on_error;
				}
				name = narrow_string_allocate(
					name_size );

				if( name == NULL )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create sub key name.",
					 function );

					result = -EIO;

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libcreg_key_get_utf16_name(
					  sub_key,
					  (uint16_t *) name,
					  name_size,
					  &error );
#else
				result = libcreg_key_get_utf8_name(
					  sub_key,
					  (uint8_t *) name,
					  name_size,
					  &error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the sub key name.",
					 function );

					result = -EIO;

					goto on_error;
				}
				if( cregmount_fuse_filldir(
				     buffer,
				     filler,
				     name,
				     name_size,
				     stat_info,
				     sub_key,
				     NULL,
				     CREGMOUNT_FILE_ENTRY_TYPE_KEY,
				     0,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set directory entry.",
					 function );

					result = -EIO;

					goto on_error;
				}
				memory_free(
				 name );

				name = NULL;
			}
			if( libcreg_key_free(
			     &sub_key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				result = -EIO;

				goto on_error;
			}
		}
		if( libcreg_key_get_number_of_values(
		     key,
		     &number_of_values,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values.",
			 function );

			result = -EIO;

			goto on_error;
		}
		if( number_of_values > 0 )
		{
			/* Make the values accessible
			 */
			if( cregmount_fuse_filldir(
			     buffer,
			     filler,
			     "(values)",
			     9,
			     stat_info,
			     key,
			     NULL,
			     CREGMOUNT_FILE_ENTRY_TYPE_VALUES,
			     0,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set directory entry.",
				 function );

				result = -EIO;

				goto on_error;
			}
		}
	}
	else if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES )
	{
		if( libcreg_key_get_number_of_values(
		     key,
		     &number_of_values,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values.",
			 function );

			result = -EIO;

			goto on_error;
		}
		for( value_index = 0;
		     value_index < number_of_values;
		     value_index++ )
		{
			if( libcreg_key_get_value(
			     key,
			     value_index,
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve value: %d.",
				 function,
				 value_index );

				result = -EIO;

				goto on_error;
			}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
			result = libcreg_value_get_utf16_name_size(
				  value,
				  &name_size,
				  &error );
#else
			result = libcreg_value_get_utf8_name_size(
				  value,
				  &name_size,
				  &error );
#endif
			if( result != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the value size.",
				 function );

				result = -EIO;

				goto on_error;
			}
			if( name_size > 0 )
			{
				if( name_size > (size_t) SSIZE_MAX )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid name size value exceeds maximum.",
					 function );

					result = -EIO;

					goto on_error;
				}
				name = narrow_string_allocate(
					name_size );

				if( name == NULL )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create value name.",
					 function );

					result = -EIO;

					goto on_error;
				}
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
				result = libcreg_value_get_utf16_name(
					  value,
					  (uint16_t *) name,
					  name_size,
					  &error );
#else
				result = libcreg_value_get_utf8_name(
					  value,
					  (uint8_t *) name,
					  name_size,
					  &error );
#endif
				if( result != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the value name.",
					 function );

					result = -EIO;

					goto on_error;
				}
				if( cregmount_fuse_filldir(
				     buffer,
				     filler,
				     name,
				     name_size,
				     stat_info,
				     key,
				     value,
				     CREGMOUNT_FILE_ENTRY_TYPE_VALUE,
				     0,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set directory entry.",
					 function );

					result = -EIO;

					goto on_error;
				}
				memory_free(
				 name );

				name = NULL;
			}
			else
			{
				/* Make the default value accessible
				 */
				if( cregmount_fuse_filldir(
				     buffer,
				     filler,
				     "(default)",
				     10,
				     stat_info,
				     key,
				     value,
				     CREGMOUNT_FILE_ENTRY_TYPE_VALUE,
				     0,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set directory entry.",
					 function );

					result = -EIO;

					goto on_error;
				}
			}
			if( libcreg_value_free(
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value: %d.",
				 function,
				 value_index );

				result = -EIO;

				goto on_error;
			}
		}
	}
	if( libcreg_key_free(
	     &key,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free key.",
		 function );

		result = -EIO;

		goto on_error;
	}
	memory_free(
	 stat_info );

	stat_info = NULL;

	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	if( stat_info != NULL )
	{
		memory_free(
		 stat_info );
	}
	return( result );
}

/* Retrieves the file stat info
 * Returns 0 if successful or a negative errno value otherwise
 */
int cregmount_fuse_getattr(
     const char *path,
     struct stat *stat_info )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_value_t *value        = NULL;
	uint8_t *data                 = NULL;
	static char *function         = "cregmount_fuse_getattr";
	size_t data_size              = 0;
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int result                    = -ENOENT;
	uint8_t file_entry_type       = 0;
	uint8_t use_mount_time        = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( stat_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid stat info.",
		 function );

		result = -EINVAL;

		goto on_error;
	}
	if( memory_set(
	     stat_info,
	     0,
	     sizeof( struct stat ) ) == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear stat info.",
		 function );

		result = errno;

		goto on_error;
	}
	path_length = narrow_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] == '/' )
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_ROOT;
			use_mount_time  = 1;
			result          = 0;
		}
	}
	else
	{
		normalized_path_length = path_length;

		if( ( normalized_path_length >= 1 )
		 && ( path[ normalized_path_length - 1 ] == '/' ) )
		{
			normalized_path_length -= 1;
		}
		if( ( normalized_path_length >= 9 )
		 && ( path[ normalized_path_length - 9 ] == '/' )
		 && ( system_string_compare(
		       &( path[ normalized_path_length - 8 ] ),
		       _SYSTEM_STRING( "(values)" ),
		       8 ) == 0 ) )
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
		}
		else
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
		}
		result = mount_handle_get_key_by_path(
			  cregmount_mount_handle,
			  path,
			  path_length,
			  (system_character_t) '/',
			  &key,
			  &error );

		if( result == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key for: %s.",
			 function,
			 path );

			result = -ENOENT;

			goto on_error;
		}
		else if( ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
		      && ( result == 0 ) )
		{
			result = mount_handle_get_value_by_path(
				  cregmount_mount_handle,
				  path,
				  path_length,
				  (system_character_t) '/',
				  &key,
				  &value,
				  &error );

			if( result == -1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value for: %s.",
				 function,
				 path );

				result = -ENOENT;

				goto on_error;
			}
			else if( result != 0 )
			{
				file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUE;
			}
		}
		if( result == 0 )
		{
			result = -ENOENT;

			goto on_error;
		}
		if( value != NULL )
		{
			if( libcreg_value_get_value_data_size(
			     value,
			     &data_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data size.",
				 function );

				result = -EIO;

				goto on_error;
			}
			if( libcreg_value_free(
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -EIO;

				goto on_error;
			}
		}
		if( key != NULL )
		{
			if( libcreg_key_free(
			     &key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key.",
				 function );

				result = -EIO;

				goto on_error;
			}
		}
		use_mount_time  = 1;
		result          = 0;
	}
	if( result == 0 )
	{
		if( cregmount_fuse_set_stat_info(
		     stat_info,
		     (size64_t) data_size,
		     file_entry_type,
		     use_mount_time,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
			 "%s: unable to set stat info.",
			 function );

			goto on_error;
		}
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Cleans up when fuse is done
 */
void cregmount_fuse_destroy(
      void *private_data CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cregmount_fuse_destroy";

	CREGTOOLS_UNREFERENCED_PARAMETER( private_data )

	if( cregmount_mount_handle != NULL )
	{
		if( mount_handle_free(
		     &cregmount_mount_handle,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free mount handle.",
			 function );

			goto on_error;
		}
	}
	return;

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return;
}

#elif defined( HAVE_LIBDOKAN )

/* Opens a file or directory
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_CreateFile(
               const wchar_t *path,
               DWORD desired_access,
               DWORD share_mode CREGTOOLS_ATTRIBUTE_UNUSED,
               DWORD creation_disposition,
               DWORD attribute_flags CREGTOOLS_ATTRIBUTE_UNUSED,
               DOKAN_FILE_INFO *file_info )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_value_t *value        = NULL;
	static char *function         = "cregmount_dokan_CreateFile";
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int result                    = 0;
	uint8_t file_entry_type       = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( share_mode )
	CREGTOOLS_UNREFERENCED_PARAMETER( attribute_flags )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( ( desired_access & GENERIC_WRITE ) != 0 )
	{
		return( -ERROR_WRITE_PROTECT );
	}
	/* Ignore the share_mode
	 */
	if( creation_disposition == CREATE_NEW )
	{
		return( -ERROR_FILE_EXISTS );
	}
	else if( creation_disposition == CREATE_ALWAYS )
	{
		return( -ERROR_ALREADY_EXISTS );
	}
	else if( creation_disposition == OPEN_ALWAYS )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition == TRUNCATE_EXISTING )
	{
		return( -ERROR_FILE_NOT_FOUND );
	}
	else if( creation_disposition != OPEN_EXISTING )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid creation disposition.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] != (wchar_t) '\\' )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid path: %ls.",
			 function,
			 path );

			result = -ERROR_BAD_ARGUMENTS;

			goto on_error;
		}
	}
	else
	{
		normalized_path_length = path_length;

		if( ( normalized_path_length >= 1 )
		 && ( path[ normalized_path_length - 1 ] == '\\' ) )
		{
			normalized_path_length -= 1;
		}
		if( ( normalized_path_length >= 9 )
		 && ( path[ normalized_path_length - 9 ] == '\\' )
		 && ( system_string_compare(
		       &( path[ normalized_path_length - 8 ] ),
		       _SYSTEM_STRING( "(values)" ),
		       8 ) == 0 ) )
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
		}
		else
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
		}
		result = mount_handle_get_key_by_path(
		          cregmount_mount_handle,
		          path,
		          path_length,
		          (system_character_t) '\\',
		          &key,
		          &error );

		if( result == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key for: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		else if( ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
		      && ( result == 0 ) )
		{
			result = mount_handle_get_value_by_path(
				  cregmount_mount_handle,
				  path,
				  path_length,
			          (system_character_t) '\\',
				  &key,
				  &value,
				  &error );

			if( result == -1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value for: %ls.",
				 function,
				 path );

				result = -ERROR_FILE_NOT_FOUND;

				goto on_error;
			}
		}
		if( result == 0 )
		{
			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( value != NULL )
		{
			if( libcreg_value_free(
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
		if( key != NULL )
		{
			if( libcreg_key_free(
			     &key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Opens a directory
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_OpenDirectory(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	static char *function         = "cregmount_dokan_OpenDirectory";
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int result                    = 0;
	uint8_t file_entry_type       = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] != (wchar_t) '\\' )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
			 "%s: invalid path: %ls.",
			 function,
			 path );

			result = -ERROR_BAD_ARGUMENTS;

			goto on_error;
		}
	}
	else
	{
		normalized_path_length = path_length;

		if( ( normalized_path_length >= 1 )
		 && ( path[ normalized_path_length - 1 ] == '\\' ) )
		{
			normalized_path_length -= 1;
		}
		if( ( normalized_path_length >= 9 )
		 && ( path[ normalized_path_length - 9 ] == '\\' )
		 && ( system_string_compare(
		       &( path[ normalized_path_length - 8 ] ),
		       _SYSTEM_STRING( "(values)" ),
		       8 ) == 0 ) )
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
		}
		else
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
		}
		result = mount_handle_get_key_by_path(
		          cregmount_mount_handle,
		          path,
		          path_length,
		          (system_character_t) '\\',
		          &key,
		          &error );

		if( result == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key for: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		else if( result == 0 )
		{
			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( libcreg_key_free(
		     &key,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Closes a file or direcotry
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_CloseFile(
               const wchar_t *path,
               DOKAN_FILE_INFO *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cregmount_dokan_CloseFile";
	int result               = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Reads a buffer of data at the specified offset
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_ReadFile(
               const wchar_t *path,
               void *buffer,
               DWORD number_of_bytes_to_read,
               DWORD *number_of_bytes_read,
               LONGLONG offset,
               DOKAN_FILE_INFO *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_value_t *value        = NULL;
	uint8_t *data                 = NULL;
	static char *function         = "cregmount_dokan_ReadFile";
	size_t data_size              = 0;
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int read_count                = 0;
	int result                    = 0;
	uint8_t file_entry_type       = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_to_read > (DWORD) INT32_MAX )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_VALUE_EXCEEDS_MAXIMUM,
		 "%s: invalid number of bytes to read value exceeds maximum.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( number_of_bytes_read == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid number of bytes read.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	normalized_path_length = path_length;

	if( ( normalized_path_length >= 1 )
	 && ( path[ normalized_path_length - 1 ] == '\\' ) )
	{
		normalized_path_length -= 1;
	}
	if( ( normalized_path_length >= 9 )
	 && ( path[ normalized_path_length - 9 ] == '\\' )
	 && ( system_string_compare(
	       &( path[ normalized_path_length - 8 ] ),
	       _SYSTEM_STRING( "(values)" ),
	       8 ) == 0 ) )
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
	}
	else
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
	}
	if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES )
	{
		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	result = mount_handle_get_value_by_path(
		  cregmount_mount_handle,
		  path,
		  path_length,
		  (system_character_t) '\\',
		  &key,
		  &value,
		  &error );

	if( result == -1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value for: %ls.",
		 function,
		 path );

		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	if( result == 0 )
	{
		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	if( libcreg_value_get_value_data_size(
	     value,
	     &data_size,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve value data size.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( data_size > 0 )
	{
/* TODO add bounds check */
		data = (uint8_t *) memory_allocate(
		                    sizeof( uint8_t ) * data_size );

		if( data == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
			 "%s: unable to create value data.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		if( libcreg_value_get_value_data(
		     value,
		     data,
		     data_size,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	if( offset < (LONGLONG) data_size )
	{
		if( ( offset + (LONGLONG) number_of_bytes_to_read ) > (LONGLONG) data_size )
		{
			number_of_bytes_to_read = (DWORD) data_size - (DWORD) offset;
		}
		if( memory_copy(
		     buffer,
		     &( data[ offset ] ),
		     (size_t) number_of_bytes_to_read ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy data.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		read_count = (int) number_of_bytes_to_read;
	}
	if( value != NULL )
	{
		if( libcreg_value_free(
		     &value,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free value.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	if( key != NULL )
	{
		if( libcreg_key_free(
		     &key,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
			 "%s: unable to free key.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	/* Dokan does not require the read function to return ERROR_HANDLE_EOF
	 */
	*number_of_bytes_read = (DWORD) read_count;

	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( data != NULL )
	{
		memory_free(
		 data );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Sets the values in a find data structure
 * Returns 1 if successful or -1 on error
 */
int cregmount_dokan_set_find_data(
     WIN32_FIND_DATAW *find_data,
     size64_t size,
     uint8_t file_entry_type,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function = "cregmount_dokan_set_find_data";

	if( find_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid find data.",
		 function );

		return( -1 );
	}
	if( ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUE )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: supported file entry type.",
		 function );

		return( -1 );
	}
/* TODO implement
	if( use_mount_time != 0 )
	{
	}
*/
	if( size > 0 )
	{
		find_data->nFileSizeHigh = (DWORD) ( size >> 32 );
		find_data->nFileSizeLow  = (DWORD) ( size & 0xffffffffUL );
	}
	find_data->dwFileAttributes = FILE_ATTRIBUTE_READONLY;

	if( ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 || ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 || ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		find_data->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	}
	return( 1 );
}

/* Fills a directory entry
 * Returns 1 if successful or -1 on error
 */
int cregmount_dokan_filldir(
     PFillFindData fill_find_data,
     DOKAN_FILE_INFO *file_info,
     wchar_t *name,
     size_t name_size,
     WIN32_FIND_DATAW *find_data,
     libcreg_key_t *key,
     libcreg_value_t *value,
     uint8_t file_entry_type,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	wchar_t *sanitized_name    = NULL;
	static char *function      = "cregmount_dokan_filldir";
	size_t sanitized_name_size = 0;
	uint32_t data_size         = 0;

	if( fill_find_data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid fill find data.",
		 function );

		return( -1 );
	}
	if( ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUE )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: supported file entry type.",
		 function );

		return( -1 );
	}
	if( mount_handle_get_sanitized_filename(
	     cregmount_mount_handle,
	     name,
	     name_size,
	     (system_character_t) '\\',
	     &sanitized_name,
	     &sanitized_name_size,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve sanitized filename.",
		 function );

		goto on_error;
	}
	if( value != NULL )
	{
		if( libcreg_value_get_value_data_size(
		     value,
		     &data_size,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve value data size.",
			 function );

			goto on_error;
		}
	}
	if( memory_set(
	     find_data,
	     0,
	     sizeof( WIN32_FIND_DATAW ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear find data.",
		 function );

		goto on_error;
	}
	if( wide_string_copy(
	     find_data->cFileName,
	     sanitized_name,
	     sanitized_name_size ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy filename.",
		 function );

		goto on_error;
	}
	if( sanitized_name_size < (size_t) 14 )
	{
		if( wide_string_copy(
		     find_data->cAlternateFileName,
		     sanitized_name,
		     sanitized_name_size ) == NULL )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy alternate filename.",
			 function );

			goto on_error;
		}
	}
	if( cregmount_dokan_set_find_data(
	     find_data,
	     (size64_t) data_size,
	     file_entry_type,
	     use_mount_time,
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		goto on_error;
	}
	if( fill_find_data(
	     find_data,
	     file_info ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set directory entry.",
		 function );

		goto on_error;
	}
	memory_free(
	 sanitized_name );

	return( 1 );

on_error:
	if( sanitized_name != NULL )
	{
		memory_free(
		 sanitized_name );
	}
	return( -1 );
}

/* Reads a directory
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_FindFiles(
               const wchar_t *path,
               PFillFindData fill_find_data,
               DOKAN_FILE_INFO *file_info )
{
	WIN32_FIND_DATAW find_data;

	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_key_t *sub_key        = NULL;
	libcreg_value_t *value        = NULL;
	wchar_t *name                 = NULL;
	static char *function         = "cregmount_dokan_FindFiles";
	size_t data_size              = 0;
	size_t name_size              = 0;
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	uint64_t value_64bit          = 0;
	int number_of_sub_keys        = 0;
	int number_of_values          = 0;
	int result                    = 0;
	int sub_key_index             = 0;
	int value_index               = 0;
	uint8_t file_entry_type       = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	normalized_path_length = path_length;

	if( ( normalized_path_length >= 1 )
	 && ( path[ normalized_path_length - 1 ] == '\\' ) )
	{
		normalized_path_length -= 1;
	}
	if( ( normalized_path_length >= 9 )
	 && ( path[ normalized_path_length - 9 ] == '\\' )
	 && ( system_string_compare(
	       &( path[ normalized_path_length - 8 ] ),
	       _SYSTEM_STRING( "(values)" ),
	       8 ) == 0 ) )
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
	}
	else
	{
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
	}
	if( mount_handle_get_key_by_path(
	     cregmount_mount_handle,
	     path,
	     path_length,
	     (system_character_t) '\\',
	     &key,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
		 "%s: unable to retrieve key for: %ls.",
		 function,
		 path );

		result = -ERROR_FILE_NOT_FOUND;

		goto on_error;
	}
	if( cregmount_dokan_filldir(
	     fill_find_data,
	     file_info,
	     L".",
	     2,
	     &find_data,
	     key,
	     NULL,
	     CREGMOUNT_FILE_ENTRY_TYPE_KEY,
	     0,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( cregmount_dokan_filldir(
	     fill_find_data,
	     file_info,
	     L"..",
	     3,
	     &find_data,
	     key,
	     NULL,
	     CREGMOUNT_FILE_ENTRY_TYPE_KEY,
	     0,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set find data.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	{
		if( libcreg_key_get_number_of_sub_keys(
		     key,
		     &number_of_sub_keys,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of sub keys.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		for( sub_key_index = 0;
		     sub_key_index < number_of_sub_keys;
		     sub_key_index++ )
		{
			if( libcreg_key_get_sub_key(
			     key,
			     sub_key_index,
			     &sub_key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve sub key: %d.",
				 function,
				 sub_key_index );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( libcreg_key_get_utf16_name_size(
			     sub_key,
			     &name_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the sub key name size.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( name_size > 0 )
			{
				if( name_size > (size_t) ( SSIZE_MAX / sizeof( wchar_t ) ) )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid name size value exceeds maximum.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				name = wide_string_allocate(
					name_size );

				if( name == NULL )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create sub key name.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				if( libcreg_key_get_utf16_name(
				     sub_key,
				     (uint16_t *) name,
				     name_size,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the sub key name.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				if( cregmount_dokan_filldir(
				     fill_find_data,
				     file_info,
				     name,
				     name_size,
				     &find_data,
				     sub_key,
				     NULL,
				     CREGMOUNT_FILE_ENTRY_TYPE_KEY,
				     0,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set find data.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				memory_free(
				 name );

				name = NULL;
			}
			if( libcreg_key_free(
			     &sub_key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free sub key: %d.",
				 function,
				 sub_key_index );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
		if( libcreg_key_get_number_of_values(
		     key,
		     &number_of_values,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		if( number_of_values > 0 )
		{
			/* Make the values accessible
			 */
			if( cregmount_dokan_filldir(
			     fill_find_data,
			     file_info,
			     L"(values)",
			     9,
			     &find_data,
			     key,
			     NULL,
			     CREGMOUNT_FILE_ENTRY_TYPE_VALUES,
			     0,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
				 "%s: unable to set find data.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
	}
	else if( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES )
	{
		if( libcreg_key_get_number_of_values(
		     key,
		     &number_of_values,
		     &error ) != 1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve number of values.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
		for( value_index = 0;
		     value_index < number_of_values;
		     value_index++ )
		{
			if( libcreg_key_get_value(
			     key,
			     value_index,
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to free retrieve value: %d.",
				 function,
				 value_index );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( libcreg_value_get_value_data_size(
			     value,
			     &data_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data size.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( libcreg_value_get_utf16_name_size(
			     value,
			     &name_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve the value size.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( name_size > 0 )
			{
				if( name_size > (size_t) ( SSIZE_MAX / sizeof( wchar_t ) ) )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_VALUE_EXCEEDS_MAXIMUM,
					 "%s: invalid name size value exceeds maximum.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				name = wide_string_allocate(
					name_size );

				if( name == NULL )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_MEMORY,
					 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
					 "%s: unable to create value name.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				if( libcreg_value_get_utf16_name(
				     value,
				     (uint16_t *) name,
				     name_size,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
					 "%s: unable to retrieve the value name.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				if( cregmount_dokan_filldir(
				     fill_find_data,
				     file_info,
				     name,
				     name_size,
				     &find_data,
				     key,
				     value,
				     CREGMOUNT_FILE_ENTRY_TYPE_VALUE,
				     0,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set find data.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
				memory_free(
				 name );

				name = NULL;
			}
			else
			{
				/* Make the default value accessible
				 */
				if( cregmount_dokan_filldir(
				     fill_find_data,
				     file_info,
				     L"(default)",
				     10,
				     &find_data,
				     key,
				     value,
				     CREGMOUNT_FILE_ENTRY_TYPE_VALUE,
				     0,
				     &error ) != 1 )
				{
					libcerror_error_set(
					 &error,
					 LIBCERROR_ERROR_DOMAIN_RUNTIME,
					 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
					 "%s: unable to set find data.",
					 function );

					result = -ERROR_GEN_FAILURE;

					goto on_error;
				}
			}
			if( libcreg_value_free(
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value: %d.",
				 function,
				 value_index );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
	}
	if( libcreg_key_free(
	     &key,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
		 "%s: unable to free key.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( name != NULL )
	{
		memory_free(
		 name );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( sub_key != NULL )
	{
		libcreg_key_free(
		 &sub_key,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Sets the values in a file information structure
 * Returns 1 if successful or -1 on error
 */
int cregmount_dokan_set_file_information(
     BY_HANDLE_FILE_INFORMATION *file_information,
     size64_t size,
     uint8_t file_entry_type,
     uint8_t use_mount_time,
     libcerror_error_t **error )
{
	static char *function = "cregmount_dokan_set_file_information";

	if( file_information == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file information.",
		 function );

		return( -1 );
	}
	if( ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUE )
	 && ( file_entry_type != CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: supported file entry type.",
		 function );

		return( -1 );
	}
/* TODO implement
	if( use_mount_time != 0 )
	{
	}
*/
	if( size > 0 )
	{
		file_information->nFileSizeHigh = (DWORD) ( size >> 32 );
		file_information->nFileSizeLow  = (DWORD) ( size & 0xffffffffUL );
	}
	file_information->dwFileAttributes = FILE_ATTRIBUTE_READONLY;

	if( ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
	 || ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_ROOT )
	 || ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_VALUES ) )
	{
		file_information->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
	}
	return( 1 );
}

/* Retrieves the file information
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_GetFileInformation(
               const wchar_t *path,
               BY_HANDLE_FILE_INFORMATION *file_information,
               DOKAN_FILE_INFO *file_info )
{
	libcerror_error_t *error      = NULL;
	libcreg_key_t *key            = NULL;
	libcreg_value_t *value        = NULL;
	static char *function         = "cregmount_dokan_GetFileInformation";
	size_t data_size              = 0;
	size_t normalized_path_length = 0;
	size_t path_length            = 0;
	int result                    = 0;
	uint8_t file_entry_type       = 0;
	uint8_t use_mount_time        = 0;

	if( path == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid path.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	if( file_info == NULL )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid file info.",
		 function );

		result = -ERROR_BAD_ARGUMENTS;

		goto on_error;
	}
	path_length = wide_string_length(
	               path );

	if( path_length == 1 )
	{
		if( path[ 0 ] != (wchar_t) '\\' )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
			 LIBCERROR_ARGUMENT_ERROR_UNSUPPORTED_VALUE,
			 "%s: unsupported path: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_ROOT;
		use_mount_time  = 1;
	}
	else
	{
		normalized_path_length = path_length;

		if( ( normalized_path_length >= 1 )
		 && ( path[ normalized_path_length - 1 ] == '\\' ) )
		{
			normalized_path_length -= 1;
		}
		if( ( normalized_path_length >= 9 )
		 && ( path[ normalized_path_length - 9 ] == '\\' )
		 && ( system_string_compare(
		       &( path[ normalized_path_length - 8 ] ),
		       _SYSTEM_STRING( "(values)" ),
		       8 ) == 0 ) )
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_VALUES;
		}
		else
		{
			file_entry_type = CREGMOUNT_FILE_ENTRY_TYPE_KEY;
		}
		result = mount_handle_get_key_by_path(
		          cregmount_mount_handle,
		          path,
		          path_length,
		          (system_character_t) '\\',
		          &key,
		          &error );

		if( result == -1 )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
			 "%s: unable to retrieve key for: %ls.",
			 function,
			 path );

			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		else if( ( file_entry_type == CREGMOUNT_FILE_ENTRY_TYPE_KEY )
		      && ( result == 0 ) )
		{
			result = mount_handle_get_value_by_path(
				  cregmount_mount_handle,
				  path,
				  path_length,
			          (system_character_t) '\\',
				  &key,
				  &value,
				  &error );

			if( result == -1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value for: %ls.",
				 function,
				 path );

				result = -ERROR_FILE_NOT_FOUND;

				goto on_error;
			}
		}
		if( result == 0 )
		{
			result = -ERROR_FILE_NOT_FOUND;

			goto on_error;
		}
		if( value != NULL )
		{
			if( libcreg_value_get_value_data_size(
			     value,
			     &data_size,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_GET_FAILED,
				 "%s: unable to retrieve value data size.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
			if( libcreg_value_free(
			     &value,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free value.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
		if( key != NULL )
		{
			if( libcreg_key_free(
			     &key,
			     &error ) != 1 )
			{
				libcerror_error_set(
				 &error,
				 LIBCERROR_ERROR_DOMAIN_RUNTIME,
				 LIBCERROR_RUNTIME_ERROR_FINALIZE_FAILED,
				 "%s: unable to free key.",
				 function );

				result = -ERROR_GEN_FAILURE;

				goto on_error;
			}
		}
		use_mount_time  = 1;
	}
	if( cregmount_dokan_set_file_information(
	     file_information,
	     (size64_t) data_size,
	     file_entry_type,
	     use_mount_time,
	     &error ) != 1 )
	{
		libcerror_error_set(
		 &error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_SET_FAILED,
		 "%s: unable to set file info.",
		 function );

		result = -ERROR_GEN_FAILURE;

		goto on_error;
	}
	return( result );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	if( value != NULL )
	{
		libcreg_value_free(
		 &value,
		 NULL );
	}
	if( key != NULL )
	{
		libcreg_key_free(
		 &key,
		 NULL );
	}
	return( result );
}

/* Retrieves the volume information
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_GetVolumeInformation(
               wchar_t *volume_name,
               DWORD volume_name_size,
               DWORD *volume_serial_number,
               DWORD *maximum_filename_length,
               DWORD *file_system_flags,
               wchar_t *file_system_name,
               DWORD file_system_name_size,
               DOKAN_FILE_INFO *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	libcerror_error_t *error = NULL;
	static char *function    = "cregmount_dokan_GetVolumeInformation";
	int result               = 0;

	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	if( ( volume_name != NULL )
	 && ( volume_name_size > (DWORD) ( sizeof( wchar_t ) * 4 ) ) )
	{
		/* Using wcsncpy seems to cause strange behavior here
		 */
		if( memory_copy(
		     volume_name,
		     L"CREG",
		     sizeof( wchar_t ) * 4 ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy volume name.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	if( volume_serial_number != NULL )
	{
		/* If this value contains 0 it can crash the system is this an issue in Dokan?
		 */
		*volume_serial_number = 0x19831116;
	}
	if( maximum_filename_length != NULL )
	{
		*maximum_filename_length = 256;
	}
	if( file_system_flags != NULL )
	{
		*file_system_flags = FILE_CASE_SENSITIVE_SEARCH
		                   | FILE_CASE_PRESERVED_NAMES
		                   | FILE_UNICODE_ON_DISK
		                   | FILE_READ_ONLY_VOLUME;
	}
	if( ( file_system_name != NULL )
	 && ( file_system_name_size > (DWORD) ( sizeof( wchar_t ) * 6 ) ) )
	{
		/* Using wcsncpy seems to cause strange behavior here
		 */
		if( memory_copy(
		     file_system_name,
		     L"Dokan",
		     sizeof( wchar_t ) * 6 ) == NULL )
		{
			libcerror_error_set(
			 &error,
			 LIBCERROR_ERROR_DOMAIN_MEMORY,
			 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
			 "%s: unable to copy file system name.",
			 function );

			result = -ERROR_GEN_FAILURE;

			goto on_error;
		}
	}
	return( 0 );

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
	return( result );
}

/* Unmount the image
 * Returns 0 if successful or a negative error code otherwise
 */
int __stdcall cregmount_dokan_Unmount(
               DOKAN_FILE_INFO *file_info CREGTOOLS_ATTRIBUTE_UNUSED )
{
	static char *function = "cregmount_dokan_Unmount";

	CREGTOOLS_UNREFERENCED_PARAMETER( file_info )

	return( 0 );
}

#endif

/* The main program
 */
#if defined( HAVE_WIDE_SYSTEM_CHARACTER )
int wmain( int argc, wchar_t * const argv[] )
#else
int main( int argc, char * const argv[] )
#endif
{
	libcreg_error_t *error                      = NULL;
	system_character_t *option_ascii_codepage   = NULL;
	system_character_t *option_extended_options = NULL;
	system_character_t *mount_point             = NULL;
	system_character_t *source                  = NULL;
	char *program                               = "cregmount";
	system_integer_t option                     = 0;
	int result                                  = 0;
	int verbose                                 = 0;

#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	struct fuse_operations cregmount_fuse_operations;

	struct fuse_args cregmount_fuse_arguments   = FUSE_ARGS_INIT(0, NULL);
	struct fuse_chan *cregmount_fuse_channel    = NULL;
	struct fuse *cregmount_fuse_handle          = NULL;

#elif defined( HAVE_LIBDOKAN )
	DOKAN_OPERATIONS cregmount_dokan_operations;
	DOKAN_OPTIONS cregmount_dokan_options;
#endif

	libcnotify_stream_set(
	 stderr,
	 NULL );
	libcnotify_verbose_set(
	 1 );

	if( libclocale_initialize(
	     "cregtools",
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize locale values.\n" );

		goto on_error;
	}
        if( cregtools_output_initialize(
             _IONBF,
             &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize output settings.\n" );

		goto on_error;
	}
	cregoutput_version_fprint(
	 stdout,
	 program );

	while( ( option = cregtools_getopt(
	                   argc,
	                   argv,
	                   _SYSTEM_STRING( "c:hvVX:" ) ) ) != (system_integer_t) -1 )
	{
		switch( option )
		{
			case (system_integer_t) '?':
			default:
				fprintf(
				 stderr,
				 "Invalid argument: %" PRIs_SYSTEM "\n",
				 argv[ optind - 1 ] );

				usage_fprint(
				 stdout );

				return( EXIT_FAILURE );

			case (system_integer_t) 'c':
				option_ascii_codepage = optarg;

				break;

			case (system_integer_t) 'h':
				usage_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'v':
				verbose = 1;

				break;

			case (system_integer_t) 'V':
				cregoutput_copyright_fprint(
				 stdout );

				return( EXIT_SUCCESS );

			case (system_integer_t) 'X':
				option_extended_options = optarg;

				break;
		}
	}
	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing source file.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	source = argv[ optind++ ];

	if( optind == argc )
	{
		fprintf(
		 stderr,
		 "Missing mount point.\n" );

		usage_fprint(
		 stdout );

		return( EXIT_FAILURE );
	}
	mount_point = argv[ optind ];

	libcnotify_verbose_set(
	 verbose );
	libcreg_notify_set_stream(
	 stderr,
	 NULL );
	libcreg_notify_set_verbose(
	 verbose );

	if( mount_handle_initialize(
	     &cregmount_mount_handle,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to initialize mount handle.\n" );

		goto on_error;
	}
	if( option_ascii_codepage != NULL )
	{
		result = mount_handle_set_ascii_codepage(
		          cregmount_mount_handle,
		          option_ascii_codepage,
		          &error );

		if( result == -1 )
		{
			fprintf(
			 stderr,
			 "Unable to set ASCII codepage in mount handle.\n" );

			goto on_error;
		}
		else if( result == 0 )
		{
			fprintf(
			 stderr,
			 "Unsupported ASCII codepage defaulting to: windows-1252.\n" );
		}
	}
	if( mount_handle_open_input(
	     cregmount_mount_handle,
	     source,
	     &error ) != 1 )
	{
		fprintf(
		 stderr,
		 "Unable to open CREG file.\n" );

		goto on_error;
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( memory_set(
	     &cregmount_fuse_operations,
	     0,
	     sizeof( struct fuse_operations ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear fuse operations.\n" );

		goto on_error;
	}
	if( option_extended_options != NULL )
	{
		/* This argument is required but ignored
		 */
		if( fuse_opt_add_arg(
		     &cregmount_fuse_arguments,
		     "" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &cregmount_fuse_arguments,
		     "-o" ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
		if( fuse_opt_add_arg(
		     &cregmount_fuse_arguments,
		     option_extended_options ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable add fuse arguments.\n" );

			goto on_error;
		}
	}
	cregmount_fuse_operations.open    = &cregmount_fuse_open;
	cregmount_fuse_operations.read    = &cregmount_fuse_read;
	cregmount_fuse_operations.readdir = &cregmount_fuse_readdir;
	cregmount_fuse_operations.getattr = &cregmount_fuse_getattr;
	cregmount_fuse_operations.destroy = &cregmount_fuse_destroy;

	cregmount_fuse_channel = fuse_mount(
	                          mount_point,
	                          &cregmount_fuse_arguments );

	if( cregmount_fuse_channel == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse channel.\n" );

		goto on_error;
	}
	cregmount_fuse_handle = fuse_new(
	                         cregmount_fuse_channel,
	                         &cregmount_fuse_arguments,
	                         &cregmount_fuse_operations,
	                         sizeof( struct fuse_operations ),
	                         cregmount_mount_handle );
	
	if( cregmount_fuse_handle == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to create fuse handle.\n" );

		goto on_error;
	}
	if( verbose == 0 )
	{
		if( fuse_daemonize(
		     0 ) != 0 )
		{
			fprintf(
			 stderr,
			 "Unable to daemonize fuse.\n" );

			goto on_error;
		}
	}
	result = fuse_loop(
	          cregmount_fuse_handle );

	if( result != 0 )
	{
		fprintf(
		 stderr,
		 "Unable to run fuse loop.\n" );

		goto on_error;
	}
	fuse_destroy(
	 cregmount_fuse_handle );

	fuse_opt_free_args(
	 &cregmount_fuse_arguments );

	return( EXIT_SUCCESS );

#elif defined( HAVE_LIBDOKAN )
	if( memory_set(
	     &cregmount_dokan_operations,
	     0,
	     sizeof( DOKAN_OPERATIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan operations.\n" );

		goto on_error;
	}
	if( memory_set(
	     &cregmount_dokan_options,
	     0,
	     sizeof( DOKAN_OPTIONS ) ) == NULL )
	{
		fprintf(
		 stderr,
		 "Unable to clear dokan options.\n" );

		goto on_error;
	}
	cregmount_dokan_options.Version     = 600;
	cregmount_dokan_options.ThreadCount = 0;
	cregmount_dokan_options.MountPoint  = mount_point;

	if( verbose != 0 )
	{
		cregmount_dokan_options.Options |= DOKAN_OPTION_STDERR;
#if defined( HAVE_DEBUG_OUTPUT )
		cregmount_dokan_options.Options |= DOKAN_OPTION_DEBUG;
#endif
	}
/* This will only affect the drive properties
	cregmount_dokan_options.Options |= DOKAN_OPTION_REMOVABLE;
*/
	cregmount_dokan_options.Options |= DOKAN_OPTION_KEEP_ALIVE;

	cregmount_dokan_operations.CreateFile           = &cregmount_dokan_CreateFile;
	cregmount_dokan_operations.OpenDirectory        = &cregmount_dokan_OpenDirectory;
	cregmount_dokan_operations.CreateDirectory      = NULL;
	cregmount_dokan_operations.Cleanup              = NULL;
	cregmount_dokan_operations.CloseFile            = &cregmount_dokan_CloseFile;
	cregmount_dokan_operations.ReadFile             = &cregmount_dokan_ReadFile;
	cregmount_dokan_operations.WriteFile            = NULL;
	cregmount_dokan_operations.FlushFileBuffers     = NULL;
	cregmount_dokan_operations.GetFileInformation   = &cregmount_dokan_GetFileInformation;
	cregmount_dokan_operations.FindFiles            = &cregmount_dokan_FindFiles;
	cregmount_dokan_operations.FindFilesWithPattern = NULL;
	cregmount_dokan_operations.SetFileAttributes    = NULL;
	cregmount_dokan_operations.SetFileTime          = NULL;
	cregmount_dokan_operations.DeleteFile           = NULL;
	cregmount_dokan_operations.DeleteDirectory      = NULL;
	cregmount_dokan_operations.MoveFile             = NULL;
	cregmount_dokan_operations.SetEndOfFile         = NULL;
	cregmount_dokan_operations.SetAllocationSize    = NULL;
	cregmount_dokan_operations.LockFile             = NULL;
	cregmount_dokan_operations.UnlockFile           = NULL;
	cregmount_dokan_operations.GetFileSecurity      = NULL;
	cregmount_dokan_operations.SetFileSecurity      = NULL;
	cregmount_dokan_operations.GetDiskFreeSpace     = NULL;
	cregmount_dokan_operations.GetVolumeInformation = &cregmount_dokan_GetVolumeInformation;
	cregmount_dokan_operations.Unmount              = &cregmount_dokan_Unmount;

	result = DokanMain(
	          &cregmount_dokan_options,
	          &cregmount_dokan_operations );

	switch( result )
	{
		case DOKAN_SUCCESS:
			break;

		case DOKAN_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: generic error\n" );
			break;

		case DOKAN_DRIVE_LETTER_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: bad drive letter\n" );
			break;

		case DOKAN_DRIVER_INSTALL_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to load driver\n" );
			break;

		case DOKAN_START_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: driver error\n" );
			break;

		case DOKAN_MOUNT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unable to assign drive letter\n" );
			break;

		case DOKAN_MOUNT_POINT_ERROR:
			fprintf(
			 stderr,
			 "Unable to run dokan main: mount point error\n" );
			break;

		default:
			fprintf(
			 stderr,
			 "Unable to run dokan main: unknown error: %d\n",
			 result );
			break;
	}
	return( EXIT_SUCCESS );

#else
	fprintf(
	 stderr,
	 "No sub system to mount CREG format.\n" );

	return( EXIT_FAILURE );
#endif

on_error:
	if( error != NULL )
	{
		libcnotify_print_error_backtrace(
		 error );
		libcerror_error_free(
		 &error );
	}
#if defined( HAVE_LIBFUSE ) || defined( HAVE_LIBOSXFUSE )
	if( cregmount_fuse_handle != NULL )
	{
		fuse_destroy(
		 cregmount_fuse_handle );
	}
	fuse_opt_free_args(
	 &cregmount_fuse_arguments );
#endif
	if( cregmount_mount_handle != NULL )
	{
		mount_handle_free(
		 &cregmount_mount_handle,
		 NULL );
	}
	return( EXIT_FAILURE );
}

