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

#ifndef STATS_COLLECTOR_H
#define STATS_COLLECTOR_H

#include <omnetpp.h>
#include <string>
#include <vector>

#define SC_NB_CELL_PDF 2

struct SCPDFCell
{
    double lower;
    double upper;
    double cnt;
    double probability;
};

// Generic OMNeT class for collecting statistics.
class StatsCollector: public cSimpleModule
{


protected:
	/// Initializes the module.
	virtual void initialize();
	virtual void finish();

	cDoubleHistogram statsSyncError;
	std::vector<double> statsSyncErrorRaw;
	std::vector<SCPDFCell> pdfSyncError;

	cOutVector deviationVector;

public:

	void addSyncError(double syncError);

	double backgroundTrafficDownstreamOfferedLoad;
	double backgroundTrafficUpstreamOfferedLoad;
	int backgroundTrafficMessageLength;
	double extraPTPUpstreamDelay;
	int nbPTPDuppToSend;

	static StatsCollector* findStatsCollector(const cModule* previous);

};

#endif
