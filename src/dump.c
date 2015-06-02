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
#include "dump.h"
// Sourced from dump_open and dump_close files from open-dvb-0.4
// Open all dump files
/**
 * This function opens dump files for debugging of the system. It outputs the result of each stage of the DVB-T encoding and modulation proces.
 * @return the success (0) or failure (1) of the dump file opening function
 */
int dump_open ()
{
	// Generic files
	if(my_fopen(NULL, &dump_refs.main, "main", 2))
	{
		return 1;
	}

	// 1. Scrambler
	if(my_fopen(&dump_refs.scramble, &dump_refs.scramble_txt, "scramble", 3))
	{
		return 1;
	}

	// 2. Reed-Solomon (rs)
	if(my_fopen(&dump_refs.rs_encode, &dump_refs.rs_encode_txt, "rs_encode", 3))
	{
		return 1;
	}

	// 3. Outer interleaver
	if(my_fopen(&dump_refs.outer_interleave, &dump_refs.outer_interleave_txt, "outer_interleave", 3))
	{
		return 1;
	}

	// 4. Convolutional codec
	if(my_fopen(&dump_refs.convolutional_encode, &dump_refs.convolutional_encode_txt, "convolutional_encode", 3))
	{
		return 1;
	}

	// 5. Puncturing
	if(my_fopen(&dump_refs.puncturing, &dump_refs.puncturing_txt, "puncturing", 3))
	{
		return 1;
	}

	// 6. Innner Interleaver
	if(my_fopen(&dump_refs.inner_interleave, &dump_refs.inner_interleave_txt, "inner_interleave", 3))
	{
		return 1;
	}

	// 7. Mapper
	if(my_fopen(&dump_refs.map, &dump_refs.map_txt, "map", 3))
	{
		return 1;
	}

	// 8. Reference Signals
	if(my_fopen(&dump_refs.insert_reference_signals, &dump_refs.insert_reference_signals_txt, "insert_reference_signals", 3))
	{
		return 1;
	}

	// 9. OFDM codec
	if(my_fopen(&dump_refs.ofdm_encode, &dump_refs.ofdm_encode_txt, "ofdm_encode", 3))
	{
		return 1;
	}
	return 0;	// Files opened as required
}


/**
 * This function initialises the debugging dump functionality.
 */
void dump_init ()
{
	// Generic files
	dump_refs.main = NULL;

	// 1. Scrambler
	dump_refs.scramble = NULL;
	dump_refs.scramble_txt = NULL;

	// 2. Reed-Solomon (rs)
	dump_refs.rs_encode = NULL;
	dump_refs.rs_encode_txt = NULL;

	// 3. Outer interleaver
	dump_refs.outer_interleave = NULL;
	dump_refs.outer_interleave_txt = NULL;

	// 4. Convolutional codec
	dump_refs.convolutional_encode = NULL;
	dump_refs.convolutional_encode_txt = NULL;

	// 5. Puncturing
	dump_refs.puncturing = NULL;
	dump_refs.puncturing_txt = NULL;

	// 6. Innner Interleaver
	dump_refs.inner_interleave = NULL;
	dump_refs.inner_interleave_txt = NULL;

	// 7. Mapper
	dump_refs.map = NULL;
	dump_refs.map_txt = NULL;

	// 8. Reference Signals
	dump_refs.insert_reference_signals = NULL;
	dump_refs.insert_reference_signals_txt = NULL;

	// 9. OFDM codec
	dump_refs.ofdm_encode = NULL;
	dump_refs.ofdm_encode_txt = NULL;

	return;
}

// Close all dump files
/**
 * This function closes any open debugging dump files.
 */
void dump_close ()
{
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Closing File dump
	//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
	// Main file pointers
	if(dump_refs.main!=NULL)
	{
		fclose(dump_refs.main);
	}
	// 1. Scrambler
	if(dump_refs.scramble!=NULL)
	{
		fclose(dump_refs.scramble);
	}
	if(dump_refs.scramble_txt!=NULL)
	{
		fclose(dump_refs.scramble_txt);
	}

	// 2. Reed-Solomon (rs)
	if(dump_refs.rs_encode!=NULL)
	{
		fclose(dump_refs.rs_encode);
	}
	if(dump_refs.rs_encode_txt!=NULL)
	{
		fclose(dump_refs.rs_encode_txt);
	}

	// 3. Outer interleaver
	if(dump_refs.outer_interleave!=NULL)
	{
		fclose(dump_refs.outer_interleave);
	}
	if(dump_refs.outer_interleave_txt!=NULL)
	{
		fclose(dump_refs.outer_interleave_txt);
	}

	// 4. Convolutional codec
	if(dump_refs.convolutional_encode!=NULL)
	{
		fclose(dump_refs.convolutional_encode);
	}
	if(dump_refs.convolutional_encode_txt!=NULL)
	{
		fclose(dump_refs.convolutional_encode_txt);
	}

	// 5. Puncturing
	if(dump_refs.puncturing!=NULL)
	{
		fclose(dump_refs.puncturing);
	}
	if(dump_refs.puncturing_txt!=NULL)
	{
		fclose(dump_refs.puncturing_txt);
	}

	// 6. Innner Interleaver
	if(dump_refs.inner_interleave!=NULL)
	{
		fclose(dump_refs.inner_interleave);
	}
	if(dump_refs.inner_interleave_txt!=NULL)
	{
		fclose(dump_refs.inner_interleave_txt);
	}

	// 7. Mapper
	if(dump_refs.map!=NULL)
	{
		fclose(dump_refs.map);
	}
	if(dump_refs.map_txt!=NULL)
	{
		fclose(dump_refs.map_txt);
	}

	// 8. Reference Signals
	if(dump_refs.insert_reference_signals!=NULL)
	{
		fclose(dump_refs.insert_reference_signals);
	}
	if(dump_refs.insert_reference_signals_txt!=NULL)
	{
		fclose(dump_refs.insert_reference_signals_txt);
	}

	// 9. OFDM codec
	if(dump_refs.ofdm_encode!=NULL)
	{
		fclose(dump_refs.ofdm_encode);
	}
	if(dump_refs.ofdm_encode_txt!=NULL)
	{
		fclose(dump_refs.ofdm_encode_txt);
	}
}
/**
 * This function is a wraper for the fopen function that comes with C. It is used to make the code more concise. Both binary and text based files are opened (to enable easier debugging).
 * @param ref FILE Pointer for the binary dump file
 * @param ref_txt FILE Pointer for the text based dump file
 * @param postfix Postfix for the files to be opened
 * @param mode Specification of a text (bit 1) and/or binary (bit 2) file to be opened
 * @return the success (0) or failure (1) of the dump file opening function
 */
int my_fopen(FILE** ref, FILE** ref_txt, char* postfix, unsigned char mode)
{
	char fname[1000];
	char fname_txt[1000];
	strcpy(fname, dvbotc_options.debug_output_prefix);
	strcat(fname, "/");
	strcat(fname, postfix);
	strcpy(fname_txt, fname);
	strcat(fname, ".bin");
	strcat(fname_txt, ".txt");
	// Select the modes desired using a mask
	// 1: Binary dump file
	// 2: ASCII text dump file

	if(mode & 1)
	{
		*ref = fopen(fname, "wb");
		if(!ref)
			{return 1;}
	}
	if(mode & 2)
	{
		*ref_txt = fopen(fname_txt, "w");
		if(!ref_txt)
			{return 1;}
	}
	return 0;
}
