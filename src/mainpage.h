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

/** @mainpage Overview
*
* @authors Damien Phillip Morrissey
*
* @section intro Introduction
* DVB Open Transmission Core (dvbotc) is an implementation of the DVB-T standard ETSI 300 744v1.05. It is based on <A HREF="http://www.radionetworkprocessor.com/opendvb.html"> opendvb</A>, but has been heavily modified and re-written in the C language to be able to achieve the goal of Real Time encoding and modulation using standard hardware. This work was done as part of an undergraduate thesis at the <A HREF="http://www.elec.uow.edu.au"> University of Wollongong </A> in 2007.
@subsection Restrictions
The program does not cater for the following:
* - 8k Mode
* - TPS Error protection for for any mode except for the default
* - Puncturing modes except for 1/2 and 2/3
* The program has been tested against opendvb (an open implementation of the DVB-T standard in Octave which has been used in several successful projects according to its author) and <A HREF="http://dante.diei.unipg.it/~baruffa/dvbt/index.php">gbdvb</A> (a free, but closed source, implementation of the DVB-T standard), however no valid transmission has yet been achieved that could be received by a set top box. It is thought that the transmission methods attempted (a VGA card and a USRP) were either not suited or not properly implemented because the output of opendvb matches the output of dvbotc exactly (except for the TPS transmission, which is not performed by opendvb). The output of dvbtenco matches dvbotc exactly up to and including the insertion of reference signals, however differences in the way this output is converted to the time domain mean that there are differences in the IFFT output.\\
The fact that the outputs do not match at this point are not overly concerning as both opendvb and gbdvb are known to produce a viable signal, but both differ. Furthermore, the standard does not describe the IFFT process in the main body, rather in the Appendix (giving scope for implementation variations).
*
* <hr>
* @section notes Release Notes
* Please refer to the source for the release notes.
* <hr>
* @section requirements Requirements
* This software is known to run on just about any modern (post 2001 hardware) machine running a 2.6 kernel. Specifically, it was developed using a 1.71GHz Celeron M Laptop with 1GB RAM, however actual memory requirements are estimated to be much lower (almost insignifficant). Sufficient hard disk space for the output file is required (which can be in the order of multiple gigabytes for just a few minutes of data).
*/
