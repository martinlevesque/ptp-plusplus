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

#include <iostream>
#include "DatarateChannelWithPktDelay.h"

using namespace std;

Register_Class(DatarateChannelWithPktDelay);

DatarateChannelWithPktDelay::DatarateChannelWithPktDelay(const char *name)
    : cDatarateChannel(name)
{
}

DatarateChannelWithPktDelay::~DatarateChannelWithPktDelay()
{
}

void DatarateChannelWithPktDelay::initialize()
{
    cDatarateChannel::initialize();

    delayPkt = par("delayPkt").doubleValue();
}

simtime_t DatarateChannelWithPktDelay::calculateDuration(cMessage *msg) const
{
    return 0;
}

void DatarateChannelWithPktDelay::processMessage(cMessage *msg, simtime_t t, result_t& result)
{

    //cout << "---- node = " << fromNode << " full path " << string(msg->getArrivalModule()->getFullPath()) << endl;
    //cout << "  datarate = " << nodesInfos[fromNode].datarateNode << endl;
    //cout << "  delay = " << nodesInfos[fromNode].delayNode << endl;

    double localFinishTime;

    // datarate modeling
    if (msg->isPacket())
    {
        cPacket *pkt = (cPacket *)msg;
        simtime_t duration = pkt->getBitLength() / this->getDatarate() + delayPkt;
        result.duration = duration;
        this->forceTransmissionFinishTime(t + duration);
        localFinishTime = t.dbl() + duration.dbl();
    }
    else
    {
        this->forceTransmissionFinishTime(t);
    }

    // propagation delay modeling
    result.delay = this->getDelay();

    // bit error modeling - could be added
    /*
    if ((flags & (FL_BER_NONZERO | FL_PER_NONZERO)) && msg->isPacket())
    {
        cPacket *pkt = (cPacket *)msg;
        if (flags & FL_BER_NONZERO)
            if (dblrand() < 1.0 - pow(1.0-ber, (double)pkt->getBitLength()))
                pkt->setBitError(true);
        if (flags & FL_PER_NONZERO)
            if (dblrand() < per)
                pkt->setBitError(true);
    }*/

    // emit signals
    if (mayHaveListeners(messageSentSignal))
    {
        MessageSentSignalValue tmp(t, msg, &result);
        emit(messageSentSignal, &tmp);
    }
    if (mayHaveListeners(channelBusySignal))
    {
        cTimestampedValue tmp(t, 1L);
        emit(channelBusySignal, &tmp);

        //tmp.timestamp = txfinishtime;
        tmp.timestamp = localFinishTime;
        tmp.l = 0L;
        emit(channelBusySignal, &tmp);
    }
}
