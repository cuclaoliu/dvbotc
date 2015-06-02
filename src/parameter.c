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
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>
#include "dvbotc.h"

//! Parameter initialisation and verification
/**
 * Initialises and verifies DVB-T parameters based on those passed in the command line. Additional to the parameter loading function which is automatically generated using the helper program aargh.
 * @return Success(0), Failure(1)
 */
int parameter_init()
{
	// DVBT_SETTINGS.puncturing.mode = 2/3; // Note: changed to 1/2 for us
	DVBT_SETTINGS.DS_PUN_PM_num = get_part(0, dvbotc_options.puncturing_rate);
	DVBT_SETTINGS.DS_PUN_PM_den = get_part(1, dvbotc_options.puncturing_rate);

	// Mapper Settings
	// From global_settings.m, lines 140-170
	DVBT_SETTINGS.DS_MS_QAM = get_part(0, dvbotc_options.qam_level);	// DVBT_SETTINGS.map.qam_mode = 16;
	// The next parameter will change the distance between the constellations if this is implemented
	DVBT_SETTINGS.DS_MS_ALPHA = 1;						// DVBT_SETTINGS.map.alpha = 1;
	DVBT_SETTINGS.DS_MS_BITS = log2(DVBT_SETTINGS.DS_MS_QAM);
		// DVBT_SETTINGS.map.bits = log2 (DVBT_SETTINGS.map.qam_mode);

	// OFDM Symbol lengths
	// From global_settings.m, lines 216-226
	DVBT_SETTINGS.DS_SL_BITS = DS_OFDM_PC * DVBT_SETTINGS.DS_MS_BITS * 
					DVBT_SETTINGS.DS_PUN_PM_num / DVBT_SETTINGS.DS_PUN_PM_den;
		//DVBT_SETTINGS.symbol_length.bit_stream = ...
			//	DVBT_SETTINGS.payload_carriers*DVBT_SETTINGS.map.bits ...
			//	* DVBT_SETTINGS.puncturing.mode;
	DVBT_SETTINGS.DS_SL_II = DS_OFDM_PC * DVBT_SETTINGS.DS_MS_BITS;
		//DVBT_SETTINGS.symbol_length.inner_interleaver = ...
			//	DVBT_SETTINGS.payload_carriers*DVBT_SETTINGS.map.bits;
	DVBT_SETTINGS.DS_PU_OL = DVBT_SETTINGS.DS_PUN_PM_den*DVBT_SETTINGS.DS_SL_BITS/
					DVBT_SETTINGS.DS_PUN_PM_num;	// Number of bits resulting from puncturing

	// Insert Reference Signal Settings
	// From global_settings.m, line 178
	// This is actually implemented using the data table from page 27 of the DVB-T standard
	// DVBT_SETTINGS.refsig.alpha = 1/sqrt(10); % For 16QAM
	// The value to use the inverse square root of is stored to try to remove a floating point error
	// between opendvb and dvbotc to make comparisons easier
	if(DVBT_SETTINGS.DS_MS_QAM == 4)
	{
		// The alpha parameter is ignored for 4QAM
// 		DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(2);
		DVBT_SETTINGS.DS_RS_NORM = 2;
	}
	else if(DVBT_SETTINGS.DS_MS_QAM == 16)
	{
		switch( DVBT_SETTINGS.DS_MS_ALPHA )
		{
			case 1:
// 				DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(10);
				DVBT_SETTINGS.DS_RS_NORM = 10;
				break;
			case 2:
// 				DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(20);
				DVBT_SETTINGS.DS_RS_NORM = 20;
				break;
			case 4:
// 				DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(52);
				DVBT_SETTINGS.DS_RS_NORM = 52;
				break;
			// There is no default as there should always be a value
		}
	}
	else	// 64QAM is the only remaining mode
	{
		switch( DVBT_SETTINGS.DS_MS_ALPHA )
		{
			case 1:
// 				DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(42);
				DVBT_SETTINGS.DS_RS_NORM = 42;
				break;
			case 2:
// 				DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(60);
				DVBT_SETTINGS.DS_RS_NORM = 60;
				break;
			case 4:
// 				DVBT_SETTINGS.DS_RS_NORM = 1/sqrt(108);
				DVBT_SETTINGS.DS_RS_NORM = 108;
				break;
			// There is no default as there should always be a value
		}
	}
	// Cell Identifier
	// These two bytes come from command line entry
	DVBT_SETTINGS.DS_RS_CID1 = 0;
	DVBT_SETTINGS.DS_RS_CID2 = 0;
	// The format of the Cell ID is 0xXY where X is the first byte and Y is the second
	// Any other entry, if there is one, is invalid and would not have passed the input parameter check
	if(dvbotc_options.cell_id_found == 1)
	{
		// First byte
		switch(dvbotc_options.cell_id[2])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
					DVBT_SETTINGS.DS_RS_CID1 = dvbotc_options.cell_id[2] - 48;
					break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
					DVBT_SETTINGS.DS_RS_CID1 = dvbotc_options.cell_id[2] - 65 + 10;
					break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
					DVBT_SETTINGS.DS_RS_CID1 = dvbotc_options.cell_id[2] - 97 + 10;
					break;
		}
		// Second byte
		switch(dvbotc_options.cell_id[3])
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
					DVBT_SETTINGS.DS_RS_CID2 = dvbotc_options.cell_id[3] - 48;
					break;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
					DVBT_SETTINGS.DS_RS_CID2 = dvbotc_options.cell_id[3] - 65 + 10;
					break;
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
					DVBT_SETTINGS.DS_RS_CID2 = dvbotc_options.cell_id[3] - 97 + 10;
					break;
		}
	}

	// OFDM Encoding settings
	// From global_settings.m, lines 205-207
	DVBT_SETTINGS.DS_OFDM_IGI = get_part(1, dvbotc_options.guard_interval);
		//DVBT_SETTINGS.ofdm.guard_interval = 1/4;
			// The inverse is stored to keep it an integer
	DVBT_SETTINGS.DS_OFDM_GL = DS_OFDM_OM / DVBT_SETTINGS.DS_OFDM_IGI;
		//DVBT_SETTINGS.ofdm.guard_length = ...
			// DVBT_SETTINGS.ofdm.guard_interval * DVBT_SETTINGS.ofdm.mode;
	DVBT_SETTINGS.DS_SL_FFT = DS_OFDM_OM;	//DVBT_SETTINGS.symbol_length.fft = DVBT_SETTINGS.ofdm.mode;
	DVBT_SETTINGS.DS_SL_ADC = DS_OFDM_OM + DVBT_SETTINGS.DS_OFDM_GL;
		//DVBT_SETTINGS.symbol_length.ad_conv = ...
			//	DVBT_SETTINGS.ofdm.guard_length + DVBT_SETTINGS.symbol_length.fft;
	DVBT_SETTINGS.DS_CH_BW = get_part(0, dvbotc_options.chan_bw);
	return 0;
}

