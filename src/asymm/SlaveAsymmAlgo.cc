/*
    ptp++
    Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>

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

#include <iostream>

#include "SlaveAsymmAlgo.h"
#include "ApplicationBase.h"
#include "UDPSocket.h"
#include "softwareclock.h"
#include "ptpslave.h"

using namespace std;

SlaveAsymmAlgo::SlaveAsymmAlgo(cObject* p_app, UDPSocket* p_socket, int destPort, SoftwareClock* clock, int nbPTPDuppToSend)
    : app(p_app), socket(p_socket), destPort(destPort), clock(clock), nbPTPDuppToSend(nbPTPDuppToSend)
{
}

void SlaveAsymmAlgo::handleSelfMsg(cMessage* msg)
{
}

ApplicationBase* SlaveAsymmAlgo::getAppBase()
{
    ApplicationBase* a = dynamic_cast<ApplicationBase*>(app);

    ASSERT(a != NULL);

    return a;
}

PtpSlave* SlaveAsymmAlgo::getSlave()
{
    PtpSlave* a = dynamic_cast<PtpSlave*>(app);

    ASSERT(a != NULL);

    return a;
}
