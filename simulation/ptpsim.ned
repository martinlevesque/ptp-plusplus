//
//  ptp++
//  Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>
//
//  Adapted from Jens Steinhauser https://github.com/JenSte/omnet-ptp
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

package ptp.simulation;

import inet.examples.inet.routerperf.BurstHost;
import inet.linklayer.ethernet.EthernetInterface;
import inet.networklayer.autorouting.ipv4.HostAutoConfigurator;
import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.ethernet.EtherSwitch;
import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import inet.world.annotations.AnnotationDummy;
import ptp.src.hardwareclock.IHardwareClock;
import ptp.src.PtpMaster;
import ptp.src.SoftwareClock;
import inet.base.NotificationBoard;
import inet.networklayer.common.InterfaceTable;
import ptp.src.util.StatsCollector;
import inet.nodes.inet.NodeBase;
import inet.applications.IUDPApp;
import inet.transport.IUDP;
import ptp.src.util.DatarateChannelWithPktDelay;

// From NodeBase
import inet.base.NotificationBoard;
import inet.battery.IBattery;
import inet.linklayer.IExternalNic;
import inet.linklayer.IWiredNic;
import inet.linklayer.IWirelessNic;
import inet.linklayer.loopback.LoopbackInterface;
import inet.mobility.contract.IMobility;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.status.NodeStatus;
import inet.util.PcapRecorder;
import inet.nodes.inet.NetworkLayer;

import inet.networklayer.autorouting.ipv4.IPv4NodeConfigurator;
// End from NodeBase

module PtpNode //extends NodeBase
{
    parameters:
        @node();
        @labels(node,ptp-node);



        // From NodeBase
        bool hasStatus = default(false);
        int numExtInterfaces = default(0);
        int numRadios = default(0);               // the number of radios in the router. by default no wireless
        int numPcapRecorders = default(0); // no of PcapRecorders.
        string mobilityType = default(numRadios > 0 ? "StationaryMobility" : "");
        string routingFile = default("");
        bool IPForward = default(false);
        bool forwardMulticast = default(false);
        string batteryType = default("");
        // End from NodeBase

        string ptpImplementationType;
        string hwClockType;

		////////////////////
		// From StandardHost
        int numUdpApps = default(0);  // no of UDP apps. Specify the app types in INI file with udpApp[0..1].typename="UDPVideoStreamCli" syntax


        bool hasUdp = default(numUdpApps>0);
        string udpType = default(firstAvailable("UDP","UDP_None"));

        networkLayer.proxyARP = default(false);
        // End from StandardHost
        ///////////////////

        @display("bgb=862,476");
    gates:
        input radioIn[] @directIn; //TODO change this if we need WiFi
        // inout pppg[] @labels(PPPFrame-conn);
        inout ethg @labels(EtherFrame-conn);

    submodules:


        udpApp[numUdpApps]: <> like IUDPApp {
            parameters:
                @display("p=329,54,row,60");
        }
        udp: <udpType> like IUDP if hasUdp {
            parameters:
                @display("p=329,210");
        }

        swClock: SoftwareClock {
            parameters:
                @display("p=140,116");
                ptpImplementationType = ptpImplementationType;
        }
        hwClock: <hwClockType> like IHardwareClock {
            parameters:
                @display("p=140,266");
        }



        //osJitter: Jitter {
        //    parameters:
        //        @display("p=255,142");
        //}

        ////////////////////////
        // From NodeBase
        status: NodeStatus if hasStatus {
            @display("p=50,50");
        }

        notificationBoard: NotificationBoard {
            parameters:
                @display("p=53,155;is=s");
        }

        battery: <batteryType> like IBattery if batteryType != "" {
            parameters:
                @display("p=53,77;i=block/plug;is=s");
        }

        // optional mobility module. Required only if wireless cards are present
        mobility: <mobilityType> like IMobility if mobilityType != "" {
            parameters:
                @display("p=53,111");
        }

        // network layer
        networkLayer: NetworkLayer {
            parameters:
                @display("p=329,318;q=queue");
        }

        routingTable: RoutingTable {
            parameters:
                @display("p=53,225;is=s");
                IPForward = IPForward;
                forwardMulticast = forwardMulticast;
                routingFile = routingFile;
        }

