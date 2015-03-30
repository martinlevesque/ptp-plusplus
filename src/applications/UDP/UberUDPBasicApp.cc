//
// ptp++
// Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//


#include "UberUDPBasicApp.h"

#include "InterfaceTableAccess.h"
#include "IPvXAddressResolver.h"
#include "NodeOperations.h"
#include "UDPControlInfo_m.h"
#include <iostream>
#include "RandomUtil.h"
#include "StatsCollector.h"

using namespace std;


Define_Module(UberUDPBasicApp);

simsignal_t UberUDPBasicApp::sentPkSignal = registerSignal("sentPk");
simsignal_t UberUDPBasicApp::rcvdPkSignal = registerSignal("rcvdPk");

UberUDPBasicApp::UberUDPBasicApp()
{
    selfMsg = NULL;
}

UberUDPBasicApp::~UberUDPBasicApp()
{
    cancelAndDelete(selfMsg);
}

void UberUDPBasicApp::initialize(int stage)
{

    ApplicationBase::initialize(stage);

    if (stage == 0)
    {
        numSent = 0;
        numReceived = 0;
        WATCH(numSent);
        WATCH(numReceived);

        localPort = par("localPort");
        destPort = par("destPort");

        sendIntervalParetoShape = par("sendIntervalParetoShape").doubleValue();
        sendIntervalDistribution = par("sendIntervalDistribution").stringValue();

        double findSendInterval = 0;
        messageLengthBits = 0;

        StatsCollector* s = StatsCollector::findStatsCollector(getParentModule());

        if ( ! s)
        {
            error("Can't find StatsCollector from UberUDPBasicApp");
        }

        if (string(par("sendInterval").stringValue()) == "StatsCollector.backgroundTrafficDownstreamOfferedLoad")
        {
            if (s->backgroundTrafficDownstreamOfferedLoad <= 0)
                findSendInterval = 0;
            else
                findSendInterval = 1.0 / (s->backgroundTrafficDownstreamOfferedLoad / s->backgroundTrafficMessageLength);
        }
        else
        if (string(par("sendInterval").stringValue()) == "StatsCollector.backgroundTrafficUpstreamOfferedLoad")
        {
            if (s->backgroundTrafficUpstreamOfferedLoad <= 0)
                findSendInterval = 0;
            else
                findSendInterval = 1.0 / (s->backgroundTrafficUpstreamOfferedLoad/ s->backgroundTrafficMessageLength);
        }
        else
        {
            findSendInterval = atof(par("sendInterval").stringValue());
        }

        if (string(par("messageLength").stringValue()) == "StatsCollector.backgroundTrafficMessageLength")
        {
            messageLengthBits = s->backgroundTrafficMessageLength;
        }
        else
        {
            messageLengthBits = (int)atof(par("messageLength").stringValue());
        }

        sendInterval = findSendInterval;

        if (findSendInterval == 0)
        {
            startTime = 0;
        }
        else
        {
            startTime = RandomUtil::genInterval(sendInterval, sendIntervalParetoShape, sendIntervalDistribution);
        }

        stopTime = par("stopTime").doubleValue();

        if (stopTime >= SIMTIME_ZERO && stopTime < startTime)
            error("Invalid startTime/stopTime parameters");
        selfMsg = new cMessage("sendTimer");

        bitsSentAt.setName("bits_sent_at");
        bitsSentAt.setUnit("bps");

        curSecondRecording = 0;
        bitsPerSecond[curSecondRecording] = 0;
    }
}

void UberUDPBasicApp::finish()
{
    recordScalar("packets sent", numSent);
    recordScalar("packets received", numReceived);
    ApplicationBase::finish();
}

