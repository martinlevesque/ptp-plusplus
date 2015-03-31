//
//  ptp++
//  Copyright (C) 2015 Martin Levesque <levesque.martin@gmail.com>
//
//  Adapted from Jens Steinhauser https://github.com/JenSte/omnet-ptp
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

package ms.simulation;

import ms.src.GenNode;
import ms.src.Server;


network MultiServNet
{
    parameters:

		// Nb of servers:
        int M = default(4);
        int capacityPerServer = default(60);
        
        int lambda1 = default(10);
        int lambda2 = default(2);
        int lambda3 = default(4);

        @display("bgb=726,409");
    submodules:
        genAndQueue: GenNode 
        {
            parameters:
                @display("p=176,327");
                lambda1 = lambda1;
                lambda2 = lambda2;
                lambda3 = lambda3;
                capacityPerServer = capacityPerServer;
                queueLimit = 50;
        }

        server[M]: Server 
        {
            @display("p=249,112,r,100");
        }
    connections:

        for i=0..M - 1 {
            genAndQueue.toServers++ <--> server[i].toGenNodes++;
        }
}

