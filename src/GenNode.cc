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

#include "GenNode.h"
#include "RandomUtil.h"
#include <string>
#include <iostream>

#include <cpacketqueue.h>
#include <fstream>
#include <iomanip>

using namespace std;

Define_Module(GenNode);

void GenNode::initialize()
{
    //

    lambda1 = par("lambda1");
    lambda2 = par("lambda2");
    lambda3 = par("lambda3");
    capacityPerServer = par("capacityPerServer");
    queueLimit = par("queueLimit");

    //scheduleAt(simTime() + RandomUtil::genInterval(1/ratePerSec, 0, distribution), scheduledMessage);

    nbServers = gateSize("toServers");

    for (int i = 0; i < nbServers; ++i)
    {
        serverEnds.push_back(new cMessage("schedEnd"));
    }

    trafGen t;

    t.schedEnd = new cMessage("poisson-lambda1");
    t.distribution = "poisson";
    t.lambda = lambda1;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("normal-lambda1");
    t.distribution = "normal";
    t.lambda = lambda1;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("pareto-lambda1");
    t.distribution = "pareto";
    t.lambda = lambda1;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("poisson-lambda2");
    t.distribution = "poisson";
    t.lambda = lambda2;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("normal-lambda2");
    t.distribution = "normal";
    t.lambda = lambda2;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("pareto-lambda2");
    t.distribution = "pareto";
    t.lambda = lambda2;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("poisson-lambda3");
    t.distribution = "poisson";
    t.lambda = lambda3;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("normal-lambda3");
    t.distribution = "normal";
    t.lambda = lambda3;
    trafGens.push_back(t);

    t.schedEnd = new cMessage("pareto-lambda3");
    t.distribution = "pareto";
    t.lambda = lambda3;
    trafGens.push_back(t);

    for (int i = 0; i < trafGens.size(); ++i)
    {
        scheduleAt(simTime().dbl() + RandomUtil::genInterval(1.0 / trafGens[i].lambda, 1.5, trafGens[i].distribution), trafGens[i].schedEnd);
        trafGens[i].nbArrived = 0;
        trafGens[i].nbDropped = 0;
        trafGens[i].nbProcessed = 0;
    }
}

void GenNode::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        for (int i = 0; i < serverEnds.size(); ++i)
        {
            if (msg == serverEnds[i])
            {
                // process next one
                if (queue.length() > 0)
                {
                    cPacket* p = queue.pop();

                    trafGens[i].nbProcessed += 1;
                    send(p, "toServers$o", i);
                    scheduleAt(simTime().dbl() + 1.0 / (double)capacityPerServer, serverEnds[i]);
                }
            }
        }

        for (int i = 0; i < trafGens.size(); ++i)
        {
            if (trafGens[i].schedEnd == msg)
            {
                trafGens[i].nbArrived += 1;

                // msg arrived
                cPacket* m = new cPacket(trafGens[i].schedEnd->getName());
                scheduleAt(simTime().dbl() + RandomUtil::genInterval(1.0 / trafGens[i].lambda, 1.5, trafGens[i].distribution), trafGens[i].schedEnd);

                bool processed = false;

                for (int j = 0; j < serverEnds.size(); ++j)
                {
                    if ( ! serverEnds[j]->isScheduled())
                    {
                        send(m, "toServers$o", j);
                        trafGens[i].nbProcessed += 1;
                        scheduleAt(simTime().dbl() + 1.0 / (double)capacityPerServer, serverEnds[j]);
                        processed = true;
                        break;
                    }
                }

                if ( ! processed)
                {
                    if (queue.length() >= queueLimit)
                    {
                        // dropped
                        trafGens[i].nbDropped += 1;
                        delete m;
                    }
                    else
                    {
                        // enqueue
                        queue.insert(m);
                    }
                }
            }
        }
    }
}

void GenNode::finish ()
{
    double totalLambda = 0;

    for (int i = 0; i < trafGens.size(); ++i)
    {
        totalLambda += trafGens[i].nbArrived;
    }

    double erlang = totalLambda / ((double)capacityPerServer * nbServers * simTime().dbl());

    for (int i = 0; i < trafGens.size(); ++i)
    {
        double blockingProbability = trafGens[i].nbDropped / trafGens[i].nbArrived;

        std::ofstream outFile;

        outFile << fixed << showpoint;
        outFile << setprecision(12);

        outFile.open(string(trafGens[i].schedEnd->getName()) + "-results.txt", std::ios_base::app);

        outFile << erlang << " " << blockingProbability << endl;

        outFile.close();
    }
}
