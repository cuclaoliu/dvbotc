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
#include<fftw3.h>
#include "dvbotc.h"
#include "ofdm_encode.h"
#include "dump.h"

// Variables for the FFT process
complex *in;			//!< Pointer to input processing array of complex time domain samples
complex *out;			//!< Pointer to output processing array of complex frequency domain samples
fftw_plan plan_backward;	//!< FFTW plan for use from one cycle to the next

//! Initialisation function for OFDM encoder.
/*!
  This function allocates memory for the IFFT used in the OFDM process. A detailed plan for the IFFT is also generated for multiple uses.
*/
void init_encode()
{
	in = (complex*) fftw_malloc(sizeof(complex) * DVBT_SETTINGS.DS_SL_FFT);
	out = (complex*) fftw_malloc(sizeof(complex) * DVBT_SETTINGS.DS_SL_FFT);
//	plan_backward = fftw_plan_dft_1d(DS_SL_FFT, in, out, FFTW_BACKWARD, FFTW_PATIENT);
}
//! Performs ofdm encoding using the FFTW library
/**
 * Perform the OFDM encoding based on the Matlab implmentation carried out in Opendvb. The process involves first copying the input data (used carriers) into the middle of a zero initialised total number of ofdm carriers. The equivilant of an FFTShift (in Matlab) is then performed to centre the output. A one dimensional backwards fft is then carried out on the data using the fftw3 library. The result is finally stored in an output array after having a portion of the end of the ift result added to the front of the result as a guard (the fraction determined by the guard interval parameter). The resulting output has been checked against Opendvb and, for an input of 5-superframes, there were a total of nine character level differences (output was in text file format), where the difference was one number in the decimal places (out of over 1070364 bytes). This represented an insignifficant difference and could be attributed to rounding differences.
 * @param data_in Complex time domain samples
 * @return Pointer to array of complex frequency domain samples with guarding (for transmission)
 */
complex* ofdm_encode(complex* data_in)
{
	int ii = 0;
	int m = DVBT_SETTINGS.DS_SL_ADC;
	int n = DVBT_SETTINGS.DS_SL_FFT;
	int n2 = 0;		// Used in fftshift
	complex tmp;		// Used in ftshift
	int g = DVBT_SETTINGS.DS_OFDM_GL;
	int k = DS_OFDM_CC;
	//printf("The n is %d\n", n);

	plan_backward = fftw_plan_dft_1d(DVBT_SETTINGS.DS_SL_FFT, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
	complex* result;

	// Copy input with zeros in the unused carrier locations
	for(ii = 0; ii< (n-k+1)/2; ii++)
	{
		in[ii] = 0;
	}
	for(ii = (n-k+1)/2; ii< (n+k+1)/2; ii++)
	{
		in[ii] = data_in[ii - (n-k+1)/2];
	}
	for(ii = (n+k+1)/2; ii< n; ii++)
	{
		in[ii] = 0;
	}

	// Perform FFTShift as per Matlab operator fftshift
	for (ii = 0; ii < n/2; ii++)
	{
		tmp = in[ii];
		in[ii] = in[ii+n/2];
		in[ii+n/2] = tmp;
	}

	// Procesing
	fftw_execute ( plan_backward );

	// Store result ith guard interval
	MALLOC(result, sizeof(complex) * DVBT_SETTINGS.DS_SL_ADC);
	for(ii = 0; ii< DVBT_SETTINGS.DS_OFDM_GL; ii++)
	{
		result[ii] = out[DVBT_SETTINGS.DS_SL_FFT - DVBT_SETTINGS.DS_OFDM_GL + ii]/n;
	}
	for(ii = 0; ii< DVBT_SETTINGS.DS_SL_FFT; ii++)
	{
		result[ii+DVBT_SETTINGS.DS_OFDM_GL] = out[ii]/n;
	}
	// Free this variable
	free(data_in);
	fftw_destroy_plan ( plan_backward );

	// Data logging
	if(dvbotc_options.debug_output_prefix_found == 1)
	{
		dvbotc_log_complex2(dump_refs.ofdm_encode, dump_refs.ofdm_encode_txt, result, DVBT_SETTINGS.DS_SL_ADC);
	}

	return result;
}

//! Cleanup function for OFDM encoder.
/*!
  This function deallocates memory used by the IFFT in the OFDM process. THe plan used by the IFFT is also destroyed by this function.
*/
void clean_encode()
{
//	fftw_destroy_plan ( plan_backward );
	fftw_free ( in );
	fftw_free ( out );
	return;
}