void UberUDPBasicApp::setSocketOptions()
{
    int timeToLive = par("timeToLive");
    if (timeToLive != -1)
        socket.setTimeToLive(timeToLive);

    int typeOfService = par("typeOfService");
    if (typeOfService != -1)
        socket.setTypeOfService(typeOfService);

    const char *multicastInterface = par("multicastInterface");
    if (multicastInterface[0])
    {
        IInterfaceTable *ift = InterfaceTableAccess().get(this);
        InterfaceEntry *ie = ift->getInterfaceByName(multicastInterface);
        if (!ie)
            throw cRuntimeError("Wrong multicastInterface setting: no interface named \"%s\"", multicastInterface);

        socket.setMulticastOutputInterface(ie->getInterfaceId());
    }

    bool receiveBroadcast = par("receiveBroadcast");
    if (receiveBroadcast)
        socket.setBroadcast(true);

    bool joinLocalMulticastGroups = par("joinLocalMulticastGroups");
    if (joinLocalMulticastGroups)
        socket.joinLocalMulticastGroups();
}

IPvXAddress UberUDPBasicApp::chooseDestAddr()
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

void UberUDPBasicApp::sendPacket()
{
    char msgName[32];
    sprintf(msgName, "UDPBasicAppData-%d", numSent);
    cPacket *payload = new cPacket(msgName);
    payload->setBitLength(messageLengthBits);

    bitsPerSecond[(int)simTime().dbl()] += payload->getBitLength();

    if (((int)simTime().dbl()) != curSecondRecording)
    {
        bitsSentAt.record(bitsPerSecond[curSecondRecording]);
        curSecondRecording = (int)simTime().dbl();
    }

    IPvXAddress destAddr = chooseDestAddr();

    emit(sentPkSignal, payload);
    socket.sendTo(payload, destAddr, destPort);
    numSent++;
}

void UberUDPBasicApp::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
    setSocketOptions();

    const char *destAddrs = par("destAddresses");
    cStringTokenizer tokenizer(destAddrs);
    const char *token;

    while ((token = tokenizer.nextToken()) != NULL) {
        IPvXAddress result;
        IPvXAddressResolver().tryResolve(token, result);
        if (result.isUnspecified())
            EV << "cannot resolve destination address: " << token << endl;
        else
            destAddresses.push_back(result);
    }

    if (!destAddresses.empty())
    {
        selfMsg->setKind(SEND);
        processSend();
    }
    else
    {
        if (stopTime >= SIMTIME_ZERO)
        {
            selfMsg->setKind(STOP);
            scheduleAt(stopTime, selfMsg);
        }
    }
}

void UberUDPBasicApp::processSend()
{
    sendPacket();
    simtime_t d = simTime() + RandomUtil::genInterval(sendInterval, sendIntervalParetoShape, sendIntervalDistribution);

    if ((stopTime < SIMTIME_ZERO || d < stopTime) && sendInterval > 0)
    {
        selfMsg->setKind(SEND);
        scheduleAt(d, selfMsg);
    }
    else
    if (stopTime != -1)
    {
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}

void UberUDPBasicApp::processStop()
{
    socket.close();


}

void UberUDPBasicApp::handleMessageWhenUp(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        ASSERT(msg == selfMsg);
        switch (selfMsg->getKind()) {
            case START: processStart(); break;
            case SEND:  processSend(); break;
            case STOP:  processStop(); break;
            default: throw cRuntimeError("Invalid kind %d in self message", (int)selfMsg->getKind());
        }
    }
    else if (msg->getKind() == UDP_I_DATA)
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

    if (ev.isGUI())
    {
        char buf[40];
        sprintf(buf, "rcvd: %d pks\nsent: %d pks", numReceived, numSent);
        getDisplayString().setTagArg("t", 0, buf);
    }
}

void UberUDPBasicApp::processPacket(cPacket *pk)
{
    emit(rcvdPkSignal, pk);
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;
    delete pk;
    numReceived++;
}

bool UberUDPBasicApp::handleNodeStart(IDoneCallback *doneCallback)
{
    simtime_t start = std::max(startTime, simTime());

    if ((stopTime < SIMTIME_ZERO) || (start < stopTime) || (start == stopTime && startTime == stopTime))
    {
        selfMsg->setKind(START);
        scheduleAt(start, selfMsg);
    }
    return true;
}

bool UberUDPBasicApp::handleNodeShutdown(IDoneCallback *doneCallback)
{
    if (selfMsg)
        cancelEvent(selfMsg);
    //TODO if(socket.isOpened()) socket.close();
    return true;
}

void UberUDPBasicApp::handleNodeCrash()
{
    if (selfMsg)
        cancelEvent(selfMsg);
}