//! Validate guard interval
/**
 * Ensures that the guard interval entered as a command line option is valid and has been implemented.
 * @param  arg Guard interval string passed from command line
 * @return Success (0), Failure (1)
 */
int val_gi(const char* arg)
{
	if(strcmp(arg, "1/4") * strcmp(arg, "1/8") * strcmp(arg, "1/16") * strcmp(arg, "1/32")!=0)
	{
		fprintf(stderr, "Guard interval %s not a valid selection. Options: 1/4, 1/8, 1/16 or 1/32.\n", arg);
		return 1;
	}
	return 0;
}

//! Validate puncturing rate
/**
 * Ensures that the puncturing rate (aka Coding Rate) entered as a command line option is valid and has been implemented.
 * @param  arg Puncturing rate string passed from command line
 * @return Success (0), Failure (1)
 */
int val_pu(const char* arg)
{
	if(strcmp(arg, "3/4") * strcmp(arg, "5/6") * strcmp(arg, "7/8") == 0)
	{
		// Capture non-implemented modes. This may be deleted later.
		fprintf(stderr, "Puncturing mode %s not yet implemented.\n", arg);
		return 1;
	}
	if(strcmp(arg, "1/2") * strcmp(arg, "2/3") * strcmp(arg, "3/4")
			* strcmp(arg, "5/6") * strcmp(arg, "7/8")!=0)
	{
		fprintf(stderr, "Puncturing mode %s not a valid selection. Options: 1/2, 2/3, 3/4, 5/6 or 7/8.\n", arg);
		return 1;
	}
	return 0;
}

