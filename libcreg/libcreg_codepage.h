/*
 * Codepage functions
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

#if !defined( _LIBCREG_INTERNAL_CODEPAGE_H )
#define _LIBCREG_INTERNAL_CODEPAGE_H

#include <common.h>
#include <types.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* Define HAVE_LOCAL_LIBCREG for local use of libcreg
 * The definitions in <libcreg/codepage.h> are copied here
 * for local use of libcreg
 */
#if !defined( HAVE_LOCAL_LIBCREG )

#include <libcreg/codepage.h>

#else

/* The codepage definitions
 */
enum LIBCREG_CODEPAGES
{
	LIBCREG_CODEPAGE_ASCII		= 20127,

	LIBCREG_CODEPAGE_ISO_8859_1	= 28591,
	LIBCREG_CODEPAGE_ISO_8859_2	= 28592,
	LIBCREG_CODEPAGE_ISO_8859_3	= 28593,
	LIBCREG_CODEPAGE_ISO_8859_4	= 28594,
	LIBCREG_CODEPAGE_ISO_8859_5	= 28595,
	LIBCREG_CODEPAGE_ISO_8859_6	= 28596,
	LIBCREG_CODEPAGE_ISO_8859_7	= 28597,
	LIBCREG_CODEPAGE_ISO_8859_8	= 28598,
	LIBCREG_CODEPAGE_ISO_8859_9	= 28599,
	LIBCREG_CODEPAGE_ISO_8859_10	= 28600,
	LIBCREG_CODEPAGE_ISO_8859_11	= 28601,
	LIBCREG_CODEPAGE_ISO_8859_13	= 28603,
	LIBCREG_CODEPAGE_ISO_8859_14	= 28604,
	LIBCREG_CODEPAGE_ISO_8859_15	= 28605,
	LIBCREG_CODEPAGE_ISO_8859_16	= 28606,

	LIBCREG_CODEPAGE_KOI8_R		= 20866,
	LIBCREG_CODEPAGE_KOI8_U		= 21866,

	LIBCREG_CODEPAGE_WINDOWS_874	= 874,
	LIBCREG_CODEPAGE_WINDOWS_932	= 932,
	LIBCREG_CODEPAGE_WINDOWS_936	= 936,
	LIBCREG_CODEPAGE_WINDOWS_949	= 949,
	LIBCREG_CODEPAGE_WINDOWS_950	= 950,
	LIBCREG_CODEPAGE_WINDOWS_1250	= 1250,
	LIBCREG_CODEPAGE_WINDOWS_1251	= 1251,
	LIBCREG_CODEPAGE_WINDOWS_1252	= 1252,
	LIBCREG_CODEPAGE_WINDOWS_1253	= 1253,
	LIBCREG_CODEPAGE_WINDOWS_1254	= 1254,
	LIBCREG_CODEPAGE_WINDOWS_1255	= 1255,
	LIBCREG_CODEPAGE_WINDOWS_1256	= 1256,
	LIBCREG_CODEPAGE_WINDOWS_1257	= 1257,
	LIBCREG_CODEPAGE_WINDOWS_1258	= 1258
};

#endif /* !defined( HAVE_LOCAL_LIBCREG ) */

#if defined( __cplusplus )
}
#endif

#endif /* !defined( _LIBCREG_INTERNAL_CODEPAGE_H ) */

