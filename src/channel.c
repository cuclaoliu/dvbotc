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
#include "channel.h"
//! Channel data writing module.
/*!
  \param channel the pointer to the output file.
  \param chan_data the channel sample data, in complex form.
  \param size the number of samples in the channel
*/
void channel_write(FILE* channel, complex* chan_data, int size)
{
	int ii = 0;
	float temp[2];
	if(dvbotc_options.no_write ==0)
	{
// 		fwrite (chan_data, sizeof(complex), size, channel);
		for(ii = 0; ii<size; ii++)
		{
			temp[0] = creal(chan_data[ii]);
			temp[1] = cimag(chan_data[ii]);
			fwrite (&temp, sizeof(float), 2, channel);
		}
		/*for(ii = 0; ii<size; ii++)
		{
			fwrite (&chan_data[ii], sizeof(complex), 1, channel);
		}*/
	}
	free(chan_data);
	return;
}
