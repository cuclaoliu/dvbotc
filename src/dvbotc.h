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

#include "dvbotc_opts.h"	// Argument evaluator include file
#include "parameter.h"		// Parameter definition include file
struct DVBOTC dvbotc_options;			// Argument handling structure
struct dvbt_settings DVBT_SETTINGS;	// Parameter storage structure

// Memory allocation helper defines
#define oops(s) { perror((s)); exit(EXIT_FAILURE); }
#define MALLOC(s,t) if(((s) = malloc(t)) == NULL) { oops("error: malloc() "); }

// Global data structures
struct xymap
{
	unsigned char *x;
	unsigned char *y;
};
struct c_cxmap
{
	char *re;
	char *im;
};
struct f_cxmap
{
	float *re;
	float *im;
};


// Global functions implemented in dvbotc.c
void dvbotc_log(FILE* bFile, FILE* tFile, unsigned char* data, int size);
void dvbotc_log_f(FILE* bFile, FILE* tFile, float* data, int size);
void dvbotc_log_xy(FILE* bFile, FILE* tFile, struct xymap data, int size);
void dvbotc_log_c_cx(FILE* bFile, FILE* tFile, struct c_cxmap data, int size);
void dvbotc_log_f_cx(FILE* bFile, FILE* tFile, struct f_cxmap data, int size);
void print_help();

