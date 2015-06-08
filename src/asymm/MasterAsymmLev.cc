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
#include "MasterAsymmLev.h"
#include "ptp.h"
#include "MACAddress.h"
#include <iomanip>
#include "ptpmaster.h"

using namespace std;

MasterAsymmLev::MasterAsymmLev(cObject* p_app, UDPSocket* p_socket, int destPort, int nbPTPDuppToSend)
    : MasterAsymmAlgo(p_app, p_socket, destPort), nbPTPDuppToSend(nbPTPDuppToSend)
{
}

void MasterAsymmLev::handlePtpMsg(PtpPacket* ptp)
{
    MACAddress slaveMAC; // dummy
    IPvXAddress slaveIP = Ptp::getSrcIP(ptp);

    switch (ptp->getType())
    {
        case Ptp::Lev_CheckReq:
        {
            this->getMaster()->sendClassProbe(slaveIP);

            // Loop Req start
            slaveInfos[slaveIP.get4().str()].levCurProbSize = 1; // todo size
            PtpPacket* pReqStart = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lev_LoopReqStart);
            pReqStart->setByteLength(slaveInfos[slaveIP.get4().str()].levCurProbSize);
            pReqStart->setTtx(simTime());
            socket->sendTo(pReqStart, slaveIP, destPort);

            // Loop Req middle
            const int nbLeeReqMiddle = nbPTPDuppToSend - 1;

            // Loop req end
            PtpPacket* pReqEnd = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lev_LoopReqEnd);
            slaveInfos[slaveIP.get4().str()].levCurProbSize = 126;
            pReqEnd->setByteLength(slaveInfos[slaveIP.get4().str()].levCurProbSize);
            socket->sendTo(pReqEnd, slaveIP, destPort);

            break;
        }
        case Ptp::Lev_LoopRespStart:
        {
            slaveInfos[slaveIP.get4().str()].LevLoopStart = ptp->getTtx().dbl();

            slaveInfos[slaveIP.get4().str()].levPrevProbReceivedAt = simTime().dbl();
            slaveInfos[slaveIP.get4().str()].levDelays.clearResult();

            slaveInfos[slaveIP.get4().str()].Tm3 = simTime().dbl();

            slaveInfos[slaveIP.get4().str()].minRelDelay = (simTime().dbl() - slaveInfos[slaveIP.get4().str()].LevLoopStart);

            break;
        }
        case Ptp::Lev_LoopRespMiddle:
        {
            double delay = simTime().dbl() - slaveInfos[slaveIP.get4().str()].levPrevProbReceivedAt;

            slaveInfos[slaveIP.get4().str()].levDelays.collect(delay);

            slaveInfos[slaveIP.get4().str()].levPrevProbReceivedAt = simTime().dbl();

            break;
        }
        case Ptp::Lev_LoopRespEnd:
        {
            slaveInfos[slaveIP.get4().str()].Tm4 = simTime().dbl();

            double delay = simTime().dbl() - slaveInfos[slaveIP.get4().str()].levPrevProbReceivedAt;
            slaveInfos[slaveIP.get4().str()].levDelays.collect(delay);

            slaveInfos[slaveIP.get4().str()].levPrevProbReceivedAt = simTime().dbl();

            PtpPacket* p = Ptp::newGenericPtpPacket(slaveMAC, Ptp::Lev_CheckResp);
            p->setTtx(slaveInfos[slaveIP.get4().str()].Tm3);
            p->setTrx(slaveInfos[slaveIP.get4().str()].Tm4);
            p->setLevMinDelay(slaveInfos[slaveIP.get4().str()].levDelays.getMin());
            p->setLevMeanDelay(slaveInfos[slaveIP.get4().str()].levDelays.getMean());
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

void MasterAsymmLev::handleSelfMsg(cMessage* msg)
{

}
