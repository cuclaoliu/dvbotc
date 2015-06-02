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
#include "insert_reference_signals.h"
#include "dump.h"
#include "ref_sig.h"

int tps_set[17] = {34, 50, 209, 346, 413, 569, 595, 688, 790, 901, 1073, 1219, 1262, 1286, 1469, 1594, 1687};
int tps_set_length = 17;
// The above may need to be adusted in the case of 8k mode implementation

/**
 * This function takes care of the insertion of reference signals including the scattered and continual pilots, as well as the TPS (transmission parameter settings) signals. The different signals work in different ways, but essentially they are slotted in amongst the data using different modulation methods.\\ The locations of the pilots are detailed in the DVB standard, but essentially the pilot information is spread throughout the one symbol. TPS information is spread over the whole OFDM frame (68 symbols) such that just one bit is transmitted each symbol of the OFDM frame. The details of the formation of the TPS information is available in the DVB standard.
 * @param data_in Mapped data which has not yet been normalised
 * @param scnt The sender count (position in OFDM frame)
 * @return Pointer to complex array containing the OFDM symbol ready for an IFFT and transmission
 */
complex* insert_reference_signals(complex* data_in, int scnt)
{
	int vv, tt, pp, px, uu, tps;			// Iterator and index variables
	complex* data_out;
	int symbol_tps_bit = 0;				// New tps bit value to transmit this symbol

	// Allocate memory for output
	MALLOC(data_out, sizeof(complex)*DS_OFDM_CC);

	// Initialise variables
	vv = 0;		// current input payload carrier index
	px = 0;		// current pilot index in pilot_set
	tps = 0;	// current tps index in tps_set
	for(uu=0; uu<DS_OFDM_CC; uu++)			// for all output carriers
	{
		pp = pilot_set[scnt%4][px];	// get next appropriate pilot
		if(tps < tps_set_length)	// get next tps
		{
			tt = tps_set[tps];
		}
		else
		{
			tt = 0;
		}
		if(uu == pp)					// it's a pilot signal
		{
			data_out[uu] = (float)4/3 * 2 * (0.5 - refsig_w[pp]);
			// No imaginary component
			px = px + 1;
		}
		else if(uu == tt)				// it's a TPS signal
		{
			// Use the calculated value for this symbol in the frame
			if(scnt%DS_OFDM_SPF == 0)
			{
				// The first bit in the frame is done a little differently
				symbol_tps_history[tps] = 2 * (0.5 - refsig_w[tt]);
			}
			else
			{
				// When the next bit to send is zero, there is no change in the output sign
				if(symbol_tps_bits[scnt/DS_OFDM_SPF][scnt%DS_OFDM_SPF] == 1)
				{
					symbol_tps_history[tps] = -symbol_tps_history[tps];
				}
			}
			data_out[uu] = symbol_tps_history[tps];
			// No imaginary component
			tps = tps + 1;
		}
		else						// it's a payload carrier
		{
			data_out[uu] = data_in[vv]/sqrt(DVBT_SETTINGS.DS_RS_NORM);
			vv = vv + 1;
		}
	}
	// Free input variable's allocated memory
	free(data_in);

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log_complex2(dump_refs.insert_reference_signals, dump_refs.insert_reference_signals_txt, data_out, DS_OFDM_CC);
	}

	return data_out;
}

/**
 * This function initialises the insert reference signal function. In the main, the role of the task is to generate and modulate the TPS information. This transmission is used to advise receivers of the parameters used to transmit the transport stream at the frequency currently being tuned into. Practically this is achieved through the use of DBPSK transmission using some of the otherwise unused carriers. There are four frames in each superframe with 68 symbols per frame. Each symbol is made up of 2k carriers and in each symbol one bit is sent of the 68 to be transmitted (it is transmitted in 17 seperate carriers per symbol). This means that the only parameters required by the receiver are the channel bandwidth and the number of carriers (which are often fixed for a deployment or can easily be tested for).
 */
