/*
    MasterAsymmAlgo: Experimental algorithms to mitigate asymmetry.
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

#include "MasterAsymmAlgo.h"
#include "ApplicationBase.h"
#include "UDPSocket.h"
#include "ptpmaster.h"

using namespace std;

MasterAsymmAlgo::MasterAsymmAlgo(cObject* p_app, UDPSocket* p_socket, int destPort)
    : app(p_app), socket(p_socket), destPort(destPort)
{
}

void MasterAsymmAlgo::handleSelfMsg(cMessage* msg)
{}

ApplicationBase* MasterAsymmAlgo::getAppBase()
{
    ApplicationBase* a = dynamic_cast<ApplicationBase*>(app);

    ASSERT(a != NULL);

    return a;
}

PtpMaster* MasterAsymmAlgo::getMaster()
{
    PtpMaster* a = dynamic_cast<PtpMaster*>(app);

    ASSERT(a != NULL);

    return a;
}
