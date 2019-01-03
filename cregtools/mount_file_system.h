/*
 * Mount file system
 *
 * Copyright (C) 2009-2019, Joachim Metz <joachim.metz@gmail.com>
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

#if !defined( _MOUNT_FILE_SYSTEM_H )
#define _MOUNT_FILE_SYSTEM_H

#include <common.h>
#include <types.h>

#include "cregtools_libcerror.h"
#include "cregtools_libcreg.h"

#if defined( __cplusplus )
extern "C" {
#endif

typedef struct mount_file_system mount_file_system_t;

struct mount_file_system
{
	/* The mounted timestamp
	 */
	uint64_t mounted_timestamp;

	/* The file
	 */
	libcreg_file_t *creg_file;
};

int mount_file_system_initialize(
     mount_file_system_t **file_system,
     libcerror_error_t **error );

int mount_file_system_free(
     mount_file_system_t **file_system,
     libcerror_error_t **error );

int mount_file_system_signal_abort(
     mount_file_system_t *file_system,
     libcerror_error_t **error );

int mount_file_system_set_file(
     mount_file_system_t *file_system,
     libcreg_file_t *creg_file,
     libcerror_error_t **error );

int mount_file_system_get_file(
     mount_file_system_t *file_system,
     libcreg_file_t **creg_file,
     libcerror_error_t **error );

int mount_file_system_get_mounted_timestamp(
     mount_file_system_t *file_system,
     uint64_t *mounted_timestamp,
     libcerror_error_t **error );

int mount_file_system_get_key_path_from_path(
     mount_file_system_t *file_system,
     const system_character_t *path,
     size_t path_length,
     system_character_t **key_path,
     size_t *key_path_size,
     libcerror_error_t **error );

int mount_file_system_get_key_by_path(
     mount_file_system_t *file_system,
     const system_character_t *path,
     size_t path_length,
     libcreg_key_t **creg_key,
     libcerror_error_t **error );

int mount_file_system_get_value_name_from_filename(
     mount_file_system_t *file_system,
     const system_character_t *filename,
     size_t filename_length,
     system_character_t **value_name,
     size_t *value_name_size,
     libcerror_error_t **error );

int mount_file_system_get_value_by_filename(
     mount_file_system_t *file_system,
     libcreg_key_t *creg_key,
     const system_character_t *filename,
     size_t filename_length,
     libcreg_value_t **creg_value,
     libcerror_error_t **error );

int mount_file_system_get_filename_from_name(
     mount_file_system_t *file_system,
     const system_character_t *name,
     size_t name_length,
     system_character_t **filename,
     size_t *filename_size,
     libcerror_error_t **error );

int mount_file_system_get_filename_from_key(
     mount_file_system_t *file_system,
     libcreg_key_t *creg_key,
     system_character_t **filename,
     size_t *filename_size,
     libcerror_error_t **error );

int mount_file_system_get_filename_from_value(
     mount_file_system_t *file_system,
     libcreg_value_t *creg_value,
     system_character_t **filename,
     size_t *filename_size,
     libcerror_error_t **error );

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _MOUNT_FILE_SYSTEM_H ) */

