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

#ifndef PTP_H
#define PTP_H

#define PTP_CLASS_PROBE_PORT 20000

#include "UDPSocket.h"

class PtpPacket;
class MACAddress;

namespace Ptp
{

	enum FrameType {
		Sync,
		Delay_Req,
		Delay_Resp,

		// Lee post asymm algo
        Lee_CheckReq,
        Lee_LoopReqStart,
        Lee_LoopReqMiddle,
        Lee_LoopReqEnd,
        Lee_LoopRespStart,
        Lee_LoopRespMiddle,
        Lee_LoopRespEnd,
        Lee_CheckResp,

        // Lev post asymm algo
        Lev_CheckReq,
        Lev_LoopReqStart,
        Lev_LoopReqMiddle,
        Lev_LoopReqEnd,
        Lev_LoopRespStart,
        Lev_LoopRespMiddle,
        Lev_LoopRespEnd,
        Lev_CheckResp,

        // Lv post asymm algo
        Lv_ReviseReq,
        Lv_Explorer1,
        Lv_Explorer2,
        Lv_ReviseResp,

	};



	enum ET { Ethertype = 0x88f7 };

	PtpPacket* newSyncPacket(const MACAddress& dest);

	PtpPacket* newDelayReqPacket(const MACAddress& dest);

	PtpPacket* newDelayRespPacket(const MACAddress& dest);

	PtpPacket* newGenericPtpPacket(const MACAddress& dest, FrameType type);

	IPvXAddress getSrcIP(PtpPacket* ptp);
	IPvXAddress getDestIP(PtpPacket* ptp);
}

#endif
