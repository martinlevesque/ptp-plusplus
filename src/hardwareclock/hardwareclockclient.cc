/*
    Clock Synchronization Simulation in OMNet++
    Copyright (C) 2012 Jens Steinhauser (https://github.com/JenSte/omnet-ptp)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "hardwareclockclient.h"
#include "hardwareclock.h"
#include <exception>

#include <iostream>

using namespace std;

HardwareClockClient::HardwareClockClient()
	: _clock(NULL)
{
}

void HardwareClockClient::scheduleAtHWtime(const simtime_t& time, cMessage* msg)
{
	if (NULL == _clock)
		throw std::logic_error("HardwareClockClient::scheduleAtHWtime: no clock set");

	_clock->scheduleAtHWtime(time, msg, this);
}

void HardwareClockClient::scheduleAtInObject(const simtime_t realtime, cMessage* msg)
{
    cout << "ScheduleTestApp::scheduleAtInObject begin " << endl;

	Enter_Method_Silent();
	take(msg);

	scheduleAt(realtime, msg);
}

void HardwareClockClient::setHardwareClock(HardwareClock* clock)
{
    cout << "ScheduleTestApp::setHardwareClock begin " << endl;

	_clock = clock;
}
