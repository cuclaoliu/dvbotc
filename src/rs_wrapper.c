/***************************************************************************
 *   Copyright (C) 2007 by Damien Phillip Morrissey,,,   *
 *   dpm95@uow.edu.au   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<rs.h>
#include "dvbotc.h"
#include "rs_wrapper.h"
#include "dump.h"
//! Wrapper function for the Reed Solomon encoder
/**
 * This function wraps the functionality of the reed-solomon encoder by Phil Karn. In order for this function (as well as the main function) to compile the reed-solomon encoder library must be compiled and the library file copied to the source directory.
 * @param data_in Pointer to the array of data to be encoded
 * @param rs Pointer to the RS configuration
 * @return Pointer to the RS encoded data
 */
unsigned char* rs_encode_wrapper(unsigned char* data_in, void* rs)
{
	unsigned char* data_out;	// Output data pointer
	unsigned char* tmp;		// Processing pointer

	// Pad input to RS code function as per standard
	MALLOC(tmp, 239);
	memset(tmp, 0, 239);
	memcpy(tmp+51, data_in, 188);

	// Release input memory
	free(data_in);

	// Prepare correct memory space for the result
	MALLOC(data_out, 204);	// No memset required except for the end part (the parity)
	memset(data_out+188, 0, 16);
	memcpy(data_out, tmp+51, 188);

	// Perform encoding, place result directly in the output
	encode_rs_char(rs, tmp, data_out+188);

	// Release temporary variable
	free(tmp);

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log(dump_refs.rs_encode, dump_refs.rs_encode_txt, data_out, 204);
	}

	return data_out;
}
