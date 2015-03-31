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

#include "ptp.h"
#include "ptppacket_m.h"
#include <Ieee802Ctrl_m.h>
#include "UDPControlInfo_m.h"

#include <iostream>

using namespace std;

static PtpPacket* newPtpPacket(const MACAddress& dest, Ptp::FrameType type)
{
	const char* name = NULL;
	int64 length = 0;

	// the length values are taken from section D.1.3.2 and D.1.3.4 of the standard

	switch (type) {

	// PTP std.
	case Ptp::Sync:       name = "ptp sync packet";       length = 126; break;
	case Ptp::Delay_Req:  name = "ptp delay_req packet";  length = 126; break;
	case Ptp::Delay_Resp: name = "ptp delay_resp packet"; length =  63; break;


	// Lee post asymm algo
	case Ptp::Lee_CheckReq: name = "Lee_CheckReq packet"; length =  63; break;
	case Ptp::Lee_CheckResp: name = "Lee_CheckResp packet"; length =  63; break;
	case Ptp::Lee_LoopReqStart: name = "Lee_LoopReqStart packet"; length =  126; break;
	case Ptp::Lee_LoopReqMiddle: name = "Lee_LoopReqMiddle packet"; length =  126; break;
	case Ptp::Lee_LoopReqEnd: name = "Lee_LoopReqEnd packet"; length =  126; break;
	case Ptp::Lee_LoopRespStart: name = "Lee_LoopRespStart packet"; length =  126; break;
	case Ptp::Lee_LoopRespMiddle: name = "Lee_LoopRespMiddle packet"; length =  126; break;
	case Ptp::Lee_LoopRespEnd: name = "Lee_LoopRespEnd packet"; length =  126; break;

	// Lev
	case Ptp::Lev_CheckReq: name = "Lev_CheckReq packet"; length =  63; break;
    case Ptp::Lev_CheckResp: name = "Lev_CheckResp packet"; length =  63; break;
    case Ptp::Lev_LoopReqStart: name = "Lev_LoopReqStart packet"; length =  126; break;
    case Ptp::Lev_LoopReqMiddle: name = "Lev_LoopReqMiddle packet"; length =  126; break;
    case Ptp::Lev_LoopReqEnd: name = "Lev_LoopReqEnd packet"; length =  1; break;
    case Ptp::Lev_LoopRespStart: name = "Lev_LoopRespStart packet"; length =  126; break;
    case Ptp::Lev_LoopRespMiddle: name = "Lev_LoopRespMiddle packet"; length =  126; break;
    case Ptp::Lev_LoopRespEnd: name = "Lee_LoopRespEnd packet"; length =  126; break;

	// Lv post asymm algo
	case Ptp::Lv_ReviseReq: name = "Lv_ReviseReq packet"; length =  63; break;
	case Ptp::Lv_ReviseResp: name = "Lv_ReviseResp packet"; length =  63; break;
	case Ptp::Lv_Explorer1: name = "Lv_Explorer1 packet"; length =  126; break;
	case Ptp::Lv_Explorer2: name = "Lv_Explorer2 packet"; length =  126*2; break; // second is twice long
	}

	PtpPacket* p = new PtpPacket(name, IEEE802CTRL_DATA);
	p->setByteLength(length);
	p->setType(type);
	p->setTrx(0);

	//Ieee802Ctrl* c = new Ieee802Ctrl();

	//c->setDest(dest);
	//c->setEtherType(Ptp::Ethertype);
	//p->setControlInfo(c);

	return p;
}

PtpPacket* Ptp::newSyncPacket(const MACAddress& dest)
{
	return newPtpPacket(dest, Ptp::Sync);
}

PtpPacket* Ptp::newDelayReqPacket(const MACAddress& dest)
{
	return newPtpPacket(dest, Ptp::Delay_Req);
}

PtpPacket* Ptp::newDelayRespPacket(const MACAddress& dest)
{
	return newPtpPacket(dest, Ptp::Delay_Resp);
}

PtpPacket* Ptp::newGenericPtpPacket(const MACAddress& dest, Ptp::FrameType type)
{
    return newPtpPacket(dest, type);
}

IPvXAddress Ptp::getSrcIP(PtpPacket* ptp)
{
    IPvXAddress slaveIP;

    if ( dynamic_cast<UDPDataIndication*>(ptp->getControlInfo()))
    {
        UDPDataIndication* ctrl = dynamic_cast<UDPDataIndication*>(ptp->getControlInfo());

        slaveIP = ctrl->getSrcAddr().get4();
    }
    else
    {
        return slaveIP;
    }

    return slaveIP;
}

IPvXAddress Ptp::getDestIP(PtpPacket* ptp)
{
    IPvXAddress slaveIP;

    if ( dynamic_cast<UDPDataIndication*>(ptp->getControlInfo()))
    {
        UDPDataIndication* ctrl = dynamic_cast<UDPDataIndication*>(ptp->getControlInfo());

        slaveIP = ctrl->getDestAddr().get4();
    }
    else
    {
        return slaveIP;
    }

    return slaveIP;
}
