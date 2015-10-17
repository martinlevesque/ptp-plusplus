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


#ifndef __PTP_UBERUDPSINK_H
#define __PTP_UBERUDPSINK_H

#include "INETDefs.h"

#include "ApplicationBase.h"
#include "UDPSocket.h"


/**
 * Consumes and prints packets received from the UDP module. See NED for more info.
 */
class UberUDPSink : public ApplicationBase
{
  protected:
    UDPSocket socket;
    int numReceived;
    //static simsignal_t rcvdPkSignal;

  protected:
    virtual void processPacket(cPacket *msg);

  protected:
    virtual int numInitStages() const { return 4; }
    virtual void initialize(int stage);
    virtual void handleMessageWhenUp(cMessage *msg);
    virtual void finish();

    virtual bool handleNodeStart(IDoneCallback *doneCallback);
    virtual bool handleNodeShutdown(IDoneCallback *doneCallback);
    virtual void handleNodeCrash();
};

#endif

