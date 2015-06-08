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

#ifndef SCHEDULETESTAPP_H
#define SCHEDULETESTAPP_H

#include "hardwareclockclient.h"

/// Test application for the hardware clock implementation.
class ScheduleTestApp: public HardwareClockClient
{
	/// The number of pending test messages.
	int pending;

	struct ScheduleTestMsg: public cMessage
	{
		HardwareClock* clock;

		simtime_t time;
	};

protected:
	/// Handles the given message.
	virtual void handleMessage(cMessage *msg);

	int numInitStages() const;

	void initialize(int stage);

	void finish();
};

#endif
