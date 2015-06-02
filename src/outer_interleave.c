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
#include "outer_interleave.h"
#include "dump.h"
//! Outer Interleaver
/**
 * This function performs outer interleaving on the RS encoded data from the previous step of the DVB-T encoding process. It is based on the Forney approach (refer to the DVB-T standard, page 12 of V1.5.1).
 * @param data_in Pointer to an array of RS Encoded data
 * @param oi_queue Pointer to the current queue of interleaved data (that hasn't been outputted as yet)
 * @return Pointer to the interleaved data
 */
unsigned char* outer_interleave(unsigned char* data_in, unsigned char * oi_queue)
{
	int mm, ii;			// Iterators
	unsigned char* data_out;	// Output memory pointer

	// Perform interleaving
	for (mm = 0; mm < DS_OI_M; mm++)
		for (ii = 0; ii < DS_OI_I; ii++)
			oi_queue[ii*204 + mm*12 + ii] = data_in[mm*12 + ii];
	// Re-use memory
	data_out = data_in;
	for(ii = 0; ii < DS_OI_QS; ii++)
	{
		if(ii < DS_PL_OI)
		{
			data_out[ii] = oi_queue[ii];
			oi_queue[ii] = oi_queue[ii + DS_PL_OI];
		}
		else if(ii < DS_OI_QS - DS_PL_OI)
			oi_queue[ii] = oi_queue[ii + DS_PL_OI];
		else
			oi_queue[ii] = 0;
	}

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
		dvbotc_log(dump_refs.outer_interleave, dump_refs.outer_interleave_txt, data_out, 204);

	return data_out;
}

//! Initialise Outer Interleaver
/**
 * Initialises the outer interleaver (allocates memory for the queue)
 * @param oi_queue Pointer to the outer interleaver queue to be initialised
 */
void initialise_oi(unsigned char* oi_queue)
{
	int ii;			// Iterator
	MALLOC(oi_queue, DS_OI_QS);
	for(ii = 0; ii < DS_OI_QS; ii++)
	{
		oi_queue[ii] = 0;
	}
	return;
}

//! Clean Outer Interleaver
/**
 * Cleans the outer interleaver (de-allocates memory for the queue)
 * @param oi_queue Pointer to the outer interleaver queue to be dealocated
 */
void clean_oi(unsigned char* oi_queue)
{
	free(oi_queue);
	return;
}

