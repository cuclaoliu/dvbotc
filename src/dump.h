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
struct dump_refs
{
	// Main file pointers
	FILE* main;

	// 1. Scrambler
	FILE* scramble;
	FILE* scramble_txt;

	// 2. Reed-Solomon (rs)
	FILE* rs_encode;
	FILE* rs_encode_txt;

	// 3. Outer interleaver
	FILE* outer_interleave;
	FILE* outer_interleave_txt;

	// 4. Convolutional codec
	FILE* convolutional_encode;
	FILE* convolutional_encode_txt;

	// 5. Puncturing
	FILE* puncturing;
	FILE* puncturing_txt;

	// 6. Innner Interleaver
	FILE* inner_interleave;
	FILE* inner_interleave_txt;

	// 7. Mapper
	FILE* map;
	FILE* map_txt;

	// 8. Reference Signals
	FILE* insert_reference_signals;
	FILE* insert_reference_signals_txt;

	// 9. OFDM codec
	FILE* ofdm_encode;
	FILE* ofdm_encode_txt;
} dump_refs;

int dump_open ();
void dump_init ();
void dump_close ();
int my_fopen(FILE** ref, FILE** ref_txt, char* postfix, unsigned char mode);

