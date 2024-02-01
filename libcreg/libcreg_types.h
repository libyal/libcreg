/*
 * The internal type definitions
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

#if !defined( _LIBCREG_INTERNAL_TYPES_H )
#define _LIBCREG_INTERNAL_TYPES_H

#include <common.h>
#include <types.h>

/* Define HAVE_LOCAL_LIBCREG for local use of libcreg
 * The definitions in <libcreg/types.h> are copied here
 * for local use of libcreg
 */
#if defined( HAVE_LOCAL_LIBCREG )

/* The following type definitions hide internal data structures
 */
#if defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI )
typedef struct libcreg_file {}	libcreg_file_t;
typedef struct libcreg_key {}	libcreg_key_t;
typedef struct libcreg_value {}	libcreg_value_t;

#else
typedef intptr_t libcreg_file_t;
typedef intptr_t libcreg_key_t;
typedef intptr_t libcreg_value_t;

#endif /* defined( HAVE_DEBUG_OUTPUT ) && !defined( WINAPI ) */

#endif /* defined( HAVE_LOCAL_LIBCREG ) */

#endif /* !defined( _LIBCREG_INTERNAL_TYPES_H ) */

