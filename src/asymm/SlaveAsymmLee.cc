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
#include "SlaveAsymmLee.h"
#include "MasterAsymmLv.h"
#include "Ptp.h"
#include "MACAddress.h"
#include "ptpslave.h"

class PtpSlave;

using namespace std;

SlaveAsymmLee::SlaveAsymmLee(cObject* p_app, UDPSocket* p_socket, int destPort, SoftwareClock* clock, int nbPTPDuppToSend)
    : SlaveAsymmAlgo(p_app, p_socket, destPort, clock, nbPTPDuppToSend)
{
}

void SlaveAsymmLee::sendLeeCheckReq(const MACAddress& masterMAC)
{
    PtpPacket* p = Ptp::newGenericPtpPacket(masterMAC, Ptp::Lee_CheckReq);

    getSlave()->sendUpstream(p);
}

void SlaveAsymmLee::handlePostProcessing(PtpPacket* ptp)
{
    MACAddress masterMAC; // dummy

    switch (ptp->getType())
    {
        case Ptp::Delay_Resp:
        {
            // Need to send Lee_Check_Req
            sendLeeCheckReq(masterMAC);
            break;
        }
        case Ptp::Lee_LoopReqStart:
        {
            LeeTs3 = clock->getSWtime().second.dbl();

            realTs3 = simTime().dbl();
            LeeLoopStart = ptp->getTtx().dbl();

            this->getSlave()->sendClassProbe(this->getSlave()->chooseDestAddr());


            // Send back Loop resp start
            getSlave()->sendUpstream(Ptp::newGenericPtpPacket(masterMAC, Ptp::Lee_LoopRespStart));

            break;
        }
        case Ptp::Lee_LoopReqMiddle:
        {
            getSlave()->sendUpstream(Ptp::newGenericPtpPacket(masterMAC, Ptp::Lee_LoopRespMiddle));
            break;
        }
        case Ptp::Lee_LoopReqEnd:
        {
            LeeTs4 = clock->getSWtime().second.dbl();

            realTs4 = simTime().dbl();

            getSlave()->sendUpstream(Ptp::newGenericPtpPacket(masterMAC, Ptp::Lee_LoopRespEnd));
            break;
        }
        case Ptp::Lee_CheckResp:
        {
            double LeeTm3 = ptp->getTtx().dbl();
            double LeeTm4 = ptp->getTrx().dbl();

            // Process over with Lee
            double R = (LeeTm4 - LeeTm3) / (LeeTs4 - LeeTs3);

            double LeeOffset = getSlave()->timestamps.msDelay.dbl() - ((getSlave()->timestamps.msDelay + getSlave()->timestamps.smDelay).dbl()/(1.0 + R));

            getSlave()->correct(LeeOffset);

            break;
        }
        default:
        {
            getAppBase()->error("MasterAsymmLee::handlePtpMsg - case not handled");
            break;
        }
    }
}

void SlaveAsymmLee::handleSelfMsg(cMessage* msg)
{

}
