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
#include "puncturing.h"
#include "dump.h"
/**
 * Puncturing is the process through which an mxn code for convolution can be generated from a simple 1/2 code (which is essentially no puncturing) straight from the convolutional encoding stage. In this implementation two modes, 1/2 and 2/3, are implemented, however there are various other possible modes. The result of this stage is fed into the inner interleaver stage.
 * @param data_in Input X-Y data map from the convolutional encoder
 * @return Pointer to punctured convolutionally encoded data
 */
unsigned char* puncturing (struct xymap data_in)
{
	unsigned char* data_out;	// Output data pointer
	int ii;				// Iterator variable

	// Allocate memory for the output structure
	// Change DS_PU_OL if changing the puncturing rate
	MALLOC(data_out, DVBT_SETTINGS.DS_PU_OL);
	memset(data_out, 0, DVBT_SETTINGS.DS_PU_OL);

	// Puncture the data as required (we can use the numerator to determine the mode
	switch( DVBT_SETTINGS.DS_PUN_PM_num )
	{
		case 1:		// Puncturing rate must be 1/2
			for(ii=0; ii<DVBT_SETTINGS.DS_SL_BITS; ii++)
			{
				data_out[2*ii] = data_in.x[ii];
				data_out[2*ii+1] = data_in.y[ii];
			}
			break;
		case 2:		// Puncturing rate must be 2/3
			for(ii=0; ii<DVBT_SETTINGS.DS_SL_BITS; ii=ii+2)
			{
				data_out[3*ii/2] = data_in.x[ii];
				data_out[3*ii/2+1] = data_in.y[ii];
				data_out[3*ii/2+2] = data_in.y[ii+1];
			}
			break;
		// No default required as the parameter checking will detect and problems before this point
	}

	// Release memory from convolutional encoder
	free(data_in.x);
	free(data_in.y);

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log(dump_refs.puncturing, dump_refs.puncturing_txt, data_out, DVBT_SETTINGS.DS_PU_OL);
	}
	return data_out;
}
