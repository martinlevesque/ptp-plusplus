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

#ifndef __PTP_SIM_DatarateChannelWithPktDelay_CHANNEL_H
#define __PTP_SIM_DatarateChannelWithPktDelay_CHANNEL_H

#include "INETDefs.h"
#include <string>

/**
 *
 */
class DatarateChannelWithPktDelay : public cDatarateChannel
{

  public:
    explicit DatarateChannelWithPktDelay(const char *name = NULL);

    virtual ~DatarateChannelWithPktDelay();

    virtual void initialize();

    virtual void processMessage(cMessage *msg, simtime_t t, result_t& result);

    virtual simtime_t calculateDuration(cMessage *msg) const;

  private:
    double delayPkt;
};

#endif
