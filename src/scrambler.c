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
#include "scrambler.h"
#include "dump.h"
//! Data Scrambler
/**
 * This function scrambles the data to be transmitted in order to spread the spectral power of the signal. This improves the error correcting capability of the transmission system.
 * @param data_in Pointer to the MPEG-TS packet to be scrambled
 * @param cnt_in Number of bytes read for the MPEG-TS packet (if it is lower than 188, then the packet will be zero padded)
 * @param prbs_reg Pointer to the pseudo-random binary generator state register
 * @return Pointer to an array containing the original data in scrambled form
 */
unsigned char* scramble (unsigned char* data_in, int cnt_in, unsigned char* prbs_reg)
{
	unsigned char* data_out;		// Above line replaced by this to recycle memory
	unsigned char sync_byte = DS_SC_SB;	// local copy of the sync byte
	unsigned char* prbs;			// Pointer to PRBS result
	int ii;					// variable for iterations
	if(data_in[0] != sync_byte)
	{
		fprintf (stderr, "sync byte: %2.2x expected, %2.2x found", sync_byte, data_in[0]);
	}

	//////////////////////////////////////////////
	// Perform actions
	if (cnt_in % DS_SC_PP == 0)
	{
		// first packet: invert sync byte and reset prbs state
		sync_byte = DS_SC_ISB;
		prbs_reg[0] = 1;
		prbs_reg[1] = 0;
		prbs_reg[2] = 0;
		prbs_reg[3] = 1;
		prbs_reg[4] = 0;
		prbs_reg[5] = 1;
		prbs_reg[6] = 0;
		prbs_reg[7] = 1;
		prbs_reg[8] = 0;
		prbs_reg[9] = 0;
		prbs_reg[10] = 0;
		prbs_reg[11] = 0;
		prbs_reg[12] = 0;
		prbs_reg[13] = 0;
		prbs_reg[14] = 0;
					// pseudo random binary sequence (PRBS) generator
					// DVBT_SETTINGS.scrambler.init_prbs_register = ...
					//  [1 0 0 1 0 1 0 1 0 0 0 0 0 0 0];

	}
	else
	{
		// following packets: leave sync byte and advance PRBS by one
		sync_byte = DS_SC_SB;
		prbs = scrambler_prbs(1, prbs_reg);
		free(prbs);	// ensures that there are no leaks from the above function
	}

	prbs = scrambler_prbs (DS_RS_K-1, prbs_reg);

	// Recycling is good for the environment- data_in memory is just re-used
	// as the data_out location as they are the same size
	data_out = data_in;
	data_out[0] = sync_byte;
	for(ii = 1; ii < DS_RS_K; ii++)
	{
		data_out[ii] = data_in[ii]^prbs[ii-1];
	}

	// Done with prbs object
	free(prbs);

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log(dump_refs.scramble, dump_refs.scramble_txt, data_out, DS_RS_K);
	}
/*        fwrite (data_out, sizeof(char), DS_RS_K, dump_refs.scramble);
        for(ii=0; ii<DS_RS_K; ii++)
        {
                fprintf(dump_refs.scramble_txt, "%d", data_out[ii]);
                if(ii < DS_RS_K - 1)
                {
                        fprintf(dump_refs.scramble_txt, "\t");
                }
                else
                {
                        fprintf(dump_refs.scramble_txt, "\n");
                }
        }*/

	return data_out;
}
