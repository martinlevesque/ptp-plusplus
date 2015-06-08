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


#include "INETDefs.h"

#include "PtpPrioritizedQueue.h"
#include <iostream>

#include "IPv4Datagram_m.h"
#include "UDPPacket_m.h"

using namespace std;

Define_Module(PtpPrioritizedQueue);

simsignal_t PtpPrioritizedQueue::queueLengthSignal = registerSignal("queueLength");

void PtpPrioritizedQueue::initialize()
{
    PassiveQueueBase::initialize();

    //statistics
    emit(queueLengthSignal, 0);

    outGate = gate("out");

    // configuration
    frameCapacity = par("frameCapacity");

    queueRegular.setName("RegularQueue");
    queuePtp.setName("PtpQueue");
}

// Deep inspect the incoming frame by looking at the port to verify if it is a PTP msg.
bool PtpPrioritizedQueue::isPTPMsg(cMessage* msg)
{
    // Deep inspection!

    EthernetIIFrame* frame = dynamic_cast<EthernetIIFrame*>(msg);

    if (frame)
    {
        if (frame->getEncapsulatedPacket())
        {
            cPacket* packet = frame->getEncapsulatedPacket();

            if (packet)
            {
                IPv4Datagram_Base* ip = dynamic_cast<IPv4Datagram_Base*>(packet);

                if (ip)
                {
                    if (ip->getEncapsulatedPacket())
                    {
                        UDPPacket* udp = dynamic_cast<UDPPacket*>(ip->getEncapsulatedPacket());

                        if (udp)
                        {
                            if (udp->getDestinationPort() == 319 || udp->getDestinationPort() == 320)
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    return false;
}

cMessage *PtpPrioritizedQueue::enqueue(cMessage *msg)
{
    cQueue* queueToProcess = &queueRegular; // by default

    if (isPTPMsg(msg))
    {
        queueToProcess = &queuePtp;
    }
    else
    {
        // queueToProcess = queueRegular
    }

    if (frameCapacity && queueToProcess->length() >= frameCapacity)
    {
        EV << "Queue full, dropping packet.\n";
        return msg;
    }
    else
    {
        queueToProcess->insert(msg);

        emit(queueLengthSignal, queueLength());

        return NULL;
    }
}

cMessage *PtpPrioritizedQueue::dequeue()
{
    if (queueLength() == 0)
        return NULL;

    cQueue* q = &queueRegular;

    if ( ! queuePtp.empty())
    {
        q = &queuePtp;
    }

    cMessage *msg = (cMessage *)q->pop();

    // statistics
    emit(queueLengthSignal, queueLength());

    return msg;
}

void PtpPrioritizedQueue::sendOut(cMessage *msg)
{
    send(msg, outGate);
}

bool PtpPrioritizedQueue::isEmpty()
{
    return queueRegular.empty() && queuePtp.empty();
}

long PtpPrioritizedQueue::queueLength()
{
    return queueRegular.length() + queuePtp.length();
}

