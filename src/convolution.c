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
#include "dvbotc.h"
#include "convolution.h"
#include "dump.h"
//!Convolutional encoding function
/**
 * This function carries out the mathematical process of convolutional encoding at a bit level. This is based directly on the process employed by opendvb-0.4 and may be able to be improved. The function makes use of a convolutional code it is passed by reference and modifies each iteration. The initialisation function must be called prior to execution of this function. It may be possible to define the convolutional code in the header file for this module and, hence, remove the requirement to keep passing the pointer around, however this may make the module less flexible.
 * @param data Pointer to the array of bits to be convolutionally encoded (one bit in each byte for simplicity)
 * @param conv_code Pointer to the current convolutional code (result from previous executions)
 * @return Structure containing pointers to the convolutionally encoded data in an x-y map
 */
struct xymap conv_encode(unsigned char* data, unsigned char* conv_code)
{
	struct xymap map_out;		// Output map structure
	int ii,kk;			// Iterator variables
	unsigned char tmp[CONV_M-1];	// Temporary variable
	unsigned char data_in[DVBT_SETTINGS.DS_SL_BITS];
	
	for(ii = 0; ii<DVBT_SETTINGS.DS_SL_BITS/8; ii++)
	{
		data_in[ii*8+7] = (data[ii] & 1)?1:0;
		data_in[ii*8+6] = (data[ii] & 2)?1:0;
		data_in[ii*8+5] = (data[ii] & 4)?1:0;
		data_in[ii*8+4] = (data[ii] & 8)?1:0;

		data_in[ii*8+3] = (data[ii] & 16)?1:0;
		data_in[ii*8+2] = (data[ii] & 32)?1:0;
		data_in[ii*8+1] = (data[ii] & 64)?1:0;
		data_in[ii*8+0] = (data[ii] & 128)?1:0;
	}

	// Assign memory to the output map
	MALLOC(map_out.x, DVBT_SETTINGS.DS_SL_BITS);
	memset(map_out.x, 0, DVBT_SETTINGS.DS_SL_BITS);
	MALLOC(map_out.y, DVBT_SETTINGS.DS_SL_BITS);
	memset(map_out.y, 0, DVBT_SETTINGS.DS_SL_BITS);

	// Using the simple convolutional process from convolutional_encode.m, lines 64-82
	for(ii=0; ii<DVBT_SETTINGS.DS_SL_BITS; ii++)
	{
		map_out.x[ii] = data_in[ii];
		map_out.y[ii] = data_in[ii];
		for(kk=0; kk<4; kk++)
		{
			map_out.x[ii] = map_out.x[ii]^conv_code[conv_mother_x[kk]-1];
			map_out.y[ii] = map_out.y[ii]^conv_code[conv_mother_y[kk]-1];
		}
		// Add the current data item to the code and shift the existing ones up by one
		memcpy(tmp, conv_code, CONV_M-1);
		memcpy(conv_code+1, tmp, CONV_M-1);
		conv_code[0] = data_in[ii];
	}
	// The input memory is not freed in this function

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log_xy(dump_refs.convolutional_encode, dump_refs.convolutional_encode_txt, map_out, DVBT_SETTINGS.DS_SL_BITS);
	}

	return map_out;
}

//!Convolutional encoding initialisation function
/**
 * This function allocates memory for and initialises the values of a variable which is used in the convolutional encoding process from one execution to the next. This code is modified after each execution. The mother codes for the x and y output maps are also initialised by this function, however these arrays are fully defined at compile time and, hence, do not need their memory to be allocated.
 * @param conv_code Pointer to the convolutional code to initialise
 */
void conv_init(unsigned char* conv_code)
{
	MALLOC(conv_init_register, CONV_M);
	memset(conv_init_register, 0, CONV_M);
	memset(conv_code, 0, CONV_M);
	// generator polynomials
	conv_mother_x[0] = 1;
	conv_mother_x[1] = 2;
	conv_mother_x[2] = 3;
	conv_mother_x[3] = 6;

	conv_mother_y[0] = 2;
	conv_mother_y[1] = 3;
	conv_mother_y[2] = 5;
	conv_mother_y[3] = 6;
}
//!Convolutional encoding cleanup function 
/**
 * This function releases the memory used in the convolution process (carried from one execution to the next) by this module.
 */
void conv_clean()
{
	free(conv_init_register);
}
