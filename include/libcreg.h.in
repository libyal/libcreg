/*
 * Library to access the Windows 9x/Me Registry File (CREG) format
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

#if !defined( _LIBCREG_H )
#define _LIBCREG_H

#include <libcreg/codepage.h>
#include <libcreg/definitions.h>
#include <libcreg/error.h>
#include <libcreg/extern.h>
#include <libcreg/features.h>
#include <libcreg/types.h>

#include <stdio.h>

#if defined( LIBCREG_HAVE_BFIO )
#include <libbfio.h>
#endif

#if defined( __cplusplus )
extern "C" {
#endif

/* -------------------------------------------------------------------------
 * Support functions
 * ------------------------------------------------------------------------- */

/* Returns the library version
 */
LIBCREG_EXTERN \
const char *libcreg_get_version(
             void );

/* Returns the access flags for reading
 */
LIBCREG_EXTERN \
int libcreg_get_access_flags_read(
     void );

/* Retrieves the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_get_codepage(
     int *codepage,
     libcreg_error_t **error );

/* Sets the narrow system string codepage
 * A value of 0 represents no codepage, UTF-8 encoding is used instead
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_set_codepage(
     int codepage,
     libcreg_error_t **error );

/* Determines if a file contains a CREG file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_check_file_signature(
     const char *filename,
     libcreg_error_t **error );

#if defined( LIBCREG_HAVE_WIDE_CHARACTER_TYPE )

/* Determines if a file contains a CREG file signature
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_check_file_signature_wide(
     const wchar_t *filename,
     libcreg_error_t **error );

#endif /* defined( LIBCREG_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBCREG_HAVE_BFIO )

/* Determines if a file contains a CREG file signature using a Basic File IO (bfio) handle
 * Returns 1 if true, 0 if not or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_check_file_signature_file_io_handle(
     libbfio_handle_t *bfio_handle,
     libcreg_error_t **error );

#endif /* defined( LIBCREG_HAVE_BFIO ) */

/* -------------------------------------------------------------------------
 * Notify functions
 * ------------------------------------------------------------------------- */

/* Sets the verbose notification
 */
LIBCREG_EXTERN \
void libcreg_notify_set_verbose(
      int verbose );

/* Sets the notification stream
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_notify_set_stream(
     FILE *stream,
     libcreg_error_t **error );

/* Opens the notification stream using a filename
 * The stream is opened in append mode
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_notify_stream_open(
     const char *filename,
     libcreg_error_t **error );

/* Closes the notification stream if opened using a filename
 * Returns 0 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_notify_stream_close(
     libcreg_error_t **error );

/* -------------------------------------------------------------------------
 * Error functions
 * ------------------------------------------------------------------------- */

/* Frees an error
 */
LIBCREG_EXTERN \
void libcreg_error_free(
      libcreg_error_t **error );

/* Prints a descriptive string of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_error_fprint(
     libcreg_error_t *error,
     FILE *stream );

/* Prints a descriptive string of the error to the string
 * Returns the number of printed characters if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_error_sprint(
     libcreg_error_t *error,
     char *string,
     size_t size );

/* Prints a backtrace of the error to the stream
 * Returns the number of printed characters if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_error_backtrace_fprint(
     libcreg_error_t *error,
     FILE *stream );

/* Prints a backtrace of the error to the string
 * Returns the number of printed characters if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_error_backtrace_sprint(
     libcreg_error_t *error,
     char *string,
     size_t size );

/* -------------------------------------------------------------------------
 * File functions
 * ------------------------------------------------------------------------- */

/* Creates a file
 * Make sure the value file is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_initialize(
     libcreg_file_t **file,
     libcreg_error_t **error );

/* Frees a file
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_free(
     libcreg_file_t **file,
     libcreg_error_t **error );

/* Signals the file to abort its current activity
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_signal_abort(
     libcreg_file_t *file,
     libcreg_error_t **error );

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_open(
     libcreg_file_t *file,
     const char *filename,
     int access_flags,
     libcreg_error_t **error );

#if defined( LIBCREG_HAVE_WIDE_CHARACTER_TYPE )

/* Opens a file
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_open_wide(
     libcreg_file_t *file,
     const wchar_t *filename,
     int access_flags,
     libcreg_error_t **error );

#endif /* defined( LIBCREG_HAVE_WIDE_CHARACTER_TYPE ) */

