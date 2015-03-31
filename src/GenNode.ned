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

package ms.src;

simple GenNode
{
    parameters:
        @node();
        @display("i=device/hub");
        int lambda1 = default(1); // number of messages per second
        int lambda2 = default(1); // number of messages per second
        int lambda3 = default(1); // number of messages per second
        
        int capacityPerServer = default(10);
        int queueLimit = default(50);
    gates:
        
        inout toServers[];
        
}
