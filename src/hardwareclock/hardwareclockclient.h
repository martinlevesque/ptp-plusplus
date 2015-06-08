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

#ifndef HARDWARECLOCKCLIENT_H
#define HARDWARECLOCKCLIENT_H

//#include "hardwareclock.h"
#include <omnetpp.h>

class HardwareClock;

/// \brief Base class for classes that want to schedule events at hardware time.
class HardwareClockClient: public cSimpleModule
{
	/// Hardware clock to use in 'scheduleAtHWtime()'.
	HardwareClock* _clock;

	/// Calls 'scheduleAt' in the context of the HardwareClockClient object
	/// (calls Enter_Method_Silent()).
	///
	/// \param realtime	The simulation time when to schedule the message.
	/// \param msg	The message to schedule. The ownership of the message is
	/// 		take()n in the function.
	void scheduleAtInObject(const simtime_t realtime, cMessage* msg);

protected:
	/// Sets the hardware clock to use in 'scheduleAtHWtime()'.
	void setHardwareClock(HardwareClock* clock);

	/// Schedules a message at hardware time.
	///
	/// The hardware clock whose time is used has to be set with
	/// 'setHardwareClock()' prior to calling this function.
	/// Because the message is scheduled internally through the
	/// objects 'scheduleAt' function, it's really a self message
	/// (and msg->isSelfMessage() can be used).
	///
	/// \param hwtime	The hardware time when to schedule the message.
	/// \param msg	The message to schedule.
	void scheduleAtHWtime(const simtime_t& hwtime, cMessage* msg);

public:
	HardwareClockClient();

	// needs access to 'scheduleAtInObject'
	friend class HardwareClock;
};

#endif
