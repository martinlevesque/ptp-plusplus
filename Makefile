#
# OMNeT++/OMNEST Makefile for ptp-sim
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/ieee80211/radio/errormodel -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/mobility/contract -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/applications/pingapp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/ieee80211/radio -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/base -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/transport/sctp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/autorouting/ipv4 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util/headerserializers/ipv6 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/arp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/ipv6 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/radio/propagation -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/routing/aodv -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/xmipv6 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/transport/contract -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/ipv6tunneling -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/world/radio -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/common -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util/headerserializers/ipv4 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/contract -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/radio -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/world/obstacles -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util/headerserializers -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/icmpv6 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/transport/tcp_common -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/battery/models -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/status -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/applications/common -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/transport/udp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/ieee80211/mac -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/contract -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util/headerserializers/sctp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/common -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/networklayer/ipv4 -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util/headerserializers/udp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/util/headerserializers/tcp -I/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/src/linklayer/ethernet -L/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6/out/$$\(CONFIGNAME\)/src -linet -DINET_IMPORT -KINET_2_6_PROJ=/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6
#

# Name of target to be created (-o option)
TARGET = ptp-sim$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I$(INET_2_6_PROJ)/src/linklayer/ieee80211/radio/errormodel \
    -I$(INET_2_6_PROJ)/src/mobility/contract \
    -I$(INET_2_6_PROJ)/src/applications/pingapp \
    -I$(INET_2_6_PROJ)/src \
    -I$(INET_2_6_PROJ)/src/linklayer/ieee80211/radio \
    -I$(INET_2_6_PROJ)/src/base \
    -I$(INET_2_6_PROJ)/src/transport/sctp \
    -I$(INET_2_6_PROJ)/src/networklayer/autorouting/ipv4 \
    -I$(INET_2_6_PROJ)/src/util/headerserializers/ipv6 \
    -I$(INET_2_6_PROJ)/src/networklayer/arp \
    -I$(INET_2_6_PROJ)/src/networklayer/ipv6 \
    -I$(INET_2_6_PROJ)/src/linklayer/radio/propagation \
    -I$(INET_2_6_PROJ)/src/networklayer/routing/aodv \
    -I$(INET_2_6_PROJ)/src/networklayer/xmipv6 \
    -I$(INET_2_6_PROJ)/src/transport/contract \
    -I$(INET_2_6_PROJ)/src/networklayer/ipv6tunneling \
    -I$(INET_2_6_PROJ)/src/world/radio \
    -I$(INET_2_6_PROJ)/src/networklayer/common \
    -I$(INET_2_6_PROJ)/src/util/headerserializers/ipv4 \
    -I$(INET_2_6_PROJ)/src/linklayer/contract \
    -I$(INET_2_6_PROJ)/src/linklayer/radio \
    -I$(INET_2_6_PROJ)/src/world/obstacles \
    -I$(INET_2_6_PROJ)/src/util/headerserializers \
    -I$(INET_2_6_PROJ)/src/networklayer/icmpv6 \
    -I$(INET_2_6_PROJ)/src/transport/tcp_common \
    -I$(INET_2_6_PROJ)/src/battery/models \
    -I$(INET_2_6_PROJ)/src/status \
    -I$(INET_2_6_PROJ)/src/applications/common \
    -I$(INET_2_6_PROJ)/src/transport/udp \
    -I$(INET_2_6_PROJ)/src/linklayer/ieee80211/mac \
    -I$(INET_2_6_PROJ)/src/networklayer/contract \
    -I$(INET_2_6_PROJ)/src/util/headerserializers/sctp \
    -I$(INET_2_6_PROJ)/src/linklayer/common \
    -I$(INET_2_6_PROJ)/src/util \
    -I$(INET_2_6_PROJ)/src/networklayer/ipv4 \
    -I$(INET_2_6_PROJ)/src/util/headerserializers/udp \
    -I$(INET_2_6_PROJ)/src/util/headerserializers/tcp \
    -I$(INET_2_6_PROJ)/src/linklayer/ethernet \
    -I. \
    -Isimulation \
    -Isimulation/results \
    -Isrc \
    -Isrc/applications \
    -Isrc/applications/UDP \
    -Isrc/asymm \
    -Isrc/hardwareclock \
    -Isrc/queueing \
    -Isrc/util

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(INET_2_6_PROJ)/out/$(CONFIGNAME)/src  -linet
LIBS += -Wl,-rpath,`abspath $(INET_2_6_PROJ)/out/$(CONFIGNAME)/src`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/src/ptp.o \
    $O/src/ptpmaster.o \
    $O/src/ptpslave.o \
    $O/src/softwareclock.o \
    $O/src/applications/UDP/UberUDPBasicApp.o \
    $O/src/applications/UDP/UberUDPSink.o \
    $O/src/asymm/MasterAsymmAlgo.o \
    $O/src/asymm/MasterAsymmLee.o \
    $O/src/asymm/MasterAsymmLev.o \
    $O/src/asymm/MasterAsymmLv.o \
    $O/src/asymm/SlaveAsymmAlgo.o \
    $O/src/asymm/SlaveAsymmLee.o \
    $O/src/asymm/SlaveAsymmLev.o \
    $O/src/asymm/SlaveAsymmLv.o \
    $O/src/hardwareclock/driftsource.o \
    $O/src/hardwareclock/hardwareclock.o \
    $O/src/hardwareclock/hardwareclockclient.o \
    $O/src/hardwareclock/scheduletestapp.o \
    $O/src/hardwareclock/storagewindow.o \
    $O/src/queueing/PtpPrioritizedQueue.o \
    $O/src/util/DatarateChannelWithPktDelay.o \
    $O/src/util/RandomUtil.o \
    $O/src/util/StatsCollector.o \
    $O/src/ptppacket_m.o

