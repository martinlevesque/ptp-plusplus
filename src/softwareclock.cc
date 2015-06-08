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

#include "softwareclock.h"
#include "hardwareclock.h"
#include "StatsCollector.h"
#include <exception>

#include <iostream>
#include <utility>
using namespace std;

Define_Module(SoftwareClock);


void SoftwareClock::initialize()
{
	clock = HardwareClock::findFirstClock(getParentModule());

	ptpImplementationType = par("ptpImplementationType").stringValue();
	factor = 0.0;

	factorLimit = 0.05;

	WATCH_RW(factor);

	deviationVector.setName("time_deviation");
	deviationVector.setUnit("s");

	factorVector.setName("factor");

	statsCollector = StatsCollector::findStatsCollector(getParentModule());

	if ( ! statsCollector)
	{
	    error("There is no StatsCollector !");
	}

	nbSyntonizationPoints = par("nbSyntonizationPoints").longValue();

}

// TODO: improve
void SoftwareClock::addSyntoPoint(double tx, double rx)
{

    syntoPointsTX.push_back(tx);
    syntoPointsRX.push_back(rx);

    if (syntoPointsTX.size() > nbSyntonizationPoints)
    {
        syntoPointsTX.erase(syntoPointsTX.begin());
        syntoPointsRX.erase(syntoPointsRX.begin());
    }
}

void SoftwareClock::printSyntoPoints()
{
    list<double>::iterator itTX = syntoPointsTX.begin();
    list<double>::iterator itRX = syntoPointsRX.begin();

    for (int i = 0; i < syntoPointsTX.size(); ++i)
    {
        ++itTX;
        ++itRX;
    }
}

// TODO: improve
// return -1 if invalid, frequency otherwise
double SoftwareClock::calcSyntoFreq()
{
    if (syntoPointsTX.size() <= 1)
    {
        return -1;
    }

    // potential novelty: make a linear regression, how ? TODO

    double numerator = syntoPointsTX.back() - syntoPointsTX.front();
    double denominator = syntoPointsRX.back() - syntoPointsRX.front();

    ASSERT(denominator > 0);

    return numerator / denominator;
}

pair<SimTime, SimTime> SoftwareClock::getSWtime()
{
	SimTime swt = clock->getHWtime() + factor;

	SimTime tCor = swt;

	double freq = calcSyntoFreq();

	if (freq > 0)
	{
	    tCor *= freq; // TODO if used, should take into account AMOUNT OF TIME SINCE LAST POINT !
	}

	deviationVector.record(tCor - simTime());

	if (ptpImplementationType == "PtpSlave")
	{
	    statsCollector->addSyncError((tCor - simTime()).dbl());
	}



	return make_pair(swt, swt);
}

void SoftwareClock::setFactor(double f)
{
	if (f >  factorLimit)
		f =  factorLimit;
	else if (f < -factorLimit)
		f = -factorLimit;

	factor = f;

	factorVector.record(f);
}

SoftwareClock* SoftwareClock::findFirstClock(const cModule* parent)
{
	for (cModule::SubmoduleIterator i(parent); !i.end(); i++) {
		cModule* child = i();

		if (0 == strcmp("SoftwareClock", child->getClassName()))
			return check_and_cast<SoftwareClock*>(child);
	}

	throw std::logic_error("no SoftwareClock found");
}
