//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

#ifndef __GEN_NODE_H__
#define __GEN_NODE_H__

#include <omnetpp.h>

#include <string>
#include <vector>
#include <map>

struct trafGen
{
    cMessage* schedEnd;
    std::string distribution;
    double lambda;

    // stats
    double nbArrived;
    double nbDropped;
    double nbProcessed;
};

/**
 */
class GenNode : public cSimpleModule
{
  protected:
    cMessage * previousMsg;

    int lambda1, lambda2, lambda3;
    int capacityPerServer;
    int queueLimit;

    cPacketQueue queue;

    int nbServers;
    std::vector<trafGen> trafGens;
    std::vector<cMessage*> serverEnds;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
};



#endif