#if defined( LIBCREG_HAVE_BFIO )

/* Opens a file using a Basic File IO (bfio) handle
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_open_file_io_handle(
     libcreg_file_t *file,
     libbfio_handle_t *file_io_handle,
     int access_flags,
     libcreg_error_t **error );

#endif /* defined( LIBCREG_HAVE_BFIO ) */

/* Closes a file
 * Returns 0 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_close(
     libcreg_file_t *file,
     libcreg_error_t **error );

/* Determine if the file corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_is_corrupted(
     libcreg_file_t *file,
     libcreg_error_t **error );

/* Retrieves the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_get_ascii_codepage(
     libcreg_file_t *file,
     int *ascii_codepage,
     libcreg_error_t **error );

/* Sets the file ASCII codepage
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_set_ascii_codepage(
     libcreg_file_t *file,
     int ascii_codepage,
     libcreg_error_t **error );

/* Retrieves the format version
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_get_format_version(
     libcreg_file_t *file,
     uint16_t *major_version,
     uint16_t *minor_version,
     libcreg_error_t **error );

/* Retrieves the file type
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_get_type(
     libcreg_file_t *file,
     uint32_t *file_type,
     libcreg_error_t **error );

/* Retrieves the root key
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_get_root_key(
     libcreg_file_t *file,
     libcreg_key_t **root_key,
     libcreg_error_t **error );

/* Retrieves the key for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_get_key_by_utf8_path(
     libcreg_file_t *file,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **key,
     libcreg_error_t **error );

/* Retrieves the key for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_file_get_key_by_utf16_path(
     libcreg_file_t *file,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **key,
     libcreg_error_t **error );

/* -------------------------------------------------------------------------
 * Key functions
 * ------------------------------------------------------------------------- */

/* Frees a key
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_free(
     libcreg_key_t **key,
     libcreg_error_t **error );

/* Determine if the key corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_is_corrupted(
     libcreg_key_t *key,
     libcreg_error_t **error );

/* Retrieves the offset of the key
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_offset(
     libcreg_key_t *key,
     off64_t *offset,
     libcreg_error_t **error );

/* Retrieves the key name size
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_name_size(
     libcreg_key_t *key,
     size_t *string_size,
     libcreg_error_t **error );

/* Retrieves the key name
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_name(
     libcreg_key_t *key,
     uint8_t *string,
     size_t string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-8 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_utf8_name_size(
     libcreg_key_t *key,
     size_t *utf8_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-8 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_utf8_name(
     libcreg_key_t *key,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-16 string size of the key name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_utf16_name_size(
     libcreg_key_t *key,
     size_t *utf16_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-16 string value of the key name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_utf16_name(
     libcreg_key_t *key,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcreg_error_t **error );

/* Retrieves the number of values
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_number_of_values(
     libcreg_key_t *key,
     int *number_of_values,
     libcreg_error_t **error );

/* Retrieves a specific value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_value_by_index(
     libcreg_key_t *key,
     int value_index,
     libcreg_value_t **value,
     libcreg_error_t **error );

/* Retrieves the value for the specific UTF-8 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_value_by_utf8_name(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_value_t **value,
     libcreg_error_t **error );

/* Retrieves the value for the specific UTF-16 encoded name
 * To retrieve the default value specify value name as NULL and its length as 0
 * Returns 1 if successful, 0 if no such value or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_value_by_utf16_name(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_value_t **value,
     libcreg_error_t **error );

/* Retrieves the number of sub keys
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_number_of_sub_keys(
     libcreg_key_t *key,
     int *number_of_sub_keys,
     libcreg_error_t **error );

/* Retrieves the sub key for the specific index
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_index(
     libcreg_key_t *key,
     int sub_key_index,
     libcreg_key_t **sub_key,
     libcreg_error_t **error );

/* Retrieves the sub key for the specific UTF-8 encoded name
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf8_name(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **sub_key,
     libcreg_error_t **error );

/* Retrieves the sub key for the specific UTF-8 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf8_path(
     libcreg_key_t *key,
     const uint8_t *utf8_string,
     size_t utf8_string_length,
     libcreg_key_t **sub_key,
     libcreg_error_t **error );

/* Retrieves the sub key for the specific UTF-16 encoded name
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf16_name(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **sub_key,
     libcreg_error_t **error );

/* Retrieves the sub key for the specific UTF-16 encoded path
 * The path separator is the \ character
 * Returns 1 if successful, 0 if no such sub key or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_key_get_sub_key_by_utf16_path(
     libcreg_key_t *key,
     const uint16_t *utf16_string,
     size_t utf16_string_length,
     libcreg_key_t **sub_key,
     libcreg_error_t **error );

/* -------------------------------------------------------------------------
 * Value functions
 * ------------------------------------------------------------------------- */

