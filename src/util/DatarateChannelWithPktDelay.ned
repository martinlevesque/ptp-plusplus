//
// This program is property of its copyright holder. All rights reserved.
//
package ptp.src.util;



channel DatarateChannelWithPktDelay extends ned.DatarateChannel
{
    @class(DatarateChannelWithPktDelay);
    
    double delayPkt @unit(s) = default(0);
}
