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

#include "ptpmaster.h"
#include "ptp.h"
#include "ptppacket_m.h"
#include <Ieee802Ctrl_m.h>
#include "IPvXAddressResolver.h"
#include "UDPControlInfo_m.h"
#include "StatsCollector.h"
#include <iomanip>
#include <iostream>
#include "MasterAsymmLv.h"
#include "MasterAsymmLee.h"
#include "MasterAsymmLev.h"

using namespace std;

Define_Module(PtpMaster);

PtpMaster::PtpMaster()
	: selfMsg(NULL)
{
}

void PtpMaster::sendClassProbe(IPvXAddress ip)
{
    // probe to improve TODO
    if (withProbeClass)
    {
        PtpClassProbe* classProbe = new PtpClassProbe("PtpClassProbe");
        classProbe->setByteLength(1400);
        socketProbe.sendTo(classProbe, ip, PTP_CLASS_PROBE_PORT);
    }
    // probe to improve TODO
}

void PtpMaster::sendSync()
{
    sendClassProbe(chooseDestAddr());

    PtpPacket* p = Ptp::newSyncPacket(MACAddress::BROADCAST_ADDRESS);

    // p->ttx is not set, the phy does that for us
    p->setTtx(simTime());

    // send(p, "port$o");
    socket.sendTo(p, chooseDestAddr(), destPort);
}

void PtpMaster::processDelayReq(PtpPacket* delayReq)
{
    IPvXAddress slaveIP = Ptp::getSrcIP(delayReq);

    MACAddress slaveMAC;// = c->getSrc();

    PtpPacket* p = Ptp::newDelayRespPacket(slaveMAC);

    // just send back the timestamps
    // (the timestamping phy doesn't overwrite timestamps
    // in a Delay_Resp packet)

    if (0 == delayReq->getTrx().raw())
    {
        SimTime s = simTime();
        delayReq->setTrx(s);
    }

    p->setTtx(delayReq->getTtx());
    p->setTrx(delayReq->getTrx());
    slaveInfos[slaveIP.get4().str()].StdTm3 = p->getTrx().dbl();
    p->setRealTtx(delayReq->getRealTtx());
    p->setRealTrx(delayReq->getRealTrx());

    socket.sendTo(p, slaveIP, destPort);
}

void PtpMaster::initialize(int stage)
{
    ApplicationBase::initialize(stage);

    if (stage == 0)
    {
        if (NULL == selfMsg)
            selfMsg = new cMessage("ptpMaster self message");

        localPort = par("localPort");
        destPort = par("destPort");
        withProbeClass = par("withProbeClass").boolValue();

        syncInterval = SimTime::parse("1s") / (int)par("syncRate");

        scheduleAt(syncInterval, selfMsg);
        processStarted = false;

        ptpPostAsymmAlgo = par("ptpPostAsymmAlgo").stringValue();

        if (ptpPostAsymmAlgo != "" && ptpPostAsymmAlgo != "Lee" && ptpPostAsymmAlgo != "Lv" && ptpPostAsymmAlgo != "Lev")
        {
            error("PtpMaster post asymm algo error");
        }

        nbPTPDuppToSend = 0;

        if (string(par("nbPTPDuppToSend").stringValue()) == "StatsCollector.nbPTPDuppToSend")
        {
            StatsCollector* s = StatsCollector::findStatsCollector(getParentModule());

            if ( ! s)
            {
                error("PtpMaster err 2 extra upstream delay init");
            }
            else
            {
                nbPTPDuppToSend = s->nbPTPDuppToSend;
            }
        }

        asymmAlgo = NULL;

        if (ptpPostAsymmAlgo == "Lv")
        {
            asymmAlgo = new MasterAsymmLv(this, &socket, destPort);
        }
        else
        if (ptpPostAsymmAlgo == "Lee")
        {
            asymmAlgo = new MasterAsymmLee(this, &socket, destPort, nbPTPDuppToSend);
        }
        else
        if (ptpPostAsymmAlgo == "Lev")
        {
            asymmAlgo = new MasterAsymmLev(this, &socket, destPort, nbPTPDuppToSend);
        }
    }
}

void PtpMaster::setSocketOptions()
{
    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");

    if (joinLocalMulticastGroups)
    {
        socket.joinLocalMulticastGroups();
        socketProbe.joinLocalMulticastGroups();
    }
}

void PtpMaster::processStart()
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

void PtpMaster::handleMessageWhenUp(cMessage *msg)
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

void PtpMaster::processPacket(cPacket *pk)
{
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    handleMessage(pk);
}



void PtpMaster::handleMessage(cMessage* msg)
{
    if (msg->isSelfMessage())
    {

        if (selfMsg == msg)
        {
            if ( ! processStarted)
            {
                processStart();
                processStarted = true;
            }

            sendSync();

            scheduleAt(simTime() + syncInterval, msg);
        }
        else
        {
            if (asymmAlgo)
                asymmAlgo->handleSelfMsg(msg);
        }

        return;
    }
    else
    {
        if (PtpPacket* ptp = dynamic_cast<PtpPacket*>(msg))
        {
            switch (ptp->getType())
            {
                case Ptp::Delay_Req:
                {
                    processDelayReq(ptp);
                    break;
                }
                default:
                {
                    if (asymmAlgo != NULL)
                        asymmAlgo->handlePtpMsg(ptp);

                    break;
                }
            }
        }
        else
        if (PtpClassProbe* p = dynamic_cast<PtpClassProbe*>(msg))
        {
            //cout << "received probe at master " << endl;
        }

        delete msg;
    }
}

IPvXAddress PtpMaster::chooseDestAddr()
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

void PtpMaster::finish()
{
    ApplicationBase::finish();
}
