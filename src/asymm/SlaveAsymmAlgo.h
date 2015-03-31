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

#ifndef SLAVE_ASYMM_ALGO_H
#define SLAVE_ASYMM_ALGO_H

#include <omnetpp.h>
#include "ApplicationBase.h"
#include "UDPSocket.h"
#include "ptppacket_m.h"
#include "softwareclock.h"

class PtpSlave;

class SlaveAsymmAlgo
{
protected:
    SlaveAsymmAlgo(cObject* p_app, UDPSocket* p_socket, int destPort, SoftwareClock* clock, int nbPTPDuppToSend);

    cObject* app;
    UDPSocket* socket;
    int destPort;
    SoftwareClock* clock;
    int nbPTPDuppToSend;

public:
    virtual void handlePostProcessing(PtpPacket* ptp) = 0;
    virtual void handleSelfMsg(cMessage* msg);

    virtual ApplicationBase* getAppBase();
    virtual PtpSlave* getSlave();
};



#endif
