/*
 * Data type functions
 *
 * Copyright (C) 2013-2023, Joachim Metz <joachim.metz@gmail.com>
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

#include "libcreg_data_type.h"

#if defined( HAVE_DEBUG_OUTPUT )

libcreg_data_type_t libcreg_data_types[ ] = {
	{ 0x0000, "REG_NONE",				"Undefined" },
	{ 0x0001, "REG_SZ",				"String" },
	{ 0x0002, "REG_EXPAND_SZ",			"String with expandable variables" },
	{ 0x0003, "REG_BINARY",				"Binary data" },
	{ 0x0004, "REG_DWORD_LITTLE_ENDIAN",		"Integer 32-bit signed little-endian" },
	{ 0x0005, "REG_DWORD_BIG_ENDIAN",		"Integer 32-bit signed big-endian" },
	{ 0x0006, "REG_LINK",				"Link" },
	{ 0x0007, "REG_MULTI_SZ",			"Array of strings" },
	{ 0x0008, "REG_RESOURCE_LIST",			"Resource list" },
	{ 0x0009, "REG_FULL_RESOURCE_DESCRIPTOR",	"Full resource descriptor" },
	{ 0x000a, "REG_RESOURCE_REQUIREMENTS_LIST",	"Resource requirements list" },
	{ 0x000b, "REG_QWORD_LITTLE_ENDIAN",		"Integer 64-bit signed little-endian" },

	{ (uint32_t) -1, "_UNKNOWN_", "Unknown" } };

/* Retrieves a string containing the data_type identifier
 */
const char *libcreg_data_type_get_identifier(
             uint32_t data_type )
{
	int iterator = 0;

	while( ( libcreg_data_types[ iterator ] ).data_type != (uint32_t) -1 )
	{
		if( ( libcreg_data_types[ iterator ] ).data_type == data_type )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libcreg_data_types[ iterator ] ).identifier );
}

/* Retrieves a string containing the data_type description
 */
const char *libcreg_data_type_get_description(
             uint32_t data_type )
{
	int iterator = 0;

	while( ( libcreg_data_types[ iterator ] ).data_type != (uint32_t) -1 )
	{
		if( ( libcreg_data_types[ iterator ] ).data_type == data_type )
		{
			break;
		}
		iterator++;
	}
	return(
	 ( libcreg_data_types[ iterator ] ).description );
}

#endif

