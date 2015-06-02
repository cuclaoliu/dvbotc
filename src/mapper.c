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
#include<complex.h>
#include "dvbotc.h"
#include "mapper.h"
#include "qam_ref.h"
#include "dump.h"
//! Map data to the IQ plane based on the selected level of QAM
/** This function originally used chars rather than floats to keep things fast and lowest overhead possible, however the function operated correctly for 4QAM only. While this made thing simple, it was restrictive. A new version based on table lookups and floating point math has been implemented that is compatible with any QAM level. This function is based very little on the opendvb implementation and more on the fundamental theory behind mapping.
 *
 * @param data_in Bit stream to be mapped (using one unsigned char for each bit)
 * @return Pointer to array of complex points containing the mapped data
 */
complex * map(unsigned char* data_in)
{
	complex* data_out;
	int tmp_re, tmp_im, tmp;
	int n = DVBT_SETTINGS.DS_SL_II/DVBT_SETTINGS.DS_MS_BITS;	// Simplify code- half of the size of the input
	int ii, jj;				// Iterator variables

	// Allocate memory
	MALLOC(data_out, n*sizeof(complex));

	// Clear memory
	memset(data_out, 0, n*sizeof(complex));

	// This is directly based on the dvb standard
	// page 22 and hard-coded to 4QAM.
	// Map symbols
	for(ii = 0; ii<DVBT_SETTINGS.DS_SL_II; ii = ii + DVBT_SETTINGS.DS_MS_BITS)
	{
/*		// To reduce complex number math, temporary integers are used until mapping is complete
		tmp_re = 1;
		tmp_im = 1;
		// For the first QAM stage, 1 is used, then 3 then both 5 and 7
		if(DVBT_SETTINGS.DS_MS_BITS == 6)	// 64QAM
		{
			if(data_in[ii+2] == 0)
			{
				tmp_re+=2;
			}
			if(data_in[ii+3] == 0)
			{
				tmp_im+=2;
			}
		}
		if(DVBT_SETTINGS.DS_MS_BITS == 4)	// 16QAM
		{
			if(data_in[ii+2] == 0)
			{
				tmp_re+=2;
			}
			if(data_in[ii+3] == 0)
			{
				tmp_im+=2;
			}
		}
		if(data_in[ii] == 1)
		{
			tmp_re = -tmp_re;
		}
		if(data_in[ii+1] == 1)
		{
			tmp_im=-tmp_im;
		}
		data_out[ii/DVBT_SETTINGS.DS_MS_BITS] = tmp_re + I*tmp_im;
*/
		// Lookup based approach for mapping
		tmp = 0;
		for(jj = 0; jj < DVBT_SETTINGS.DS_MS_BITS; jj++)
		{
			tmp = tmp*2+data_in[ii+jj];
		}
		data_out[ii/DVBT_SETTINGS.DS_MS_BITS] = qam_re[DVBT_SETTINGS.DS_MS_BITS/2-1][tmp] + 
							I*qam_im[DVBT_SETTINGS.DS_MS_BITS/2-1][tmp];
// 		data_out[ii/DVBT_SETTINGS.DS_MS_BITS] *= DVBT_SETTINGS.DS_RS_NORM;
// 		data_out[ii/DVBT_SETTINGS.DS_MS_BITS] /= sqrt(DVBT_SETTINGS.DS_RS_NORM);
	}

	// Release memory of input data structure
	free(data_in);

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log_complex(dump_refs.map, dump_refs.map_txt, data_out, n);
	}

	return data_out;
}