# Message files
MSGFILES = \
    src/ptppacket.msg

# Other makefile variables (-K)
INET_2_6_PROJ=/Users/dtres1/works/programmes/omnetpp-4.6/samples/inet-2.6

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" -loppmain$D $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS) -DINET_IMPORT $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

# we want to recompile everything if COPTS changes,
# so we store COPTS into $COPTS_FILE and have object
# files depend on it (except when "make depend" was called)
COPTS_FILE = $O/.last-copts
ifneq ($(MAKECMDGOALS),depend)
ifneq ("$(COPTS)","$(shell cat $(COPTS_FILE) 2>/dev/null || echo '')")
$(shell $(MKPATH) "$O" && echo "$(COPTS)" >$(COPTS_FILE))
endif
endif

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $O/$(TARGET)
	$(Q)$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	@echo Creating executable: $@
	$(Q)$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(AS_NEEDED_OFF) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY: all clean cleanall depend msgheaders

.SUFFIXES: .cc

$O/%.o: %.cc $(COPTS_FILE)
	@$(MKPATH) $(dir $@)
	$(qecho) "$<"
	$(Q)$(CXX) -c $(CXXFLAGS) $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(qecho) MSGC: $<
	$(Q)$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	$(qecho) Cleaning...
	$(Q)-rm -rf $O
	$(Q)-rm -f ptp-sim ptp-sim.exe libptp-sim.so libptp-sim.a libptp-sim.dll libptp-sim.dylib
	$(Q)-rm -f ./*_m.cc ./*_m.h
	$(Q)-rm -f simulation/*_m.cc simulation/*_m.h
	$(Q)-rm -f simulation/results/*_m.cc simulation/results/*_m.h
	$(Q)-rm -f src/*_m.cc src/*_m.h
	$(Q)-rm -f src/applications/*_m.cc src/applications/*_m.h
	$(Q)-rm -f src/applications/UDP/*_m.cc src/applications/UDP/*_m.h
	$(Q)-rm -f src/asymm/*_m.cc src/asymm/*_m.h
	$(Q)-rm -f src/hardwareclock/*_m.cc src/hardwareclock/*_m.h
	$(Q)-rm -f src/queueing/*_m.cc src/queueing/*_m.h
	$(Q)-rm -f src/util/*_m.cc src/util/*_m.h

cleanall: clean
	$(Q)-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(qecho) Creating dependencies...
	$(Q)$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc simulation/*.cc simulation/results/*.cc src/*.cc src/applications/*.cc src/applications/UDP/*.cc src/asymm/*.cc src/hardwareclock/*.cc src/queueing/*.cc src/util/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/src/ptp.o: src/ptp.cc \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/ptp.h \
	src/ptppacket_m.h
$O/src/ptpmaster.o: src/ptpmaster.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLee.h \
	src/asymm/MasterAsymmLev.h \
	src/asymm/MasterAsymmLv.h \
	src/ptp.h \
	src/ptpmaster.h \
	src/ptppacket_m.h \
	src/util/StatsCollector.h
$O/src/ptppacket_m.o: src/ptppacket_m.cc \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/ptp.h \
	src/ptppacket_m.h
$O/src/ptpslave.o: src/ptpslave.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLv.h \
	src/asymm/SlaveAsymmAlgo.h \
	src/asymm/SlaveAsymmLee.h \
	src/asymm/SlaveAsymmLev.h \
	src/asymm/SlaveAsymmLv.h \
	src/ptp.h \
	src/ptppacket_m.h \
	src/ptpslave.h \
	src/softwareclock.h \
	src/util/StatsCollector.h
$O/src/softwareclock.o: src/softwareclock.cc \
	src/hardwareclock/hardwareclock.h \
	src/softwareclock.h \
	src/util/StatsCollector.h
$O/src/applications/UDP/UberUDPBasicApp.o: src/applications/UDP/UberUDPBasicApp.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/ModuleAccess.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/NotifierConsts.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/common/IInterfaceTable.h \
	$(INET_2_6_PROJ)/src/networklayer/common/InterfaceEntry.h \
	$(INET_2_6_PROJ)/src/networklayer/common/InterfaceTableAccess.h \
	$(INET_2_6_PROJ)/src/networklayer/common/InterfaceToken.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddressResolver.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/applications/UDP/UberUDPBasicApp.h \
	src/util/RandomUtil.h \
	src/util/StatsCollector.h
$O/src/applications/UDP/UberUDPSink.o: src/applications/UDP/UberUDPSink.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPControlInfo_m.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/applications/UDP/UberUDPSink.h
$O/src/asymm/MasterAsymmAlgo.o: src/asymm/MasterAsymmAlgo.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/ptp.h \
	src/ptpmaster.h \
	src/ptppacket_m.h
$O/src/asymm/MasterAsymmLee.o: src/asymm/MasterAsymmLee.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLee.h \
	src/ptp.h \
	src/ptpmaster.h \
	src/ptppacket_m.h
$O/src/asymm/MasterAsymmLev.o: src/asymm/MasterAsymmLev.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLev.h \
	src/ptp.h \
	src/ptpmaster.h \
	src/ptppacket_m.h
$O/src/asymm/MasterAsymmLv.o: src/asymm/MasterAsymmLv.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLv.h \
	src/ptp.h \
	src/ptppacket_m.h
$O/src/asymm/SlaveAsymmAlgo.o: src/asymm/SlaveAsymmAlgo.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/SlaveAsymmAlgo.h \
	src/ptp.h \
	src/ptppacket_m.h \
	src/ptpslave.h \
	src/softwareclock.h \
	src/util/StatsCollector.h
$O/src/asymm/SlaveAsymmLee.o: src/asymm/SlaveAsymmLee.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLv.h \
	src/asymm/SlaveAsymmAlgo.h \
	src/asymm/SlaveAsymmLee.h \
	src/ptp.h \
	src/ptppacket_m.h \
	src/ptpslave.h \
	src/softwareclock.h \
	src/util/StatsCollector.h
$O/src/asymm/SlaveAsymmLev.o: src/asymm/SlaveAsymmLev.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLv.h \
	src/asymm/SlaveAsymmAlgo.h \
	src/asymm/SlaveAsymmLev.h \
	src/ptp.h \
	src/ptppacket_m.h \
	src/ptpslave.h \
	src/softwareclock.h \
	src/util/StatsCollector.h
$O/src/asymm/SlaveAsymmLv.o: src/asymm/SlaveAsymmLv.cc \
	$(INET_2_6_PROJ)/src/applications/common/ApplicationBase.h \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/ILifecycle.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/LifecycleOperation.h \
	$(INET_2_6_PROJ)/src/base/NodeOperations.h \
	$(INET_2_6_PROJ)/src/base/OperationalBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv6Address.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPvXAddress.h \
	$(INET_2_6_PROJ)/src/transport/contract/UDPSocket.h \
	src/asymm/MasterAsymmAlgo.h \
	src/asymm/MasterAsymmLv.h \
	src/asymm/SlaveAsymmAlgo.h \
	src/asymm/SlaveAsymmLv.h \
	src/ptp.h \
	src/ptppacket_m.h \
	src/ptpslave.h \
	src/softwareclock.h \
	src/util/StatsCollector.h
$O/src/hardwareclock/driftsource.o: src/hardwareclock/driftsource.cc \
	src/hardwareclock/driftsource.h
$O/src/hardwareclock/hardwareclock.o: src/hardwareclock/hardwareclock.cc \
	src/hardwareclock/driftsource.h \
	src/hardwareclock/hardwareclock.h \
	src/hardwareclock/hardwareclockclient.h \
	src/hardwareclock/storagewindow.h
$O/src/hardwareclock/hardwareclockclient.o: src/hardwareclock/hardwareclockclient.cc \
	src/hardwareclock/hardwareclock.h \
	src/hardwareclock/hardwareclockclient.h
$O/src/hardwareclock/scheduletestapp.o: src/hardwareclock/scheduletestapp.cc \
	src/hardwareclock/hardwareclock.h \
	src/hardwareclock/hardwareclockclient.h \
	src/hardwareclock/scheduletestapp.h
$O/src/hardwareclock/storagewindow.o: src/hardwareclock/storagewindow.cc \
	src/hardwareclock/driftsource.h \
	src/hardwareclock/hardwareclock.h \
	src/hardwareclock/storagewindow.h
$O/src/queueing/PtpPrioritizedQueue.o: src/queueing/PtpPrioritizedQueue.cc \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	$(INET_2_6_PROJ)/src/base/IPassiveQueue.h \
	$(INET_2_6_PROJ)/src/base/PassiveQueueBase.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/Ieee802Ctrl_m.h \
	$(INET_2_6_PROJ)/src/linklayer/contract/MACAddress.h \
	$(INET_2_6_PROJ)/src/linklayer/ethernet/EtherFrame_m.h \
	$(INET_2_6_PROJ)/src/linklayer/ethernet/Ethernet.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPProtocolId_m.h \
	$(INET_2_6_PROJ)/src/networklayer/contract/IPv4Address.h \
	$(INET_2_6_PROJ)/src/networklayer/ipv4/IPv4Datagram_m.h \
	$(INET_2_6_PROJ)/src/transport/udp/UDPPacket_m.h \
	src/queueing/PtpPrioritizedQueue.h
$O/src/util/DatarateChannelWithPktDelay.o: src/util/DatarateChannelWithPktDelay.cc \
	$(INET_2_6_PROJ)/src/base/Compat.h \
	$(INET_2_6_PROJ)/src/base/INETDefs.h \
	src/util/DatarateChannelWithPktDelay.h
$O/src/util/RandomUtil.o: src/util/RandomUtil.cc \
	src/util/RandomUtil.h
$O/src/util/StatsCollector.o: src/util/StatsCollector.cc \
	src/util/StatsCollector.h

