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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include "dvbotc.h"
#include "scrambler.h"
#include "rs_wrapper.h"
#include "outer_interleave.h"
#include "convolution.h"
#include "puncturing.h"
#include "inner_interleaver.h"
#include "mapper.h"
#include "insert_reference_signals.h"
#include "ofdm_encode.h"
#include "channel.h"
#include "dump.h"

// The below line defines how many bytes to read from the input file- useful when
// debugging and the number of bytes changes
#define IFILE_BYTES DS_RS_K
//! Main dvbotc routine
/**
 * Encodes a valid MPEG Transport stream according to the DVB-T standard such that it is ready for transmission.
 * @param argc Count of input parameters
 * @param argv[] Array of pointers to the input parameters
 * @return Error code (if any)
 */
int main(int argc, char *argv[])
{
	// Obtain arguments for dvbotc
	// Close program if the parameters are incorrect
	if(get_dvbotc_opts(argc, argv, &dvbotc_options) == 1)
	{
		print_help();
		return 1;
	}
	
	// Glitch in options loader workaround
	//if(!dvbotc_options.debug_output_prefix)
	//	dvbotc_options.debug_output_prefix_found = 0;
	//dvbotc_options.debug_output_prefix_found = 0;

	FILE* input_file;		// Source file handle
	FILE* output_file;		// Output file handle
	unsigned char* data_in;		// Holds data while reading
	unsigned char* data_out;	// Holds data while completing output processing
	unsigned char* prbs_reg;	// Reference for prbs start
	unsigned char* oi_queue;	// Outer-interleaver queue pointer
	unsigned char* tmp;		// Used by helper programs
	unsigned char* tmp2;		// Used to pad data stream if needed
	unsigned char* bit_stream;	// Bit stream holder
	unsigned char conv_code[CONV_M];// Convolutional code storage variable
	struct xymap map_sig;		// XY Map variable
	complex* mapped_sig;		// Mapped data for reference insertion
	complex* sig_with_ref;		// Mapped signal with reference signals
	complex* chan_data;		// OFDM encoded data return variable

	int cnt = 0;			// Counter for packets processed
	int scnt = 0;			// Counter for symbols processed
	int fcnt = 0;			// Counter for frames processed
	int rcnt = 0;			// Read counter for processing
	int bs_size = 0;		// Bit stream size
	int zp_size = 0;		// Zero passed bit stream size- ensures all data is actualy sent
	int min_iterations = 55;	// Minimum number of iterations (for testing- opendvb may need this)
	int ii;				// Iterator variable
	int symb;			// Symbol iterator variable
	int num_symbols = 0;		// Current OFDM symbols to be processed

	// Display help information if this is what was requested
	if(dvbotc_options.help == 1)
	{
		print_help();
		return 0;
	}
	
	// Set up DVB-T parameters and verify integrity
	if(parameter_init() != 0)
	{
		fprintf(stderr, "DVB-T parameter initialization failure.\n");
		print_help();
		return 1;
	}

	// Open dump files if needed
	dump_init();
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		if(dump_open() == 1)
		{
			fprintf(stderr, "Debug output files in %s unable to be opened.\n", dvbotc_options.debug_output_prefix);
			dump_close();
			return 1;
		}
	}
	// Open the input file in binary mode
	//input_file = fopen ("/home/dpm95/ref/5-superframes/test.ts", "rb");
	//input_file = fopen ("/home/dpm95/ref/one_second_ref.ts", "rb");
	input_file = fopen (dvbotc_options.input_file, "rb");
	if(!input_file)
	{
		fprintf(stderr, "ERROR: File %s unable to be opened.\n", dvbotc_options.input_file);
		dump_close();
		return 1;
	}

	// Open output file for resulting modulated signal
	output_file = fopen (dvbotc_options.output, "wb");
	if(!output_file)
	{
		fprintf(stderr, "ERROR: File %s unable to be opened for output.\n", dvbotc_options.output);
		fclose(input_file);
		dump_close();
		return 1;
	}

	// Initialise any functions that require it
	// RS code library init
	/* 8 bit symbol size
	   gfpoly is a number where the 0th order if the lowest bit in this number
	   so we have x^8 + x^4 + x^3 + x^2 + 1 as our polynomial, so 100011101 (left is the most signifficant bit)
	   so it is 1+4+8+16+0+0+0+256=285 (which is 0x11d in hex)
	   using 0 and 1 for the next two parameters- they producde the correct output
	   16 is the number of parity bytes to be added */
	void * rs = (void*)init_rs_char(8, 0x11d, 0, 1, 16);
	// Ensure correctly initialised
	if(rs == NULL)
	{
		fprintf(stderr, "ERROR: Unable to initialise RS encoder.\n");
		fclose(input_file);
		fclose(output_file);
		dump_close();
		return 1;
	}
	conv_init(conv_code);
	init_encode();
	init_reference_signals();
	// Any required checks should now have been completed that would indicate if the
	// program was going to fail later or not so now it can safely be assumes that
	// cleaning later in the program will take place

	// Allocate memory for processing
	MALLOC(prbs_reg, 15);
	MALLOC(data_in, IFILE_BYTES);
	MALLOC(oi_queue, DS_OI_QS);
	memset(oi_queue, 0, DS_OI_QS);

	// Output parameters for information
	printf("=====================\nDVB-T Parameters\n=====================\nPuncturing:\t%d/%d\nQAM:\t\t%dQAM\nGuard Interval:\t1/%d\nBandwidth:\t%dMHz\n---------------------\n\n", DVBT_SETTINGS.DS_PUN_PM_num, DVBT_SETTINGS.DS_PUN_PM_den, DVBT_SETTINGS.DS_MS_QAM, DVBT_SETTINGS.DS_OFDM_IGI, DVBT_SETTINGS.DS_CH_BW);

	// Process data while information remains that can be processed
	while (!feof(input_file) || bs_size - zp_size > 0 || cnt<min_iterations)// || (bs_size > 0 && bs_size*8 / DS_SL_BITS > 0))
	{
		// Packet reading and bit stream loading
		if(!feof(input_file))
		{
			rcnt = fread(data_in, sizeof(char), DS_RS_K, input_file);
		}
		else
		{
			rcnt = 0;
			zp_size += 204;
		}

		for(ii = rcnt; ii<IFILE_BYTES;ii++)
		{
			data_in[ii] = 0;
		}
		if(rcnt == 0)
		{
			data_in[0] = DS_SC_SB;
		}
		// Perform Energy Dispersal
		scramble (data_in, cnt, prbs_reg);
		// Perform RS Encoding
		data_in = rs_encode_wrapper(data_in, rs);
		// Perform outer interleaving
		data_in = outer_interleave (data_in, oi_queue);

		// Note: The below bit stream is in bytes, NOT bits (as in open-dvb)
		//	 The size is also in bits as a result.
		// Add data to the Bit Stream
		if(bs_size == 0)
		{
			bit_stream = data_in;
			bs_size = 204;
			MALLOC(data_in, IFILE_BYTES);
		}
		else
		{
			MALLOC(tmp, bs_size + 204);
			memcpy(tmp,bit_stream, bs_size);
			memcpy(tmp + bs_size, data_in, 204);
			free(bit_stream);
			bit_stream = tmp;
			bs_size += 204;
			// Allocate the correct ammount of memory to data_in for
			// the next program cycle
			free(data_in);
			MALLOC(data_in, IFILE_BYTES);
		}
		cnt++;			// Increment packet counter and used to be DVBT_STATE_SENDER.n

		// Modulation and transmission
		if(bs_size > 0)
		{
			// Beginning of modulationdata_in
			// The size of the chunks worked on is defined in dvbotc.h
			num_symbols = bs_size*8 / DVBT_SETTINGS.DS_SL_BITS;	// Number of symbols to generate
								// By default this will act like a floor function
			for(symb = 0; symb < num_symbols; symb++)
			{
				// Process each symbol
				map_sig = conv_encode(bit_stream + symb*(DVBT_SETTINGS.DS_SL_BITS/8), conv_code);
				/* Performance Note:: we actually demultiplex the punctured signal in the
				   interleaver. If we use QPSK and 1/2 puncturing the demultiplexed
				   result will be the same as the x and y output from convolution
				   so, in theory, we should be able to skip puncturing
				   and the first part of interleaving in this case.
				   I have not implemented this yet, however. DM 2007-05-11.*/
				data_out = puncturing(map_sig);
				data_out = inner_interleave(data_out, scnt);
				mapped_sig = map(data_out);
				sig_with_ref = insert_reference_signals(mapped_sig, scnt + fcnt*DS_OFDM_SPF);
				chan_data = ofdm_encode(sig_with_ref);

				// Place data in the channel
				channel_write(output_file, chan_data, DVBT_SETTINGS.DS_SL_ADC);

				scnt++;	// Increment the number of symbols in frame used to be DVBT_STATE_SENDER.l
				if(scnt >= DS_OFDM_SPF)
				{
					// scnt should = DS_OFDM_SPF or we have an invalid frame state here
					scnt = 0;	// Reset counter
					// Increment frame count
					fcnt++;		// this used to be DVBT_STATE_SENDER.m
					if(fcnt >= DS_OFDM_FPSF)
					{
						// This is the completion of a superframe
						// fcnt should = DS_OFDM FPSF or there is an invalid frame state
						fcnt = 0;	// Reset frame count
						cnt = 0;	// Reset packet counter
						// If the bit stream is empty now there are invalid frame parameters
					}
				}
			}
			// Remove the processed bits from the stream
			bs_size -= num_symbols * DVBT_SETTINGS.DS_SL_BITS / 8;
			if(bs_size>0)
			{
				MALLOC(tmp, bs_size);
				memcpy(tmp, bit_stream + num_symbols * DVBT_SETTINGS.DS_SL_BITS / 8, bs_size);
				free(bit_stream);
				bit_stream = tmp;
			}
			else
			{
				free(bit_stream);
			}
		}
	}
	// Close dump files
	dump_close();

	// Program clean-up
	fclose(input_file);
	fclose(output_file);
	free_rs_char(rs);
	free(prbs_reg);
	clean_oi(oi_queue);
	free(data_in);
	conv_clean();
	clean_encode();
	if(bs_size != 0)
	{
		// Ensure that no memory is left allocated for the bit stream
		free(bit_stream);
	}
	return EXIT_SUCCESS;
}

