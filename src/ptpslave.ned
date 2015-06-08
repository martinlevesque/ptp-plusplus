//
//  ptp++
//  Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>
// 
//  Adapted from Jens Steinhauser https://github.com/JenSte/omnet-ptp
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

package ptp.src;

import inet.applications.IUDPApp;

simple PtpSlave like IUDPApp
{
	parameters:
		bool controllerEnabled = default(true);
		string delayRequestIntervalType = default("without"); // without, fixed, randomInt
		double delayRequestIntervalValue = default(0.0);
		
		int localPort = default(319);
		int destPort = default(319);
		
		bool joinLocalMulticastGroups = default(true);
		string destAddresses = default("");
		
		double DelayAsymmetryDms = default(0.0); // substracted to the D_{ms}
		
		string ptpPostAsymmAlgo = default(""); // "", "Lee" or "Lv"
		
		string nbPTPDuppToSend = default(""); // experimental
		bool withProbeClass = default(false); // experimental

	gates:
		input udpIn @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
}