/* Frees a value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_free(
     libcreg_value_t **value,
     libcreg_error_t **error );

/* Determine if the value corrupted
 * Returns 1 if corrupted, 0 if not or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_is_corrupted(
     libcreg_value_t *value,
     libcreg_error_t **error );

/* Retrieves the offset of the value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_offset(
     libcreg_value_t *value,
     off64_t *offset,
     libcreg_error_t **error );

/* Retrieves the value name size
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_name_size(
     libcreg_value_t *value,
     size_t *string_size,
     libcreg_error_t **error );

/* Retrieves the value name
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_name(
     libcreg_value_t *value,
     uint8_t *string,
     size_t string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-8 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_utf8_name_size(
     libcreg_value_t *value,
     size_t *utf8_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-8 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_utf8_name(
     libcreg_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-16 string size of the value name
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_utf16_name_size(
     libcreg_value_t *value,
     size_t *utf16_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-16 string value of the value name
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_utf16_name(
     libcreg_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcreg_error_t **error );

/* Retrieves the value type
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_type(
     libcreg_value_t *value,
     uint32_t *value_type,
     libcreg_error_t **error );

/* Retrieves the value data size
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_data_size(
     libcreg_value_t *value,
     size_t *value_data_size,
     libcreg_error_t **error );

/* Retrieves the value data
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_data(
     libcreg_value_t *value,
     uint8_t *value_data,
     size_t value_data_size,
     libcreg_error_t **error );

/* Retrieves the 32-bit value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_32bit(
     libcreg_value_t *value,
     uint32_t *value_32bit,
     libcreg_error_t **error );

/* Retrieves the 64-bit value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_64bit(
     libcreg_value_t *value,
     uint64_t *value_64bit,
     libcreg_error_t **error );

/* Retrieves the UTF-8 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_utf8_string_size(
     libcreg_value_t *value,
     size_t *utf8_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-8 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_utf8_string(
     libcreg_value_t *value,
     uint8_t *utf8_string,
     size_t utf8_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-16 string size
 * The returned size includes the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_utf16_string_size(
     libcreg_value_t *value,
     size_t *utf16_string_size,
     libcreg_error_t **error );

/* Retrieves the UTF-16 string value
 * The function uses a codepage if necessary, it uses the codepage set for the library
 * The size should include the end of string character
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_utf16_string(
     libcreg_value_t *value,
     uint16_t *utf16_string,
     size_t utf16_string_size,
     libcreg_error_t **error );

/* Retrieves the size of a binary data value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_binary_data_size(
     libcreg_value_t *value,
     size_t *size,
     libcreg_error_t **error );

/* Retrieves the binary data value
 * Returns 1 if successful or -1 on error
 */
LIBCREG_EXTERN \
int libcreg_value_get_value_binary_data(
     libcreg_value_t *value,
     uint8_t *binary_data,
     size_t size,
     libcreg_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_H ) */

