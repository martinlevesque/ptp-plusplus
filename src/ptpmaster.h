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

#ifndef PTPMASTER
#define PTPMASTER

#include <omnetpp.h>
#include "ApplicationBase.h"
#include "UDPSocket.h"
#include <map>
#include <string>
#include "MasterAsymmAlgo.h"

class PtpPacket;

struct SlaveInfos
{
    double StdTm3, StdTm4;
};

class PtpMaster : public ApplicationBase
{
    UDPSocket socket;
    UDPSocket socketProbe;
    bool withProbeClass;

    std::vector<IPvXAddress> destAddresses;

	SimTime syncInterval;
	bool processStarted;

	cMessage* selfMsg;

	int localPort, destPort;

	std::map<std::string, SlaveInfos> slaveInfos;

	std::string ptpPostAsymmAlgo;

	int nbPTPDuppToSend;

	void sendSync();


	void processDelayReq(PtpPacket* delayReq);

	virtual void setSocketOptions();

	void processStart();

	virtual void processPacket(cPacket *pk);


protected:
	virtual int numInitStages() const { return 4; }
	virtual void initialize(int stage);
	virtual void finish();

	virtual void handleMessage(cMessage* msg);

	virtual void handleMessageWhenUp(cMessage *msg);

	MasterAsymmAlgo* asymmAlgo;

public:

	IPvXAddress chooseDestAddr();
	void sendClassProbe(IPvXAddress ip);

	PtpMaster();
};

#endif
