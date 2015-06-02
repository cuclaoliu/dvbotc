/***************************************************************************
 *   Copyright (C) 2007 by Damien Phillip Morrissey,,,   		   *
 *   damien@damienandlaurel.com 					   *
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
// DVB Settings
// Fixed settings (those that really don't change) are #define'd, but those that can change are part of the
// Parameter structure

#ifndef DVBOTC_PARAMETER_H_GUARD
#define DVBOTC_PARAMETER_H_GUARD
// Scrambler Settings
// From global_settings.m, lines 30-43
const int DS_SC_SB = 0x47; 		//!< Scrambler sync byte
				/**< DVBT_SETTINGS.scrambler.sync_byte = hex2dec('47');*/
const int DS_SC_ISB = 0xB8; 		//!< Scrambler inverted sync byte
				/**< DVBT_SETTINGS.scrambler.inv_sync_byte = 255 - 
					DVBT_SETTINGS.scrambler.sync_byte;*/
const int DS_SC_IPR = 0x4A80;		//!< Initial shift register contents
				/**< Initial contents of shift register of
					pseudo random binary sequence (PRBS) generator\n
					DVBT_SETTINGS.scrambler.init_prbs_register = ...
					[1 0 0 1 0 1 0 1 0 0 0 0 0 0 0]; */
const int DS_SC_PP = 8;		//!< Scrambler pseudorandom binary sequence period
				/**< DVBT_SETTINGS.scrambler.prbs_period = 8;*/

// Simplified Reed-Solomon Settings (using rscode makes this simpler)
// From rs_init.m, lines 35-36
				// shortened RS(204,188,t=8) code (p. 12)
const int DS_RS_K = 188; 		//!< Number of bytes input into the Red-solomon encoder
const int DS_RS_N = 204; 		//!< Final length of the RS code protected packet (including redundant bytes)


// Packet Length Settings
// From global_settings.m, lines 211-214
const int DS_PL_MUX = DS_RS_K; 	//!< Packet length (in bytes) of the packet after the MUX, before the RS encoder
				/**< DVBT_SETTINGS.packet_length.mux = DVBT_SETTINGS.rs.k;*/
const int DS_PL_RS = DS_RS_N; 	//!< Packet length (in bytes) of the packet after RS encoding
				/**< DVBT_SETTINGS.packet_length.rs = DVBT_SETTINGS.rs.n;*/
const int DS_PL_OI = DS_RS_N; 	//!< Packet length (in bytes) of the packet into the outer interleaver
				/**< DVBT_SETTINGS.packet_length.outer_interleaver = DVBT_SETTINGS.rs.n;*/

// Outer Interleaver Settings
// From global_settings.m, lines 59-67
const int DS_OI_I = 12; 		// DVBT_SETTINGS.outer_interleaver.i = 12;
const int DS_OI_M = 17; 		// DVBT_SETTINGS.outer_interleaver.m = 17;
const int DS_OI_QS = 2448; 		//! Outer interleaver Queue size
				/**< DVBT_SETTINGS.outer_interleaver.queue_size = 
					DVBT_SETTINGS.outer_interleaver.i^2 
					* DVBT_SETTINGS.outer_interleaver.m;*/
// OFDM Frame Structure Settings
// From global_settings.m, lines 79-83
const int DS_OFDM_OM = 2048;		//!< OFDM mode (number of carriers)
				/**< Only 2k mode has been implemented in this version of dvbotc.\n
					DVBT_SETTINGS.ofdm_mode = 2048;*/
const int DS_OFDM_PC = 1512*DS_OFDM_OM/2048;		//!< The number of OFDM carriers that will contain useful data
				/**< DVBT_SETTINGS.payload_carriers = 1512;*/
const int DS_OFDM_KMIN = 0;
const int DS_OFDM_KMAX = 1704**DS_OFDM_OM/2048;
const int DS_OFDM_UC = DS_OFDM_KMAX-DS_OFDM_KMIN+1;		//!< The number of OFDM carriers that will be used (including reference signals)
				/** DVBT_SETTINGS.used_carriers = 1705;*/
const int DS_OFDM_CC = DS_OFDM_UC;	//!< The OFDM carrier count (same as the used carriers)
				/**< DVBT_SETTINGS.carrier_count = DVBT_SETTINGS.used_carriers;*/
const int DS_OFDM_SPF = 68;		//!< Number of OFDM symbols per OFDM frame
				/**< DVBT_SETTINGS.symbols_per_frame = 68;*/
const int DS_OFDM_FPSF = 4;		//!< Number of OFDM frames per OFDM superframe
				/**< DVBT_SETTINGS.frames_per_superframe = 4;*/

// Iner Interleaver Settings
// From global_settings.m, lines 90-135
const int DS_II_BS = 126;		//!< Inner interleaver block size (bytes)
				/**< DVBT_SETTINGS.inner_interleaver.block_size = 126;*/

// The remainder of settings can be found in the structure defined below
//! Contains DVB-T parameters calculated at run time from user options
struct dvbt_settings
{
	int DS_PUN_PM_num;	//!< Numerator for puncturing rate
	int DS_PUN_PM_den;	//!< Denominator for puncturing rate

	char DS_MS_QAM;		//!< QAM Mode
	char DS_MS_ALPHA;	//!< Alpha Setting
	char DS_MS_BITS;	//!< Bits per sample based on QAM level

	int DS_SL_BITS;		//!< Bits per OFDM symbol
	int DS_SL_II;		//!< Inner interleaver symbol length
	int DS_PU_OL;		//!< Bits resulting from puncturing

	float DS_RS_NORM;	//!< Normalization factor c based on QAM and alpha parameter used when inserting reference signals
	char DS_RS_CID1;	//!< First byte of the Cell Identification parameter (if used)
	char DS_RS_CID2;	//!< Second byte of the Cell Identification parameter (if used)

	char DS_OFDM_IGI;	//!< Inverse Guard Interval (e.g. 4 means GI of \f$ \frac{1}{4} \f$)
	int DS_OFDM_GL;		//!< Guard length in terms of OFDM carriers

	int DS_SL_FFT;		//!< FFT symbol length
	int DS_SL_ADC;		//!< ADC (Analog to Digital Converter) symbol length

	int DS_CH_BW;		//!< Channel bandwidth (in MHz) for Digital to Analogue Converter
};

int parameter_init();
int val_gi(const char* arg);	// Validate guard interval selection
int val_pu(const char* arg);	// Validate puncturing rate selection
int val_qa(const char* arg);	// Validate qam level selection
int val_bw(const char* arg);	// Validate channel bandwidth selection
int val_id(const char* arg);	// Validate channel identifier parameter selection
int get_part(int part, const char* in);	// Obtain part of a fraction (passed as a string)

#endif
