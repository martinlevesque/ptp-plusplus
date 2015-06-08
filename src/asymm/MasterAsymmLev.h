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

#ifndef MASTER_ASYMM_LEV_H
#define MASTER_ASYMM_LEV_H

#include <omnetpp.h>
#include "ptppacket_m.h"
#include "MasterAsymmAlgo.h"

struct LevSlaveInfos
{
    double Tm3, Tm4;
    double LevLoopStart;
    int levCurProbSize;
    double levPrevProbReceivedAt;
    cDoubleHistogram levDelays; //
    double minRelDelay;
};

class MasterAsymmLev: public MasterAsymmAlgo
{
protected:
    std::map<std::string, LevSlaveInfos> slaveInfos;
    int nbPTPDuppToSend;

public:
    MasterAsymmLev(cObject* p_app, UDPSocket* p_socket, int destPort, int nbPTPDuppToSend);

    void handlePtpMsg(PtpPacket* ptp);
    void handleSelfMsg(cMessage* msg);

};

#endif
