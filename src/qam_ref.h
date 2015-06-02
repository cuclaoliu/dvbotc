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
//! Holds the real components of the mapped bit group for a given group for the different QAM levels
/**
 * To use this array the value of the symbol is used as the index of the array. 4QAM uses the first array, 16QAM the next and 64QAM the one after. In the case of the lower order QAM levels (lower than 64) the unused portions of the array are simply initialised to zero. This is used in the mapping module.
 */
int qam_re[][64]= {
	{1, 1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{3, 3, 1, 1, 3, 3, 1, 1, -3, -3, -1, -1, -3, -3, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 7, 5, 5, 7, 7, 5, 5, 1, 1, 3, 3, 1, 1, 3, 3, 7, 7, 5, 5, 7, 7, 5, 5, 1, 1, 3, 3, 1, 1, 3, 3, -7, -7, -5, -5, -7, -7, -5, -5, -1, -1, -3, -3, -1, -1, -3, -3, -7, -7, -5, -5, -7, -7, -5, -5, -1, -1, -3, -3, -1, -1, -3, -3}
};
//! Holds the imaginary components of the mapped bit group for a given group for the different QAM levels
/**
 * To use this array the value of the symbol is used as the index of the array. 4QAM uses the first array, 16QAM the next and 64QAM the one after. In the case of the lower order QAM levels (lower than 64) the unused portions of the array are simply initialised to zero. This is used in the mapping module.
 */
int qam_im[][64]= {
	{1, -1, 1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{3, 1, 3, 1, -3, -1, -3, -1, 3, 1, 3, 1, -3, -1, -3, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{7, 5, 7, 5, 1, 3, 1, 3, 7, 5, 7, 5, 1, 3, 1, 3, -7, -5, -7, -5, -1, -3, -1, -3, -7, -5, -7, -5, -1, -3, -1, -3, 7, 5, 7, 5, 1, 3, 1, 3, 7, 5, 7, 5, 1, 3, 1, 3, -7, -5, -7, -5, -1, -3, -1, -3, -7, -5, -7, -5, -1, -3, -1, -3}
};

