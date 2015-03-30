/*
    ptp++
    Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>

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

// std dev
// http://www.omnetpp.org/doc/omnetpp/api//classcHistogram.html

#include "StatsCollector.h"
#include <exception>
#include <fstream>
#include <iomanip>

#include <iostream>
using namespace std;

Define_Module(StatsCollector);


void StatsCollector::initialize()
{
	deviationVector.setName("sw_clock_deviation");
	deviationVector.setUnit("s");

	backgroundTrafficDownstreamOfferedLoad = par("backgroundTrafficDownstreamOfferedLoad").doubleValue();
	backgroundTrafficUpstreamOfferedLoad = par("backgroundTrafficUpstreamOfferedLoad").doubleValue();
	backgroundTrafficMessageLength = par("backgroundTrafficMessageLength").longValue();
	nbPTPDuppToSend = par("nbPTPDuppToSend").longValue();
	;

	extraPTPUpstreamDelay = par("extraPTPUpstreamDelay").doubleValue();

	// the cells could be improved.
	// statsSyncError.setRangeAutoUpper(0);
	// statsSyncError.setNumCells(10000);


}

void StatsCollector::finish()
{
    /////////////////////////////////////
    // Overall sync error stats:
    std::ofstream outFileGeneralSyncStats;

    outFileGeneralSyncStats << fixed << showpoint;
    outFileGeneralSyncStats << setprecision(12);

    outFileGeneralSyncStats.open("GeneralSyncStats.txt", std::ios_base::app);

    outFileGeneralSyncStats << nbPTPDuppToSend << " " << extraPTPUpstreamDelay << " " << backgroundTrafficUpstreamOfferedLoad << " " << backgroundTrafficDownstreamOfferedLoad << " " << statsSyncError.getMean() << " " << statsSyncError.getStddev() << " " << statsSyncError.getMin() << " " << statsSyncError.getMax() << statsSyncError.getCount() << endl;

    outFileGeneralSyncStats.close();

    /////////////////////////////////////
    // PDF sync error stats:

    stringstream filename;
    filename << fixed << setprecision(0);
    filename << "CDFSyncStats_.txt";
    std::ofstream outFilePDFSyncStats;

    outFilePDFSyncStats << fixed << showpoint;
    outFilePDFSyncStats << setprecision(12);

    outFilePDFSyncStats.open(filename.str(), std::ios_base::app);


    double sumCellCounts = 0;

    for (int k = 0; k < statsSyncError.getNumCells(); ++k)
    {
        sumCellCounts += statsSyncError.getCellInfo(k).value;
    }

    double curSumCellCounts = 0;

    for (int k = 0; k < statsSyncError.getNumCells(); ++k)
    {
        if (statsSyncError.getCellInfo(k).value > 0)
        {
            curSumCellCounts += statsSyncError.getCellInfo(k).value;
            outFilePDFSyncStats << k+1 << " " << statsSyncError.getCellInfo(k).lower << " " << statsSyncError.getCellInfo(k).upper << " " << statsSyncError.getCellInfo(k).value << " " << (curSumCellCounts / sumCellCounts) << endl;
        }
    }

    outFilePDFSyncStats.close();
}

void StatsCollector::addSyncError(double syncError)
{

    double absError = fabs(syncError);

    statsSyncError.collect(absError);
    statsSyncErrorRaw.push_back(absError);

    deviationVector.record(absError);
}

StatsCollector* StatsCollector::findStatsCollector(const cModule* parent)
{
    // Look for the stats collector in "parent"
    for (cModule::SubmoduleIterator i(parent); !i.end(); i++) {
        cModule* child = i();

        if (0 == strcmp("StatsCollector", child->getClassName()))
            return check_and_cast<StatsCollector*>(child);


    }

    // recursively go upper
    cModule* p2 = parent->getParentModule();

    if (p2)
    {
        return findStatsCollector(p2);
    }

    return NULL;
}
