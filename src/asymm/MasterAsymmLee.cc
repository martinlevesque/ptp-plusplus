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
#include "MasterAsymmLee.h"
#include "Ptp.h"
#include "MACAddress.h"
#include "ptpmaster.h"

using namespace std;

MasterAsymmLee::MasterAsymmLee(cObject* p_app, UDPSocket* p_socket, int destPort, int nbPTPDuppToSend)
    : MasterAsymmAlgo(p_app, p_socket, destPort), nbPTPDuppToSend(nbPTPDuppToSend)
{
}

void MasterAsymmLee::handlePtpMsg(PtpPacket* ptp)
{
    MACAddress slaveMAC; // dummy
    IPvXAddress slaveIP = Ptp::getSrcIP(ptp);

    switch (ptp->getType())
    {
        case Ptp::Lee_CheckReq:
        {
            this->getMaster()->sendClassProbe(slaveIP);

            // Loop Req start
            PtpPacket* pReqStart = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lee_LoopReqStart);
            pReqStart->setTtx(simTime());
            socket->sendTo(pReqStart, slaveIP, destPort);

            // Loop Req middle
            const int nbLeeReqMiddle = nbPTPDuppToSend - 1;

            for (int i = 0; i < nbLeeReqMiddle; ++i)
            {
                PtpPacket* pReqMiddle = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lee_LoopReqMiddle);
                socket->sendTo(pReqMiddle, slaveIP, destPort);
            }

            // Loop req end
            PtpPacket* pReqEnd = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lee_LoopReqEnd);
            socket->sendTo(pReqEnd, slaveIP, destPort);

            break;
        }
        case Ptp::Lee_LoopRespStart:
        {
            slaveInfos[slaveIP.get4().str()].LeeTm3 = simTime().dbl();
            break;
        }
        case Ptp::Lee_LoopRespMiddle:
        {
            break;
        }
        case Ptp::Lee_LoopRespEnd:
        {
            slaveInfos[slaveIP.get4().str()].LeeTm4 = simTime().dbl();

            PtpPacket* p = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lee_CheckResp);
            p->setTtx(slaveInfos[slaveIP.get4().str()].LeeTm3);
            p->setTrx(slaveInfos[slaveIP.get4().str()].LeeTm4);
            socket->sendTo(p, slaveIP, destPort);
            break;
        }
        default:
        {
            this->getAppBase()->error("MasterAsymmLee::handlePtpMsg - case not handled");
            break;
        }
    }
}

void MasterAsymmLee::handleSelfMsg(cMessage* msg)
{

}