//! Log character (byte) data
/**
 * Logs unsigned character number data in the form of 8 bits for each element. A tab is placed between the elements in the text file output.
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the unsigned character data to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log(FILE* bFile, FILE* tFile, unsigned char* data, int size)
{
	int ii;
	fwrite (data, sizeof(unsigned char), size, bFile);
	for(ii=0; ii<size; ii++)
	{
		fprintf(tFile, "%d", data[ii]);
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Log float data
/**
 * Logs floating point number data in the form of 32 bits for each element. A tab is placed between the elements in the text file output.
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the floating point data to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log_f(FILE* bFile, FILE* tFile, float* data, int size)
{
	int ii;
	fwrite (data, sizeof(float), size, bFile);
	for(ii=0; ii<size; ii++)
	{
		fprintf(tFile, "%f", data[ii]);
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Log xy map data (character/ byte map form)
/**
 * Logs xy map data in the form of 8 bits for the x and then 8 bits for the y data. A tab is placed between the x and y data in the text file output (the same delimiter as it between the elements).
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the complex data (with x and y members) to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log_xy(FILE* bFile, FILE* tFile, struct xymap data, int size)
{
	int ii,kk;
	unsigned char shifter;
	fwrite (data.x, 1, size, bFile);
	fwrite (data.y, 1, size, bFile);
	for(ii=0; ii<size; ii++)
	{
		fprintf(tFile, "%d\t", data.x[ii]);
	}
	for(ii=0; ii<size; ii++)
	{
		fprintf(tFile, "%d", data.y[ii]);
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Log complex data (character/ byte map form)
/**
 * Logs complex data in the form of 8 bits for the real and then 8 bits for the complex data. A colon is placed between the real and complex data in the text file output.
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the complex data (with real and imaginary members) to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log_c_cx(FILE* bFile, FILE* tFile, struct c_cxmap data, int size)
{
	int ii,kk;
	unsigned char shifter;
	for(ii=0; ii<size; ii++)
	{
		fwrite (data.re+ii, 1, 1, bFile);
		fwrite (data.im+ii, 1, 1, bFile);
		fprintf(tFile, "%d:%d", data.re[ii],data.im[ii]);
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Log complex data (complex map form)
/**
 * Logs complex data in the form of 32 bits for the real and then 32 bits for the complex data. A colon is placed between the real and complex data in the text file output.
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the complex data (with real and imaginary members) to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log_f_cx(FILE* bFile, FILE* tFile, struct f_cxmap data, int size)
{
	int ii,kk;
	unsigned char shifter;
	for(ii=0; ii<size; ii++)
	{
		fwrite (data.re+ii, sizeof(float), 1, bFile);
		fwrite (data.im+ii, sizeof(float), 1, bFile);
		fprintf(tFile, "%f:%f", data.re[ii],data.im[ii]);
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Log complex data (form 1)
/**
 * Logs complex data in the form of 32 bits for the real and then 32 bits for the complex data. A colon is placed between the real and complex data in the text file output.
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the complex data to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log_complex(FILE* bFile, FILE* tFile, complex* data, int size)
{
	int ii;
	float temp_r;
	float temp_i;
	//fwrite (data, sizeof(complex), size, bFile);
	for(ii=0; ii<size; ii++)
	{
		temp_r = creal(data[ii]);
		temp_i = cimag(data[ii]);
		fwrite (&temp_r, sizeof(float), 1, bFile);
		fwrite (&temp_i, sizeof(float), 1, bFile);
		fprintf(tFile, "%d:%d", (int)creal(data[ii]), (int)cimag(data[ii]));
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Log complex data (form 2)
/**
 * Logs complex data in the form of 32 bits for the real and then 32 bits for the complex data. A colon is placed between the real and complex data in the text file output.
 * @param bFile FILE pointer to the binary output file to write to
 * @param tFile FILE pointer to the text output file to write to
 * @param data Pointer to the complex data to be logged
 * @param size Number of elements pointed to by data to be logged
 */
