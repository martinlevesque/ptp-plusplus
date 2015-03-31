//
// ptp++
// Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>
// 
// Adapted from Jens Steinhauser https://github.com/JenSte/omnet-ptp
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

cplusplus {{
#include "simtime.h"
#include "ptp.h"
}};

class noncobject SimTime;
class noncobject Ptp::FrameType;

packet PtpPacket
{
	Ptp::FrameType type;

	SimTime ttx;
	SimTime trx;
	double levMinDelay;
	double levMeanDelay;

	SimTime realTtx;
	SimTime realTrx;
	
	string destIP;
}

packet PtpClassProbe // for class probe
{
}