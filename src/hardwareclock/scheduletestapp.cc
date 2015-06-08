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

#include "scheduletestapp.h"
#include "hardwareclock.h"

#include <iostream>

using namespace std;

Define_Module(ScheduleTestApp);

int ScheduleTestApp::numInitStages() const
{
    cout << "ScheduleTestApp::numInitStages begin "  << endl;
	return 2;
}

void ScheduleTestApp::initialize(int stage)
{
    cout << "ScheduleTestApp::initialize begin " << getFullPath() << endl;

	simtime_t times[] = {
		SimTime::parse("55.503ms"),
		SimTime::parse("10.3s"),
		SimTime::parse("100.000000004s"),
		SimTime::parse("400s")
	};

	if (1 == stage) {
		// the initialization is done in
		// stage 1, because in stage 0 the
		// clocks aren't ready yet

		pending = 0;

		std::vector<HardwareClock*> clocks = HardwareClock::findClocks(getParentModule());

		std::vector<HardwareClock*>::iterator it = clocks.begin();
		while (it != clocks.end()) {
			//
			// schedule some self messages on every clock
			//
			for (size_t i = 0; i < sizeof(times) / sizeof(simtime_t); i++) {
				ScheduleTestMsg* msg = new ScheduleTestMsg();
				msg->setName("ScheduleTestMsg");
				msg->clock = *it;
				msg->time = times[i];

				setHardwareClock(*it);
				scheduleAtHWtime(times[i], msg);

				pending++;
			}

			it++;
		}

		EV << clocks.size() << " clock(s) found, " << pending << " messages scheduled \n";
	}

	cout << "ScheduleTestApp::initialize after " << getFullPath() << endl;
}

std::string extend(std::string s)
{
    cout << "ScheduleTestApp::extend begin " << endl;

	s.resize(17, ' ');
	return s;
}

void ScheduleTestApp::handleMessage(cMessage *msg)
{
    cout << "ScheduleTestApp::handleMessage begin " << endl;

	if (msg->isSelfMessage()) {
		ScheduleTestMsg* m = check_and_cast<ScheduleTestMsg*>(msg);

		pending--;

		EV << "HW time scheduled message received from clock '" << m->clock->getName() << "'\n";
		EV << "requested arrival time: " << extend(m->time.str())
			<< "sec\n";
		EV << "HW time of the clock:   " << extend(m->clock->getHWtime().str())
			<< "sec   (should be equal or very close to the above timestamp)\n";
		EV << "simulation time:        " << extend(simTime().str())
			<< "sec   (doesn't need to be equal depending on the clock drift)\n";

		delete m;
	}

	cout << "ScheduleTestApp::handleMessage end " << endl;
}

void ScheduleTestApp::finish()
{
    cout << "ScheduleTestApp::finish begin " << endl;

	if (0 == pending)
		EV << "TestApp: looks good, got all test messages\n";
	else
		EV << "TestApp: hey, " << pending << " test messages are still pending!\n";
}