void init_reference_signals()
{
	int ii, cc;

	// TPS sync word (see page 31 of DVB standard)
	int tps_sync[16] = {0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0};

	// BCH Generator polynomial for TPS error protection (see page 34 of DVB standard)
	int gx_bch[15] = {1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1};

	// The following is a pregenerated set of BCH codes applicable to the default parameters for the
	// dvbotc application
	// Other modes will need to be manually added (or BCH encoding properly implemented)
	char bch_code[][14] = {	{0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0},
				{0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0},
				{0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1},
				{0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
				};


	for(ii = 0; ii<4; ii++)
	{
		// The first bit is a one (1) and is specified below in the modulation for the frame's tps
		// The next 16 bits are the synchronisation bits
		for(cc = 0; cc<16; cc++)
		{
			symbol_tps_bits[ii][cc+1] = tps_sync[cc];
			// If this is the second or fourth frame in the superframe, then we use an
			// inverted synchronisation sequence
			if(ii==1 || ii==3)
			{
				if(symbol_tps_bits[ii][cc+1]==1)
				{
					symbol_tps_bits[ii][cc+1] = 0;
				}
				else
				{
					symbol_tps_bits[ii][cc+1] = 1;
				}
			}
		}

		// TPS length indicator (bits 17 to 22) ref page 31-32 of standard
		// In this standard the only difference can be when the Cell identifier is used
		symbol_tps_bits[ii][17] = 0;
		symbol_tps_bits[ii][18] = 1;
		// The third bit is a one if we have the Cell Identification information included
		symbol_tps_bits[ii][19] = 0;
		if(dvbotc_options.cell_id_found == 1)
		{
			// If the Cell ID is specified then we cater for it in the TPS information
			symbol_tps_bits[ii][19] = 1;
		}
		symbol_tps_bits[ii][20] = 1;
		symbol_tps_bits[ii][21] = 1;
		symbol_tps_bits[ii][22] = 1;

		// FRAME NUMBER (bits 23 and 24) ref page 32 of standard
		symbol_tps_bits[ii][23] = 0;
		if(ii==2 || ii==3)
		{
			symbol_tps_bits[ii][23] = 1;
		}
		symbol_tps_bits[ii][24] = 0;
		if(ii==1 || ii==3)
		{
			symbol_tps_bits[ii][24] = 1;
		}

		// CONSTELLATION (bits 25 and 26) ref page 32 of standard
		symbol_tps_bits[ii][25] = 0;
		if(DVBT_SETTINGS.DS_MS_BITS == 6)
		{
			symbol_tps_bits[ii][25] = 1;
		}
		symbol_tps_bits[ii][26] = 0;
		if(DVBT_SETTINGS.DS_MS_BITS == 4)
		{
			symbol_tps_bits[ii][26] = 1;
		}

		// HIERARCHY (bits 27 to 29) ref page 32 of standard
		// Not yet implemented, hence these are all 0 (meaning Non Hierarchical (NH))
		symbol_tps_bits[ii][27] = 0;
		symbol_tps_bits[ii][28] = 0;
		symbol_tps_bits[ii][29] = 0;

		// CODE RATE (bits 30 to 35) ref page 32 to 33 of standard
		// HP is the first three, then the next three are LP, however because we use NH mode the first
		// three only are used and the next three are zero
		symbol_tps_bits[ii][30] = 0;
		if(DVBT_SETTINGS.DS_PUN_PM_num == 7)
		{
			symbol_tps_bits[ii][30] = 1;
		}
		symbol_tps_bits[ii][31] = 0;
		if(DVBT_SETTINGS.DS_PUN_PM_num == 5 || DVBT_SETTINGS.DS_PUN_PM_num == 3)
		{
			symbol_tps_bits[ii][31] = 1;
		}
		symbol_tps_bits[ii][32] = 0;
		if(DVBT_SETTINGS.DS_PUN_PM_num == 2 || DVBT_SETTINGS.DS_PUN_PM_num == 5)
		{
			symbol_tps_bits[ii][32] = 1;
		}
		symbol_tps_bits[ii][33] = 0;
		symbol_tps_bits[ii][34] = 0;
		symbol_tps_bits[ii][35] = 0;

		// GUARD INTERVALS (bits 36 and 37) ref page 33 of standard
		symbol_tps_bits[ii][36] = 0;
		if(DVBT_SETTINGS.DS_OFDM_IGI == 8 || DVBT_SETTINGS.DS_OFDM_IGI == 4)
		{
			symbol_tps_bits[ii][36] = 1;
		}
		symbol_tps_bits[ii][37] = 0;
		if(DVBT_SETTINGS.DS_OFDM_IGI == 16 || DVBT_SETTINGS.DS_OFDM_IGI == 4)
		{
			symbol_tps_bits[ii][37] = 1;
		}

		// TRANSMISSION MODE (bits 38 and 39) ref page 33 of standard
		// Only 2k mode is implemented at this stage so all bits are set to zero
		symbol_tps_bits[ii][38] = 0;
		symbol_tps_bits[ii][39] = 0;

		// CELL IDENTIFIER (bits 40 to 47) ref page 34 of standard
		// These are zero unless the cell identifier is used, when data is sent differently
		// on a frame by frame basis
		// The first and third frames transmit the most signifficant byte (DVBT_SETTINGS.DS_RS_CID1)
		// Initialise the cell identification space to zero (in case it is not used)
		symbol_tps_bits[ii][40] = 0;
		symbol_tps_bits[ii][41] = 0;
		symbol_tps_bits[ii][42] = 0;
		symbol_tps_bits[ii][43] = 0;
		symbol_tps_bits[ii][44] = 0;
		symbol_tps_bits[ii][45] = 0;
		symbol_tps_bits[ii][46] = 0;
		symbol_tps_bits[ii][47] = 0;
		if(dvbotc_options.cell_id_found == 1)
		{
			if(ii == 1 || ii == 3)
			{
				if(DVBT_SETTINGS.DS_RS_CID1&0x80)
					symbol_tps_bits[ii][40] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x40)
					symbol_tps_bits[ii][41] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x20)
					symbol_tps_bits[ii][42] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x10)
					symbol_tps_bits[ii][43] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x08)
					symbol_tps_bits[ii][44] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x04)
					symbol_tps_bits[ii][45] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x02)
					symbol_tps_bits[ii][46] = 1;
				if(DVBT_SETTINGS.DS_RS_CID1&0x01)
					symbol_tps_bits[ii][47] = 1;
			}
			else
			{
				if(DVBT_SETTINGS.DS_RS_CID2&0x80)
					symbol_tps_bits[ii][40] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x40)
					symbol_tps_bits[ii][41] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x20)
					symbol_tps_bits[ii][42] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x10)
					symbol_tps_bits[ii][43] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x08)
					symbol_tps_bits[ii][44] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x04)
					symbol_tps_bits[ii][45] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x02)
					symbol_tps_bits[ii][46] = 1;
				if(DVBT_SETTINGS.DS_RS_CID2&0x01)
					symbol_tps_bits[ii][47] = 1;
			}
		}
		// RESERVED BITS (bits 48 to 53) ref page 31 of standard
		// These are zero at all times
		symbol_tps_bits[ii][48] = 0;
		symbol_tps_bits[ii][49] = 0;
		symbol_tps_bits[ii][50] = 0;
		symbol_tps_bits[ii][51] = 0;
		symbol_tps_bits[ii][52] = 0;
		symbol_tps_bits[ii][53] = 0;

		// ERROR PROTECTION (bits 54 to 67) ref page 34 of standard
		// We are using a BCH (67,53, t = 2) code for this with the generator polynomial of
		// h(x) = x^14 + x^9 + x^8 + x^6 + x^5 + x^4 + x^2 + x + 1
		for(cc = 0; cc<14; cc++)
		{
			// Just zero the values for now
			symbol_tps_bits[ii][54+cc] = 0;
		}

		// Perform BCH encoding using the generator polynomial from the dvb standard
		// Note that the first bit is not included in the codeword

		// Fixed encoding for now- using values generated by Matlab that apply only for the
		// default encoding parameters

		// Add redundancy bits to the end of the codeword
		for(cc = 0; cc<14; cc++)
		{
			symbol_tps_bits[ii][54+cc] = bch_code[cc];
		}

		// DBPSK Modulation of the TPS information
		symbol_tps_values[ii][0] = 2 * (0.5 - refsig_w[tps_set[0]]);
#ifdef DEBUG
		printf("Code No %d is:", ii);
#endif
		for(cc = 1; cc<68; cc++)
		{
#ifdef DEBUG
			printf("%d", symbol_tps_bits[ii][cc]);
			// Add a space to make logs created easier to read
			if(cc%4 == 0)
				printf(" ");
#endif
			if(symbol_tps_bits[ii][cc] == 0)
			{
				symbol_tps_values[ii][cc] = symbol_tps_values[ii][cc-1];
			}
			else
			{
				symbol_tps_values[ii][cc] = -symbol_tps_values[ii][cc-1];
			}
		}
#ifdef DEBUG
		// Output data bits to debug encoder
		// The first bit is not included in the bch encoder
		printf("\nCode_%d = [", ii);
		for(cc = 1; cc<68; cc++)
		{
			printf(" %d",symbol_tps_bits[ii][cc]);
		}
		printf("]\n");
		printf("\n");
#endif
	}
	return;
}
