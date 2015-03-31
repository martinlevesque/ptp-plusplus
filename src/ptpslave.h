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

#ifndef PTPSLAVE
#define PTPSLAVE

#include <omnetpp.h>
#include <MACAddress.h>
#include <string>
#include "ApplicationBase.h"
#include "UDPSocket.h"
#include "SlaveAsymmAlgo.h"
#include <map>

class PtpPacket;

#define CONTROLLER_STAGES 3

class SoftwareClock;

class PtpSlave : public ApplicationBase
{
    UDPSocket socket;
    UDPSocket socketProbe;
    bool withProbeClass;

	SoftwareClock* clock;

	MACAddress masterMAC;

	std::string delayRequestIntervalType;
	double delayRequestIntervalValue;
	double previousOs;

	std::string ptpPostAsymmAlgo;

	int localPort, destPort;

	SlaveAsymmAlgo* asymmAlgo;
	double DelayAsymmetryDms;

	double StdTm1, StdTs1, StdTs3, StdTm3, StdTm4;
	double realTs1, realTs3;

	int nbPTPDuppToSend;

	cMessage* schedNextDelayRequest;
	cMessage* schedProcessStart;

	bool controllerEnabled;

	struct {
		double e[CONTROLLER_STAGES];
		double r[CONTROLLER_STAGES];

		SimTime last;
	} controller;

	cOutVector errorVector[CONTROLLER_STAGES];
	cOutVector offsetVector[CONTROLLER_STAGES];

	cDoubleHistogram msDelay;
	cDoubleHistogram smDelay;

	cOutVector msDelayVector;
	cOutVector smDelayVector;

	cOutVector msDelayRealVector;
	cOutVector smDelayRealVector;

	std::vector<IPvXAddress> destAddresses;


	void sendDelayReq(const MACAddress& masterMAC);

	virtual void setSocketOptions();
	virtual void processStart();
	virtual void processPacket(cPacket *pk);

protected:

	virtual int numInitStages() const { return 4; }
	virtual void initialize(int stage);

	void finish();

	virtual void handleMessage(cMessage* msg);
	void handlePostProcessing(PtpPacket* ptp);

	void processDelayRequestIntervalType();

	virtual void handleMessageWhenUp(cMessage *msg);

public:

	IPvXAddress chooseDestAddr();
	void sendClassProbe(IPvXAddress ip);

	struct Timestamps {
	        SimTime msDelay;
	        SimTime smDelay;
	    } timestamps;

	void sendUpstream(PtpPacket* p);
	void correct(double modifiedOffset);

	PtpSlave();
};

#endif
