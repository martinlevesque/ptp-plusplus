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


package ptp.src.applications.UDP;

import inet.applications.IUDPApp;



//
simple UberUDPBasicApp like IUDPApp
{
    parameters:
        int localPort = default(-1);  // local port (-1: use ephemeral port)
        string destAddresses = default(""); // list of IP addresses, separated by spaces ("": don't send)
        int destPort;
        string messageLength; // length of messages to generate, in bytes
        double startTime = default(0.0); // application start time (start of the first packet)
        double stopTime @unit(s) = default(-1s);  // time of finishing sending, -1s means forever
        string sendInterval; // 
        double sendIntervalParetoShape = default(1.5); // for pareto
        string sendIntervalDistribution = default("pareto"); // pareto
        int timeToLive = default(-1); // if not -1, set the TTL (IPv4) or Hop Limit (IPv6) field of sent packets to this value
        int typeOfService = default(-1); // if not -1, set the ToS (IPv4) or Traffic Class (IPv6) field of sent packets to this value
        string multicastInterface = default("");  // if not empty, set the multicast output interface option on the socket (interface name expected)
        bool receiveBroadcast = default(false); // if true, makes the socket receive broadcast packets
        bool joinLocalMulticastGroups = default(false); // if true, makes the socket receive packets from all multicast groups set on local interfaces
        @display("i=block/app");
        @signal[sentPk](type=cPacket);
        @signal[rcvdPk](type=cPacket);
        @statistic[rcvdPk](title="packets received"; source=rcvdPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[sentPk](title="packets sent"; source=sentPk; record=count,"sum(packetBytes)","vector(packetBytes)"; interpolationmode=none);
        @statistic[rcvdPkLifetime](title="received packet lifetime"; source="messageAge(rcvdPk)"; unit=s; record=stats,vector; interpolationmode=none);
    gates:
        input udpIn @labels(UDPControlInfo/up);
        output udpOut @labels(UDPControlInfo/down);
}