        // linklayer
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=53,287;is=s");
        }

        pcapRecorder[numPcapRecorders]: PcapRecorder {
            @display("p=53,338,r,10");
        }

        lo0: LoopbackInterface {
            @display("p=78,406");
        }
        wlan[numRadios]: <default("Ieee80211Nic")> like IWirelessNic {
            parameters:
                @display("p=216,406,row,60;q=queue");
        }
        eth[1]: <default("EthernetInterface")> like IWiredNic {
            parameters:
                @display("p=430,416,row,60;q=txQueue");
        }
        //ppp[sizeof(pppg)]: <default("PPPInterface")> like IWiredNic {
        //    parameters:
        //        @display("p=558,406,row,60;q=txQueue");
        //}
        //ext[numExtInterfaces]: <default("ExtInterface")> like IExternalNic {
        //    parameters:
        //        @display("p=149,406,row,10;q=txQueue;i=block/ifcard");
        //}
        // End from NodeBase
        ////////////////////////



    connections allowunconnected:


        //eth[0].phys <--> {  datarate = 100Mbps; delay = 50ns; } <--> phyEth.internal;
        //phyEth.external <-->  ethg;

        eth[0].phys <--> ethg;

        // From NodeBase
        networkLayer.ifOut++ --> lo0.upperLayerIn;
        lo0.upperLayerOut --> networkLayer.ifIn++;

        for i=0..sizeof(radioIn)-1 {
            radioIn[i] --> { @display("m=s"); } --> wlan[i].radioIn;
            wlan[i].upperLayerOut --> networkLayer.ifIn++;
            wlan[i].upperLayerIn <-- networkLayer.ifOut++;
        }

        for i=0..sizeof(ethg)-1 {
            eth[i].upperLayerOut --> networkLayer.ifIn++;
            eth[i].upperLayerIn <-- networkLayer.ifOut++;
        }
        // End from NodeBase

        ////////////////////////
        // From StandardHost
        for i=0..numUdpApps-1 {
            udpApp[i].udpOut --> udp.appIn++;
            udpApp[i].udpIn <-- udp.appOut++;
            //udpApp[i].udpOut --> osJitter.upperLayerIn++;
            //udpApp[i].udpIn <-- osJitter.upperLayerOut++;

            //osJitter.lowerLayer$o --> udp.appIn++;
            //osJitter.lowerLayer$i <-- udp.appOut++;
        }

        udp.ipOut --> networkLayer.transportIn++ if hasUdp;
        udp.ipIn <-- networkLayer.transportOut++ if hasUdp;

        // End StandardHost
        ///////////////////////
}

module PtpMasterNode extends PtpNode
{
    parameters:
        hwClockType = "ConstantDrift";
        hwClock.constant_drift = 0;

        ptpImplementationType = "PtpMaster";
        //numUdpApps = 1;
        //udpApp[0].typename = "PtpMaster";

        @display("bgb=862,485");
}

module PtpSlaveNode extends PtpNode
{
    parameters:
        hwClockType = default("BoundedDriftVariation");

        ptpImplementationType = "PtpSlave";
        //numUdpApps = 1;
        //udpApp[0].typename = "PtpSlave";

        @display("bgb=875,485");
}

network GenericNetwork
{
    parameters:
        int slaves = default(2);
        volatile double backgroundDownstreamTrafficPacketsPerSec = default(0);
        volatile double backgroundUpstreamTrafficPacketsPerSec = default(0);
        int backgroundTrafficMessageLength = 1400 * 8;
        double extraPTPUpstreamDelay @unit(s) = default(0.0s);
        double asymmetricCapacity @unit(bps) = default(100Mbps);

        int nbPTPDuppToSend = default(0);



    submodules:
        StatsCollector: StatsCollector {
            @display("p=60,54");
            backgroundTrafficDownstreamOfferedLoad = backgroundDownstreamTrafficPacketsPerSec * double(backgroundTrafficMessageLength);
            backgroundTrafficUpstreamOfferedLoad = backgroundUpstreamTrafficPacketsPerSec * double(backgroundTrafficMessageLength);
            backgroundTrafficMessageLength = backgroundTrafficMessageLength;
            extraPTPUpstreamDelay = extraPTPUpstreamDelay;
            nbPTPDuppToSend = nbPTPDuppToSend;
        }

        configurator: IPv4NetworkConfigurator {
            @display("p=152,54");
        }
}

