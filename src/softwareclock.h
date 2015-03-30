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

#ifndef SOFTWARECLOCK_H
#define SOFTWARECLOCK_H

#include <omnetpp.h>
#include "StatsCollector.h"
#include <string>
#include <list>
#include <utility>

class HardwareClock;

/// Module to add a correction factor to a hardware clock.
///
/// This module uses the first found HardwareClock module
/// in the same compound module and applies a correction
/// factor to the hardware timestamps.
class SoftwareClock: public cSimpleModule
{
	HardwareClock* clock;

	// factor to correct the hardware time
	double factor;

	std::string ptpImplementationType;

	// limit for the correction factor
	double factorLimit;

	/// Vector to record the deviation between the software
	/// and simulation time.
	cOutVector deviationVector;

	/// Vector to record the factor.
	cOutVector factorVector;

	StatsCollector* statsCollector;

	int nbSyntonizationPoints;
	std::list<double> syntoPointsTX;
	std::list<double> syntoPointsRX;

private:
	double calcSyntoFreq();

protected:
	/// Initializes the module.
	virtual void initialize();

public:

	void addSyntoPoint(double tx, double rx);
	void printSyntoPoints();


	/// \returns	The hardware time corrected by the factor (= software time).
	std::pair<SimTime, SimTime> getSWtime();

	/// Sets the correction factor.
	/// \param f	The new value for the correction factor.
	void setFactor(double f);

	/// \returns	A pointer to the first found software clock in the compound module 'parent',
	//		throws a std::logic_error if there is no hardware clock.
	static SoftwareClock* findFirstClock(const cModule* parent);
};

#endif
