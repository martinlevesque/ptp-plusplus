/*
    ptp++
    Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>

    Adapted from Jens Steinhauser https://github.com/JenSte/omnet-ptp

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

#include "ptpslave.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include "softwareclock.h"
#include <Ieee802Ctrl_m.h>
#include <sstream>

#include <iostream>
#include "UDPControlInfo_m.h"
#include "IPvXAddressResolver.h"
#include <iomanip>
#include <sstream>
#include <fstream>
#include "MasterAsymmLv.h"
#include "SlaveAsymmAlgo.h"
#include "SlaveAsymmLv.h"
#include "SlaveAsymmLee.h"
#include "SlaveAsymmLev.h"

using namespace std;

Define_Module(PtpSlave);

PtpSlave::PtpSlave()
	: msDelay("master_slave_delay", 100)
	, smDelay("slave_master_delay", 100)
{
	controller.r[0] =  1;
	controller.r[1] = 12.5;
	controller.r[2] = 25;
}

void PtpSlave::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == 0)
    {
        clock = SoftwareClock::findFirstClock(getParentModule());
        EV << "using software clock with id " << clock->getId() << '\n';

        delayRequestIntervalType = par("delayRequestIntervalType").stringValue();
        delayRequestIntervalValue = par("delayRequestIntervalValue").doubleValue();
        DelayAsymmetryDms = par("DelayAsymmetryDms").doubleValue();
        previousOs = 0.0;

        localPort = par("localPort");
        destPort = par("destPort");
        withProbeClass = par("withProbeClass").boolValue();

        schedNextDelayRequest = new cMessage("schedNextDelayRequest");

        for (int i = 0; i < CONTROLLER_STAGES; i++) {
            std::stringstream sse;
            std::stringstream sso;

            sse << "e " << i;
            sso << "o " << i;

            errorVector[i].setName(sse.str().c_str());
            errorVector[i].setUnit("s");

            offsetVector[i].setName(sso.str().c_str());
            offsetVector[i].setUnit("s");
        }

        controllerEnabled = par("controllerEnabled");

        memset(controller.e, 0, sizeof(controller.e));
        controller.last = SimTime();

        // this assumes that the packet delays are
        // between 1 and 100 micro seconds
        msDelay.setRange(1e-6, 100e-6);
        smDelay.setRange(1e-6, 100e-6);

        msDelayVector.setName("master_slave_delay");
        msDelayVector.setUnit("s");
        smDelayVector.setName("slave_master_delay");
        smDelayVector.setUnit("s");

        msDelayRealVector.setName("real master_slave_delay");
        msDelayRealVector.setUnit("s");
        smDelayRealVector.setName("real slave_master_delay");
        smDelayRealVector.setUnit("s");

        schedProcessStart = new cMessage("schedProcessStart");
        scheduleAt(0.0000000001, schedProcessStart);
        //processStart();

        ptpPostAsymmAlgo = par("ptpPostAsymmAlgo").stringValue();

        nbPTPDuppToSend = 0;

        if (string(par("nbPTPDuppToSend").stringValue()) == "StatsCollector.nbPTPDuppToSend")
        {
            StatsCollector* s = StatsCollector::findStatsCollector(getParentModule());

            if ( ! s)
            {
                error("PtpSlave err 2 extra upstream delay init");
            }
            else
            {
                nbPTPDuppToSend = s->nbPTPDuppToSend;
            }
        }

        asymmAlgo = NULL;

        if (ptpPostAsymmAlgo == "Lv")
        {
            asymmAlgo = new SlaveAsymmLv(this, &socket, destPort, clock);
        }
        else
        if (ptpPostAsymmAlgo == "Lee")
        {
            asymmAlgo = new SlaveAsymmLee(this, &socket, destPort, clock, nbPTPDuppToSend);
        }
        else
        if (ptpPostAsymmAlgo == "Lev")
        {
            asymmAlgo = new SlaveAsymmLev(this, &socket, destPort, clock, nbPTPDuppToSend);
        }
    }
}

void PtpSlave::setSocketOptions()
{
    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");

    if (joinLocalMulticastGroups)
    {
        socket.joinLocalMulticastGroups();
        socketProbe.joinLocalMulticastGroups();
    }
}

void PtpSlave::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
    socketProbe.setOutputGate(gate("udpOut"));
    socketProbe.bind(PTP_CLASS_PROBE_PORT);
    setSocketOptions();

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != NULL)
    {
        IPvXAddress result;
        IPvXAddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV << "cannot resolve destination address: " << token << endl;
        else
        {
            destAddresses.push_back(result);
        }
    }
}

IPvXAddress PtpSlave::chooseDestAddr()
{
    int k = intrand(destAddresses.size());
    if (destAddresses[k].isIPv6() && destAddresses[k].get6().isLinkLocal()) // KLUDGE for IPv6
    {
        const char *destAddrs = par("destAddresses");
        cStringTokenizer tokenizer(destAddrs);
        const char *token;

        for (int i = 0; i <= k; ++i)
            token = tokenizer.nextToken();
        destAddresses[k] = IPvXAddressResolver().resolve(token);
    }
    return destAddresses[k];
}

void PtpSlave::handleMessageWhenUp(cMessage *msg)
{
     if (msg->getKind() == UDP_I_DATA)
    {
        // process incoming packet
        processPacket(PK(msg));
    }
    else if (msg->getKind() == UDP_I_ERROR)
    {
        EV << "Ignoring UDP error report\n";
        delete msg;
    }
    else
    {
        error("Unrecognized message (%s)%s", msg->getClassName(), msg->getName());
    }
}

void PtpSlave::processPacket(cPacket *pk)
{
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    handleMessage(pk);
}

void PtpSlave::finish()
{
	msDelay.recordAs("master_slave_delay", "s");
	smDelay.recordAs("slave_master_delay", "s");
	ApplicationBase::finish();

}

void PtpSlave::sendClassProbe(IPvXAddress ip)
{
    // class probe TODO
    if (withProbeClass)
    {
        PtpClassProbe* classProbe = new PtpClassProbe("PtpClassProbe");
        classProbe->setByteLength(1400);
        socketProbe.sendTo(classProbe, ip, PTP_CLASS_PROBE_PORT);
    }
    // class probe TODO
}

void PtpSlave::sendDelayReq(const MACAddress& masterMAC)
{
    sendClassProbe(this->chooseDestAddr());

    PtpPacket* p = Ptp::newDelayReqPacket(masterMAC);

    // gets overwritten by the phy when hardware
    // timestamping is used

    pair<SimTime, SimTime> ts = clock->getSWtime();

    p->setTtx(ts.second); // corrected one
    StdTs3 = ts.second.dbl();
    realTs3 = simTime().dbl();

    // send(p, "port$o");

    sendUpstream(p);
}

void PtpSlave::correct(double modifiedOffset)
{
	double offset;

	if (modifiedOffset != -1)
	{
	    offset = modifiedOffset;
	}
	else
	{
	    offset = ( timestamps.msDelay - DelayAsymmetryDms - timestamps.smDelay ).dbl() / 2.0;
	}

	double os = 0.0;

	// With controller
	if (controllerEnabled)
	{
        //
        // controller
        //

        double Ta = (simTime() - controller.last).dbl();
        controller.last = simTime();

        // offset sum

        for (int i = CONTROLLER_STAGES - 1; i >= 0; i--) {
            if (0 == i)
                controller.e[0]  = -offset;
            else
                controller.e[i] += controller.e[i-1] * Ta;

            errorVector[i].record(controller.e[i]);

            double o = controller.e[i] * controller.r[i];
            offsetVector[i].record(o);

            os += o;
        }
	}
	else // No controller
	{
	    os = -offset + previousOs;
	    previousOs = os;
	}

	clock->setFactor(os);
}

void PtpSlave::processDelayRequestIntervalType()
{
    if (delayRequestIntervalType == "without")
    {
        sendDelayReq(masterMAC);
    }
    else
    if (delayRequestIntervalType == "fixed" || delayRequestIntervalType == "randomInt")
    {
        // We need to check if there should be some delay between DelayReq messages:
        // Then we need to schedule !
        double sendAt = simTime().dbl();

        if (delayRequestIntervalType == "fixed")
        {
            sendAt += delayRequestIntervalValue;
        }
        else
        if (delayRequestIntervalType == "randomInt")
        {
            sendAt += uniform(0, delayRequestIntervalValue);
        }

        scheduleAt(sendAt, schedNextDelayRequest);
    }
}


void PtpSlave::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage())
    {
        if (dynamic_cast<PtpPacket*>(msg))
        {
            // lv
            if (asymmAlgo)
            {
                asymmAlgo->handleSelfMsg(msg);
            }
        }
        else
        if (msg == schedProcessStart)
        {
            processStart();
        }
        else
        if (msg == schedNextDelayRequest)
        {
            // need to send req
            sendDelayReq(masterMAC);
        }

        return; // do NOT delete
    }
    else
	if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg))
	{

		switch (ptp->getType())
		{

            case Ptp::Sync:
            {
                // if there is no hardware timestamp by
                // the phy, use a software timestamp
                if (0 == ptp->getTrx().raw())
                {
                    pair<SimTime, SimTime> ts = clock->getSWtime();
                    ptp->setTrx(ts.second);
                }

                timestamps.msDelay = ptp->getTrx() - ptp->getTtx();
                StdTm1 = ptp->getTtx().dbl();
                StdTs1 = ptp->getTrx().dbl();

                realTs1 = simTime().dbl();

                clock->addSyntoPoint(ptp->getTtx().dbl(), ptp->getTrx().dbl());

                msDelay.collect(timestamps.msDelay);
                msDelayVector.record(timestamps.msDelay);

                msDelayRealVector.record(ptp->getRealTrx() - ptp->getRealTtx());

                processDelayRequestIntervalType();

                break;
            }
            case Ptp::Delay_Resp:
            {
                timestamps.smDelay = ptp->getTrx() - StdTs3;
                smDelay.collect(timestamps.smDelay);
                smDelayVector.record(timestamps.smDelay);

                smDelayRealVector.record(ptp->getRealTrx() - ptp->getRealTtx());

                StdTm3 = ptp->getTtx().dbl();
                StdTm4 = ptp->getTrx().dbl();

                if (ptpPostAsymmAlgo == "")
                {
                    correct(-1);
                }
                else
                {
                    handlePostProcessing(ptp);
                }

                break;
            }
            default:
            {
                handlePostProcessing(ptp);
                break;
            }
		}
	}
	else
    if (dynamic_cast<PtpClassProbe*>(msg))
    {
        // received
    }
	else
	{
	    error("PtpSlave::handleMessage - unhandled msg !");
	}

	delete msg;
}

void PtpSlave::handlePostProcessing(PtpPacket* ptp)
{
    if (asymmAlgo)
        asymmAlgo->handlePostProcessing(ptp);
}

void PtpSlave::sendUpstream(PtpPacket* p)
{
    //if (extraUpstreamDelay <= 0.0)
    //{
        socket.sendTo(p, chooseDestAddr(), destPort);
    //}
    //else
    //{
    //    upstreamTX.insert(p);

    //    if (msgUpstreamTX->isScheduled())
    //    {
            // Nothing.
    //    }
    //    else
    //    {
    //        scheduleAt(simTime() + extraUpstreamDelay, msgUpstreamTX);
    //    }
    //}
}