network BasicNetwork extends GenericNetwork
{
    parameters:

        @display("bgb=1161,409");
    submodules:
        m: PtpMasterNode {
            parameters:
                @display("p=575,54");
                IPForward = false;
                forwardMulticast = false;
        }

        router: Router {
            parameters:
                @display("p=575,154");

                forwardMulticast = true;
        }


        s[slaves]: PtpSlaveNode {
            parameters:
                @display("p=57,267,r,100");
                IPForward = false;
                forwardMulticast = false;
        }
    connections allowunconnected:
        m.ethg <--> {  datarate = 100Mbps; delay = 0ns; } <--> router.ethg++;

        for i=0..sizeof(s) - 1 {
            s[i].ethg <--> {  datarate = 100Mbps; delay = 0ns; } <--> router.ethg++;
        }
}

network BasicWithTrafficInjectionNetwork extends GenericNetwork
{
    parameters:


        @display("bgb=1241,521");

    types:
        channel A extends DatarateChannelWithPktDelay
        {
            datarate = 100Mbps;
        }

    submodules:
        m: PtpMasterNode {
            parameters:
                @display("p=575,30");
                IPForward = false;
                forwardMulticast = false;
        }

        router: Router {
            parameters:
                @display("p=575,140");

                forwardMulticast = true;
        }


        s[slaves]: PtpSlaveNode {
            parameters:
                @display("p=47,471,r,100");
                IPForward = false;
                forwardMulticast = false;
        }

        router2: Router {
            parameters:
                @display("p=575,387");

                forwardMulticast = true;
        }

        trafSource: StandardHost {
            @display("p=771,140");
            //udpApp[0].sendInterval = 0.001;
            //udpApp[0].sendInterval = 0.001;
        }
        trafDest: StandardHost {
            @display("p=771,387");
        }
        downstream: Router {
            parameters:
                @display("p=504,258");

                forwardMulticast = true;
        }


        upstream: Router {
            parameters:
                @display("p=645,258");

                forwardMulticast = true;
        }


    connections allowunconnected:
        //m.ethg <--> AsymmetricChannel { node1 = "m"; datarateNode1 = 100000000; delayNode1 = 0.000; node2 = "router"; datarateNode2 = 100000000; delayNode2 = 0.000; } <--> router.ethg++;

        //for i=0..sizeof(s) - 1 
        //{
        //    s[i].ethg <--> AsymmetricChannel { node1 = "s[" + string(i) + "]"; delayNode1 = extraPTPUpstreamDelay; datarateNode1 = 100000000; node2 = "router2"; datarateNode2 = 100000000; delayNode2 = 0.000; }  <--> router2.ethg++;
        //}

        //router2.ethg++ <--> AsymmetricChannel { node1 = "router2"; datarateNode1 = 100000000; delayNode1 = 0.000; node2 = "router"; datarateNode2 = 100000000; delayNode2 = 0.000; } <--> router.ethg++;
        //trafSource.ethg++ <--> AsymmetricChannel { node1 = "trafSource"; datarateNode1 = 100000000; delayNode1 = 0.000; node2 = "router"; datarateNode2 = 100000000; delayNode2 = 0.000; } <--> router.ethg++;
        //trafDest.ethg++ <--> AsymmetricChannel { node1 = "trafDest"; datarateNode1 = 100000000; delayNode1 = 0.000; node2 = "router2"; datarateNode2 = 100000000; delayNode2 = 0.000; } <--> router2.ethg++;

        m.ethg <--> {  datarate = 100Mbps; delay = 0s; } <--> router.ethg++;

        for i=0..sizeof(s) - 1 {
            s[i].ethg <--> {  datarate = 100Mbps; delay = 0s; } <--> router2.ethg++;
        }

        trafSource.ethg++ <--> DatarateChannelWithPktDelay {  datarate = 100Mbps; delay = 0s; } <--> router.ethg++;
        trafDest.ethg++ <--> DatarateChannelWithPktDelay {  datarate = 100Mbps; delay = 0s; } <--> router2.ethg++;
        router.ethg++ <--> DatarateChannelWithPktDelay {  datarate = asymmetricCapacity; delay = 0s; } <--> upstream.ethg++;
        upstream.ethg++ <--> DatarateChannelWithPktDelay {  datarate = asymmetricCapacity; delay = 0s; delayPkt = extraPTPUpstreamDelay; } <--> router2.ethg++;
        downstream.ethg++ <--> DatarateChannelWithPktDelay {  datarate = 100Mbps; delay = 0.000s; delayPkt = 0.00000s; } <--> router2.ethg++;
        downstream.ethg++ <--> DatarateChannelWithPktDelay {  datarate = 100Mbps; delay = 0.000s; } <--> router.ethg++;
}
