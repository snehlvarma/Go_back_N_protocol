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

simple Station_A
{
    parameters:
        @display("i=block/routing");
    gates:
        input in;
        output out;
}

simple Station_B
{
    parameters:
        @display("i=block/process");
        int window;
        
    gates:
        input in;
        output out;
}

network gobackn
{
    @display("bgb=274,139");
    submodules:
        Station_A : Station_A {
            parameters:
                @display("i=,white;p=41,61");
        }
        Station_B : Station_B {
            parameters:
                @display("i=,black;p=223,61");
                
        }
    connections:
        Station_A.out --> {  delay = 100ms; datarate = 100Mbps;} --> Station_B.in;
        Station_A.in <-- {  delay = 100ms; datarate = 100Mbps;} <-- Station_B.out;
}