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

#include "storagewindow.h"
#include "driftsource.h"
#include <exception>

#include <iostream>

using namespace std;

StorageWindow::StorageWindow(const HardwareClock::Properties& properties, Driftsource* source)
	: properties(properties)
	, source(source)
{
	data.resize(properties.s());

	std::vector<HoldPoint>::iterator it = data.begin();

	driftVector.setName("drift");
	timeVector.setName("hardware_time");
	deviationVector.setName("time_deviation");
	driftHistogram.setName("drift_values");

	timeVector.setUnit("s");
	deviationVector.setUnit("s");

	simtime_t now = simTime();

	it->realTime = now;
	it->hardwareTime = now;
	it->drift = source->nextValue();

	recordVectors(now, now, it->drift);

	fillRange(it + 1, data.end());
}

StorageWindow::~StorageWindow()
{
	delete source;
}

void StorageWindow::finish()
{
	driftHistogram.recordAs("drift_distribution");
}

void StorageWindow::update()
{
	data = std::vector<HoldPoint>(data.begin() + properties.u(), data.end());
	data.resize(properties.s());

	fillRange(data.begin() + (properties.s() - properties.u()), data.end());
}

void StorageWindow::fillRange(std::vector<HoldPoint>::iterator first, std::vector<HoldPoint>::iterator last)
{
	while (first != last) {
		std::vector<HoldPoint>::iterator pre = first - 1;

		first->realTime = pre->realTime + properties.tint();
		first->hardwareTime = pre->hardwareTime + properties.tint() * (1 + pre->drift);
		first->drift = source->nextValue();

		recordVectors(first->realTime, first->hardwareTime, first->drift);

		first++;
	}

	_hardwareTimeEnd = data[data.size() - 1].hardwareTime + properties.tint() * (1 + data[data.size() - 1].drift);

}

void StorageWindow::recordVectors(const simtime_t& realTime, const simtime_t& hardwareTime, double drift)
{
	driftHistogram.collect(drift);

	driftVector.recordWithTimestamp(realTime, drift);
	timeVector.recordWithTimestamp(realTime, hardwareTime);
	deviationVector.recordWithTimestamp(realTime, hardwareTime - realTime);
}

const StorageWindow::HoldPoint& StorageWindow::at(size_t idx) const
{
	if (idx > data.size() - 1)
		throw std::logic_error("StorageWindow::HoldPoint: index out of bounds");

	return data[idx];
}

size_t StorageWindow::indexOf(const simtime_t& t) const
{
	return (t - data[0].realTime) / properties.tint();
}
