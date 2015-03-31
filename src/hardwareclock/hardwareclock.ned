//
//  Clock Synchronization Simulation in OMNet++
//  Copyright (C) 2012 Jens Steinhauser (https://github.com/JenSte/omnet-ptp)
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along
//  with this program; if not, write to the Free Software Foundation, Inc.,
//  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//

package ptp.src.hardwareclock;

moduleinterface IHardwareClock
{
}

simple HardwareClock {
	parameters:
		double interval @unit(s) = default(1s);
		int update = default(100);
		@display("i=device/clock");
}

simple ConstantDrift extends HardwareClock like IHardwareClock {
	parameters:
		interval = 1s;
		double constant_drift = default(50e-6);
}

simple BoundedDrift extends HardwareClock like IHardwareClock {
	parameters:
		volatile double drift_distribution = default(uniform(-100e-6, 100e-6));
}

simple BoundedDriftVariation extends HardwareClock like IHardwareClock {
	parameters:
		volatile double drift_distribution = default(uniform(-100e-6, 100e-6));
		double start_value = default(0.0);
		double max_drift_variation = default(1e-8);
}