void dvbotc_log_complex2(FILE* bFile, FILE* tFile, complex* data, int size)
{
	int ii;
	float temp_r;
	float temp_i;
	//fwrite (data, sizeof(complex), size, bFile);
	for(ii=0; ii<size; ii++)
	{
		temp_r = creal(data[ii]);
		temp_i = cimag(data[ii]);
		fwrite (&temp_r, sizeof(float), 1, bFile);
		fwrite (&temp_i, sizeof(float), 1, bFile);
		fprintf(tFile, "%f:%f", creal(data[ii]), cimag(data[ii]));
		if(ii < size - 1)
		{
			fprintf(tFile, "\t");
		}
		else
		{
			fprintf(tFile, "\n");
		}
	}	
	return;
}

//! Print usage and other help information
/**
 * Print usage and other help information if requested by the user or if there is a parameter failure. This should be updated whenever the parameter details are changed.
 */
void print_help()
{
	printf("=====================================\n");
	printf("dvbotc (DVB-T Open Transmission Core)\n");
	printf("DVB-T Compliant 2k Modulator\n");
	printf("Copyright (C) 2007 Damien Morrissey\n");
	printf("damien@damienandlaurel.com\n");
	printf("V0.5 Released under GPL, 2007-09-03\n");
	printf("=====================================\n");
	printf("Usage: dvbotc -i filename [-o filename -d directory -q QAM -p RATE -g GI -b BW -x]\n");
	printf("Options (defaults in [] where relevant): \n");
	printf("-i\t MPEG-2 transport stream filename (required)\n");
	printf("-o\t Output filename [dvbotc.bin]\n");
	printf("-d\t Debugging output directory\n");
	printf("-q\t QAM level (4, [16], 64)\n");
	printf("-p\t Puncturing rate (1/2, [2/3], 3/4, 5/6, 7/8)\n");
	printf("-g\t Guard interval ([1/4], 1/8, 1/16, 1/32)\n");
	printf("-b\t Channel bandwidth (MHz) ([6], 7, 8)\n");
	printf("-x\t Do not output result\n");
	printf("-c\t Channel ID (0x00 to 0xFF)\n");
	return;
}
