/*
    Clock Synchronization Simulation in OMNet++
    Copyright (C) 2012 Jens Steinhauser (https://github.com/JenSte/omnet-ptp)

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

#include "driftsource.h"


#include <iostream>

using namespace std;

Driftsource::~Driftsource()
{
}

double Driftsource::nextValue()
{
	const double driftLimit = -0.999999;

	double n = next();

	// limit drift to values > -1, so the time can't go back
	if (n < driftLimit)
		return driftLimit;

	return n;
}

ConstantDrift::ConstantDrift(double drift)
	: drift(drift)
{
}

double ConstantDrift::next()
{
	return drift;
}

BoundedDrift::BoundedDrift(const cPar& distribution)
	: distribution(distribution)
{
}

double BoundedDrift::next()
{
	return distribution.doubleValue();
}

BoundedDriftVariation::BoundedDriftVariation(const cPar& distribution, double max_drift_variation, double start_value, const simtime_t& tint)
	: BoundedDrift(distribution)
	, max_drift_change(tint.dbl() * max_drift_variation)
	, last_drift(start_value)
{
}

double BoundedDriftVariation::next()
{
	double drift = BoundedDrift::next();

	double diff = drift - last_drift;

	// limit the drift
	     if (diff >  max_drift_change)
		drift = last_drift + max_drift_change;
	else if (diff < -max_drift_change)
		drift = last_drift - max_drift_change;

	last_drift = drift;
	return drift;
}