//! Validate QAM level
/**
 * Ensures that the QAM level entered as a command line option is valid and has been implemented.
 * @param  arg QAM level string passed from command line
 * @return Success (0), Failure (1)
 */
int val_qa(const char* arg)
{
/*	if(strcmp(arg, "64") == 0)
	{
		// Capture non-implemented modes. This may be deleted later.
		fprintf(stderr, "QAM level %s not yet implemented.\n", arg);
		return 1;
	}
*/
	if(strcmp(arg, "4") * strcmp(arg, "16") * strcmp(arg, "64")!=0)
	{
		fprintf(stderr, "QAM level %s not a valid selection. Options: 4, 16 or 64.\n", arg);
		return 1;
	}
	return 0;
}

//! Validate channel bandwidth (in MHz)
/**
 * Ensures that the channel bandwidth entered as a command line option is valid and has been implemented.
 * @param  arg Channel bandwidth string passed from command line
 * @return Success (0), Failure (1)
 */
int val_bw(const char* arg)
{
	if(strcmp(arg, "7") == 0 || strcmp(arg, "8") == 0)
	{
		// Capture non-implemented modes. This may be deleted later.
		fprintf(stderr, "Channel bandwidth %s not yet implemented.\n", arg);
		return 1;
	}
	if(strcmp(arg, "6") * strcmp(arg, "7") * strcmp(arg, "8")!=0)
	{
		fprintf(stderr, "Channel bandwidth %s not a valid selection. Options: 6, 7 or 8.\n", arg);
		return 1;
	}
	return 0;
}

//! Validate channel identifier
/**
 * Ensures that the channel identifier entered as a command line option is valid and has been implemented. It must be entered in the format 0xXY, where X and Y are the first and second bytes respectively. This is done using strict pattern checking code.
 * @param  arg Channel identifier parameter string passed from command line
 * @return Success (0), Failure (1)
 */
int val_id(const char* arg)
{
	if(arg[0] == '0')
	{
		if(arg[1] == 'x')
		{
			if(arg[2] >= '0' && arg[2] <= '9' || arg[2] >= 'A' && arg[2] <= 'F' || arg[2] >= 'a' && arg[2] <= 'f')
			{
				if(arg[3] >= '0' && arg[3] <= '9' || arg[3] >= 'A' && arg[3] <= 'F' || arg[3] >= 'a' && arg[3] <= 'f')
				{
					if(arg[4] == '\0')
					{
						// The input pattern is valid
						return 0;
					}
				}
			}
		}
	}
	// If this point is reached it means that the pattern of the channel identification byte pair entered
	// does not comply to the required format.
	fprintf(stderr, "Channel identifier %s does not conform to the required 0xXY format (where X and Y are hexadecimal numbers).\n", arg);
	return 1;
}

//! Get portion of option string from command line
/**
 * This function obtains part of a fractional argument entered by the user and returns it as a string. This is used, along with other entry functions, to get the DVB-T parameters from the user. It is a crude function that will only work when there is exactly one digit before the / and between 1 and 2 digits after the slash. This is not a problem, however, because the inputs possible are validated, it is not a problem in this case.
 * @param part Numberator (0) or denominator (1)
 * @param in Number containing required part
 * @return 
 */
int get_part(int part, const char* in)
{
	int temp = 0;
	int found = 0;
	int ii = 0;
	if(part == 1)
	{
		// Keep skipping until past the slash if required
		while(in[0]!=47)
		{
			in++;
		}
		in++;
	}
	while(in[0] != 0 && in[0] != 47)
	{
		temp = temp*10+(in[0] - 48);
		in++;
	}
	return temp;
}
