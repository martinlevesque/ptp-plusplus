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
#include "ApplicationBase.h"
#include "MasterAsymmLv.h"
#include "Ptp.h"
#include "MACAddress.h"

using namespace std;

MasterAsymmLv::MasterAsymmLv(cObject* p_app, UDPSocket* p_socket, int destPort)
    : MasterAsymmAlgo(p_app, p_socket, destPort)
{
}

void MasterAsymmLv::handlePtpMsg(PtpPacket* ptp)
{
    MACAddress slaveMAC; // dummy
    IPvXAddress slaveIP = Ptp::getSrcIP(ptp);

    switch (ptp->getType())
    {
        case Ptp::Lv_ReviseReq:
        {
            // It then sends Explorer 1 and 2
            PtpPacket* explorer1 = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lv_Explorer1);
            PtpPacket* explorer2 = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lv_Explorer2);

            explorer1->setDestIP(slaveIP.str().c_str());
            explorer2->setDestIP(slaveIP.str().c_str());

            this->getAppBase()->scheduleAt(simTime() + DeltaLv(), explorer1);
            this->getAppBase()->scheduleAt(simTime() + DeltaLv() * 2, explorer2);
            break;
        }
        case Ptp::Lv_Explorer1:
        {
            slaveInfos[slaveIP.get4().str()].LvTm5 = simTime().dbl();
            break;
        }
        case Ptp::Lv_Explorer2:
        {
            slaveInfos[slaveIP.get4().str()].LvTm6 = simTime().dbl();

            // To finish, send revise_resp
            PtpPacket* p = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lv_ReviseResp);
            p->setTtx(slaveInfos[slaveIP.get4().str()].LvTm5);
            p->setTrx(slaveInfos[slaveIP.get4().str()].LvTm6);
            socket->sendTo(p, slaveIP, destPort);
            break;
        }
        default:
        {
            this->getAppBase()->error("MasterAsymmLv::handlePtpMsg - case not handled");
            break;
        }
    }
}

void MasterAsymmLv::handleSelfMsg(cMessage* msg)
{
    PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg);

    if (ptp)
    {
        ///////////////////////
        // Lv asymm algo:
        if (ptp->getType() == Ptp::Lv_Explorer1 || ptp->getType() == Ptp::Lv_Explorer2)
        {
            ptp->setTtx(simTime());
            socket->sendTo(ptp, IPvXAddress(ptp->getDestIP()), destPort);//
        }
    }
}

double MasterAsymmLv::DeltaLv()
{
    return 0.0001;
}
