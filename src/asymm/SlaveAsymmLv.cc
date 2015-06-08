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
#include "SlaveAsymmLv.h"
#include "SlaveAsymmAlgo.h"
#include "MasterAsymmLv.h"
#include "ptp.h"
#include "MACAddress.h"
#include "ptpslave.h"

class PtpSlave;

using namespace std;

SlaveAsymmLv::SlaveAsymmLv(cObject* p_app, UDPSocket* p_socket, int destPort, SoftwareClock* clock)
    : SlaveAsymmAlgo(p_app, p_socket, destPort, clock, 0)
{
}

void SlaveAsymmLv::handlePostProcessing(PtpPacket* ptp)
{
    MACAddress masterMAC; // dummy

    switch (ptp->getType())
    {
        case Ptp::Delay_Resp:
        {
            // Need to send Lee_Check_Req
            getSlave()->sendUpstream(Ptp::newGenericPtpPacket(masterMAC, Ptp::Lv_ReviseReq));
            break;
        }
        case Ptp::Lv_Explorer1:
        {
            LvTs3 = clock->getSWtime().second.dbl();
            LvTm3 = ptp->getTtx().dbl();

            // Need to resend at Delta
            getAppBase()->scheduleAt(simTime() + MasterAsymmLv::DeltaLv(), Ptp::newGenericPtpPacket(masterMAC, Ptp::Lv_Explorer1));

            break;
        }
        case Ptp::Lv_Explorer2:
        {
            LvTs4 = clock->getSWtime().second.dbl();
            LvTm4 = ptp->getTtx().dbl();

            // Need to resent at 2 * Delta
            getAppBase()->scheduleAt(simTime() + MasterAsymmLv::DeltaLv() * 2, Ptp::newGenericPtpPacket(masterMAC, Ptp::Lv_Explorer2));

            break;
        }
        case Ptp::Lv_ReviseResp:
        {
            LvTm5 = ptp->getTtx().dbl();
            LvTm6 = ptp->getTrx().dbl();

            double offsetLv = (LvTm6 - LvTs6)/(2.0) - (LvTs4 - LvTm4)/(2.0) - (LvTm5 - LvTs5) + (LvTs3-LvTm3);

            getSlave()->correct(offsetLv);

            break;
        }
        default:
        {
            getAppBase()->error("MasterAsymmLv::handlePtpMsg - case not handled");
            break;
        }
    }
}

void SlaveAsymmLv::handleSelfMsg(cMessage* msg)
{
    PtpPacket* pNext = dynamic_cast<PtpPacket*>(msg);

    if ( ! pNext)
    {
        getAppBase()->error("SlaveAsymmLv::handleSelfMsg err 1");
    }

    // if it is Lv Explorer 1/2, need to record the timestamp:
    if (pNext->getType() == Ptp::Lv_Explorer1)
    {
        LvTs5 = clock->getSWtime().second.dbl();
    }
    else
    if (pNext->getType() == Ptp::Lv_Explorer2)
    {
        LvTs6 = clock->getSWtime().second.dbl();
    }

    getSlave()->sendUpstream(pNext);
}
