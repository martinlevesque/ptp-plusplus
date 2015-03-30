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
#include "SlaveAsymmLev.h"
#include "MasterAsymmLv.h"
#include "Ptp.h"
#include "MACAddress.h"
#include "ptpslave.h"

class PtpSlave;

using namespace std;

SlaveAsymmLev::SlaveAsymmLev(cObject* p_app, UDPSocket* p_socket, int destPort, SoftwareClock* clock, int nbPTPDuppToSend)
    : SlaveAsymmAlgo(p_app, p_socket, destPort, clock, nbPTPDuppToSend)
{
    previousMinDelayMS = -1;
    previousMinDelay = -1;
    previousR = -1;
}

void SlaveAsymmLev::handlePostProcessing(PtpPacket* ptp)
{
    MACAddress masterMAC; // dummy

    switch (ptp->getType())
    {
        case Ptp::Delay_Resp:
        {
            // Need to send Lee_Check_Req
            getSlave()->sendUpstream(Ptp::newGenericPtpPacket(masterMAC, Ptp::Lev_CheckReq));
            break;
        }
        case Ptp::Lev_LoopReqStart:
        {
            Ts3 = clock->getSWtime().second.dbl();

            realTs3 = simTime().dbl();
            LoopStart = ptp->getTtx().dbl();
            levPrevReceivedProb = clock->getSWtime().second.dbl();
            levDelays.clearResult();

            minRelDelay = (simTime().dbl()- LoopStart);

            break;
        }
        case Ptp::Lev_LoopReqMiddle:
        {
            double delay = clock->getSWtime().second.dbl() - levPrevReceivedProb;

            levDelays.collect(delay);

            levPrevReceivedProb = clock->getSWtime().second.dbl();

            break;
        }
        case Ptp::Lev_LoopReqEnd:
        {
            double delay = clock->getSWtime().second.dbl() - levPrevReceivedProb;

            levDelays.collect(delay);

            this->getSlave()->sendClassProbe(this->getSlave()->chooseDestAddr());

            levPrevReceivedProb = clock->getSWtime().second.dbl();

            levCurSizeProb = 1; // todo size

            PtpPacket* ptpResStart = Ptp::newGenericPtpPacket(masterMAC, Ptp::Lev_LoopRespStart);

            ptpResStart->setByteLength(levCurSizeProb);

            ptpResStart->setRealTtx(simTime().dbl());
            ptpResStart->setTtx(clock->getSWtime().second.dbl() );

            getSlave()->sendUpstream(ptpResStart);

            Ts4 = clock->getSWtime().second.dbl();

            realTs4 = simTime().dbl();

            PtpPacket* ptpResEnd = Ptp::newGenericPtpPacket(masterMAC, Ptp::Lev_LoopRespEnd);
            levCurSizeProb = 126;
            ptpResEnd->setByteLength(levCurSizeProb);
            getSlave()->sendUpstream(ptpResEnd);

            break;
        }
        case Ptp::Lev_CheckResp:
        {
            double Tm3 = ptp->getTtx().dbl();
            double Tm4 = ptp->getTrx().dbl();
            double levFixDelay = ptp->getLevMinDelay();
            double levFixDelayMS = levDelays.getMin();

            double weightMinPrevious = 0.9;

            if (previousMinDelayMS != -1)
            {
                levFixDelayMS = (1 - weightMinPrevious) * levFixDelayMS + weightMinPrevious * previousMinDelayMS;
            }

            previousMinDelayMS = levFixDelayMS;

            if (previousMinDelay != -1)
            {
                levFixDelay = (1 - weightMinPrevious) * levFixDelay + weightMinPrevious * previousMinDelay;
            }

            previousMinDelay = levFixDelay;

            double R = (Tm4 - Tm3 - ptp->getLevMinDelay() * nbPTPDuppToSend) / (Ts4 - Ts3 - levDelays.getMin() * nbPTPDuppToSend);

            if (previousR != -1)
            {
                R = (1 - weightMinPrevious) * R + weightMinPrevious * previousR;
            }

            previousR = R;

            double offset = getSlave()->timestamps.msDelay.dbl() - levFixDelayMS - ((getSlave()->timestamps.msDelay - levFixDelayMS + getSlave()->timestamps.smDelay - levFixDelay).dbl()/(2));

            getSlave()->correct(offset);

            break;
        }
        default:
        {
            getAppBase()->error("MasterAsymmLev::handlePtpMsg - case not handled");
            break;
        }
    }
}

void SlaveAsymmLev::handleSelfMsg(cMessage* msg)
{

}
