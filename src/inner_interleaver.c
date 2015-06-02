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
#include "inner_interleaver.h"
#include "dump.h"
#include "ds_ii_hq.h"	// Hq settings- sourced originally from opendvb code and extracted from octave (put in array)
//! Bit and Symbol Interleaving
/**
 * Bit and symbol interleaving of the punctured data. The result is passed onto the mapper.
 * @param data_in Pointer to an array of punctured data
 * @param scnt Symbol count
 * @return Pointer to the interleaved data
 */
unsigned char* inner_interleave(unsigned char* data_in, int scnt)
{
	// Parameters
	int sblock_size = DS_II_BS*DVBT_SETTINGS.DS_MS_BITS;
	int num_blocks = DVBT_SETTINGS.DS_PU_OL/sblock_size;
	unsigned char mapping[2] = {0,1};	
	unsigned char h_param[6] = {0, 63, 105, 42, 21, 84};	//H Parameters- see line 93 of innter_interleave.m
	int ii,kk,ee,ww;			// Iterator variables
	int hh;					// For H(e,w)

	// Processing variables
	unsigned char* x;			// Pointer to input chunk
	unsigned char a[DS_II_BS][DVBT_SETTINGS.DS_MS_BITS];	// Processing array 1
	unsigned char b[DS_II_BS][DVBT_SETTINGS.DS_MS_BITS];	// Processing array 2
	unsigned char o[DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS][DVBT_SETTINGS.DS_MS_BITS];	// Output processing array 1
	unsigned char y[DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS][DVBT_SETTINGS.DS_MS_BITS];	// Output processing array 2
	unsigned char* data_out;		// Resultant output pointer

	memset(y, 0, DVBT_SETTINGS.DS_SL_II);			// Clear y memory space

	for(ii=0; ii<num_blocks; ii++)
	{
		// Prepare processing
		x = data_in+ii*sblock_size;		// Point to appropriate memory location
		memset(a, 0, DS_II_BS*DVBT_SETTINGS.DS_MS_BITS);	// Clear a memory space
		memset(b, 0, DS_II_BS*DVBT_SETTINGS.DS_MS_BITS);	// Clear b memory space

		// Step 1: Demultiplexing
		// b is the demultiplexed, mapped input at this stage
		// For the details on the mapping layout, see lines 75-85 of inner_interleave.m
		// The iterations are inside the if statements to save on clock cycles (as oppose to an outer loop)
		if(DVBT_SETTINGS.DS_MS_BITS == 6)	// 64QAM
		{
			for(kk=0; kk<DS_II_BS; kk++)
			{
				b[kk][0] = x[6*kk];
				b[kk][2] = x[6*kk+1];
				b[kk][4] = x[6*kk+2];
				b[kk][1] = x[6*kk+3];
				b[kk][3] = x[6*kk+4];
				b[kk][5] = x[6*kk+5];
			}
		}
		else if(DVBT_SETTINGS.DS_MS_BITS == 4)	// 16QAM
		{
			for(kk=0; kk<DS_II_BS; kk++)
			{
				b[kk][0] = x[4*kk];
				b[kk][2] = x[4*kk+1];
				b[kk][1] = x[4*kk+2];
				b[kk][3] = x[4*kk+3];
			}
		}
		else					// 4QAM
		{
			for(kk=0; kk<DS_II_BS; kk++)
			{
				b[kk][0] = x[2*kk];
				b[kk][1] = x[2*kk+1];
			}
		}

		// Step 2: Bit Interleaver
		for(ee=0; ee<DVBT_SETTINGS.DS_MS_BITS; ee++)
		{
			for(ww=0; ww<DS_II_BS; ww++)
			{
				// compute H(e,w)
				hh = (ww+h_param[ee])%DS_II_BS;
				//a[ww][ee] = b[hh][ee];
				y[ww+ii*DS_II_BS][ee] = b[hh][ee];
			}
		}
	}
	// Free memory of input variable
	free(data_in);

	// Step 3: Symbol Interleaver
	MALLOC(data_out, DVBT_SETTINGS.DS_SL_II);
	memset(data_out, 0, DVBT_SETTINGS.DS_SL_II);
	// As above, the looping was done inside the conditions
	// This could be made far more concise, but it is more efficient speed
	// wise to use hard coding, rather than variables, where it is
	// practical
	if(DVBT_SETTINGS.DS_MS_BITS == 6)		// 64 QAM
	{
		for(ii=0; ii<DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS; ii++)
		{
			if(scnt%2 == 0)
			{
				data_out[6*DS_II_Hq[ii]] = y[ii][0];
				data_out[6*DS_II_Hq[ii]+1] = y[ii][1];
				data_out[6*DS_II_Hq[ii]+2] = y[ii][2];
				data_out[6*DS_II_Hq[ii]+3] = y[ii][3];
				data_out[6*DS_II_Hq[ii]+4] = y[ii][4];
				data_out[6*DS_II_Hq[ii]+5] = y[ii][5];
			}
			else
			{
				data_out[6*ii] = y[DS_II_Hq[ii]][0];
				data_out[6*ii+1] = y[DS_II_Hq[ii]][1];
				data_out[6*ii+2] = y[DS_II_Hq[ii]][2];
				data_out[6*ii+3] = y[DS_II_Hq[ii]][3];
				data_out[6*ii+4] = y[DS_II_Hq[ii]][4];
				data_out[6*ii+5] = y[DS_II_Hq[ii]][5];
			}
		}
	}
	else if(DVBT_SETTINGS.DS_MS_BITS == 4)		// 16 QAM
	{
		for(ii=0; ii<DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS; ii++)
		{
			if(scnt%2 == 0)
			{
				data_out[4*DS_II_Hq[ii]] = y[ii][0];
				data_out[4*DS_II_Hq[ii]+1] = y[ii][1];
				data_out[4*DS_II_Hq[ii]+2] = y[ii][2];
				data_out[4*DS_II_Hq[ii]+3] = y[ii][3];
			}
			else
			{
				data_out[4*ii] = y[DS_II_Hq[ii]][0];
				data_out[4*ii+1] = y[DS_II_Hq[ii]][1];
				data_out[4*ii+2] = y[DS_II_Hq[ii]][2];
				data_out[4*ii+3] = y[DS_II_Hq[ii]][3];
			}
		}
	}
	else						// 4 QAM
	{
		for(ii=0; ii<DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS; ii++)
		{
			if(scnt%2 == 0)
			{
				data_out[2*DS_II_Hq[ii]] = y[ii][0];
				data_out[2*DS_II_Hq[ii]+1] = y[ii][1];
			}
			else
			{
				data_out[2*ii] = y[DS_II_Hq[ii]][0];
				data_out[2*ii+1] = y[DS_II_Hq[ii]][1];
			}
		}
	}

	// Different logging to match dvbtenco
//   * Symbol interleaver dump: files *_si.bin
//          (unsigned char); bit ordering is (MSB ... ... LSB)
//          4-QAM  --> ( 0  0  0  0  0  0 y0 y1)
//          16-QAM --> ( 0  0  0  0 y0 y1 y2 y3)
//          64-QAM --> ( 0  0 y0 y1 y2 y3 y4 y5).
//          "fread((unsigned char *), sizeof (unsigned char), int, FILE *)"
	MALLOC(data_in, DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS);
	memset(data_in, 0, DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS);
	for(ii = 0; ii<DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS; ii++)
	{
		for(kk = 0; kk<DVBT_SETTINGS.DS_MS_BITS; kk++)
		{
			data_in[ii] = data_in[ii]*2 + data_out[ii*DVBT_SETTINGS.DS_MS_BITS+kk];
		}
	}

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
// 		dvbotc_log(dump_refs.inner_interleave, dump_refs.inner_interleave_txt, data_out, DVBT_SETTINGS.DS_SL_II);
		dvbotc_log(dump_refs.inner_interleave, dump_refs.inner_interleave_txt, data_in, DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS);
	free(data_in);

	return data_out;
}
