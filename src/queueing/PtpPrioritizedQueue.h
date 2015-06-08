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


#ifndef __PtpPrioritizedQueue_H
#define __PtpPrioritizedQueue_H

#include "INETDefs.h"

#include "PassiveQueueBase.h"
#include "EtherFrame_m.h"


/**
 *
 */
class PtpPrioritizedQueue : public PassiveQueueBase
{
  protected:
    // configuration
    int frameCapacity;

    // state
    cQueue queueRegular;
    cQueue queuePtp;
    cGate *outGate;

    // statistics
    static simsignal_t queueLengthSignal;

  protected:
    virtual void initialize();

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual cMessage *enqueue(cMessage *msg);

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual cMessage *dequeue();

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual void sendOut(cMessage *msg);

    /**
     * Redefined from IPassiveQueue.
     */
    virtual bool isEmpty();

    long queueLength();

    bool isPTPMsg(cMessage* m);
};

#endif
