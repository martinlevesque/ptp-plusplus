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

#ifndef STORAGEWINDOW_H
#define STORAGEWINDOW_H

#include <omnetpp.h>
#include "hardwareclock.h"

class Driftsource;

/// Saves the data points for the continuous linear approximation of the drift function.
class StorageWindow
{
public:
	/// Represents a hold point of the continuous linear approximation.
	struct HoldPoint
	{
		/// The simulation time at the point.
		simtime_t realTime;

		/// The hardware time at the point.
		simtime_t hardwareTime;

		/// The drift value between this and the next point.
		double drift;
	};

private:
	/// Holds the data of the approximation.
	std::vector<HoldPoint> data;

	/// The properties of the clock this object belongs to.
	const HardwareClock::Properties& properties;

	/// The object that supplies the drift values to fill
	/// the data vector.
	Driftsource* source;

	/// Vector to record the drift values.
	cOutVector driftVector;

	/// Vector to record the hardware timestamps.
	cOutVector timeVector;

	/// Vector to record the deviation between the hardware
	/// and simulation time.
	cOutVector deviationVector;

	// Collects statistics about the drift values.
	cDoubleHistogram driftHistogram;

	/// The hardware timestamp of the first point after the
	/// storage window.
	simtime_t _hardwareTimeEnd;

	/// Fills the range [first, last) with new timestamp/drift values.
	void fillRange(std::vector<HoldPoint>::iterator first, std::vector<HoldPoint>::iterator last);

	/// Records the given values to the vector files.
	///
	/// \param realTime	The simulation timestamp, also used to calculate the time deviation.
	///			Has to be increasing between two calls to the function.
	/// \param hardwareTime	The hardware timestamp at realTime.
	/// \param drift	The drift value at realTime.
	void recordVectors(const simtime_t& realTime, const simtime_t& hardwareTime, double drift);

public:
	/// Initializes the storage window.
	///
	/// \param properties	Properties object of the simulated hardware clock, used
	///			to determine things like the length of the storage window, etc.
	/// \param source	Pointer to a source of drift values, the StorageWindow object
	///			takes ownership of the object being passed.
	StorageWindow(const HardwareClock::Properties& properties, Driftsource* source);

	~StorageWindow();

	/// Writes out statistics.
	void finish();

	/// Updates the storage window.
	///
	/// The first u values in the storge window are discarded, the
	/// remaining ones are moved to the front and the empty space
	/// is filled up with new values.
	void update();

	/// Returns the time at the beginning of the storage window.
	const simtime_t& hardwareTimeBegin() const { return data[0].hardwareTime; }

	/// Returns the time at the end of the storage window.
	const simtime_t& hardwareTimeEnd() const { return _hardwareTimeEnd; }

	/// Returns the hold point at index idx.
	///
	/// If the index is out of bounds, an std::logic_error exception is thrown.
	/// \param idx	The index of the requested hold point.
	/// \returns	The hold point at index idx.
	const StorageWindow::HoldPoint& at(size_t idx) const;

	/// Calculates the hold point index for a timestamp.
	///
	/// \param t	A simulation timestamp.
	/// \returns	The index of the hold point in what the simulation time lies.
	size_t indexOf(const simtime_t& t) const;
};

#endif
