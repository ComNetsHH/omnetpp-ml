//
// This file is derived from an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 2022 Institute of Communication Networks (ComNets),
//                    Hamburg University of Technology (TUHH),
//                    https://www.tuhh.de/comnets
// Copyright (C) 2022 Daniel Stolpmann <daniel.stolpmann@tuhh.de>
// Copyright (C) 2003 Ahmet Sekercioglu
// Copyright (C) 2003-2015 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' of OMNeT++/OMNEST for details on this and other legal matters.
//

// Based on: https://github.com/omnetpp/omnetpp/blob/master/samples/tictoc/tictoc1.ned

simple Txc1
{
    gates:
        input in;
        output out;
}

//
// Two instances (tic and toc) of Txc1 connected both ways.
// Tic and toc will pass messages to one another.
//
network Tictoc1
{
    submodules:
        gym_connection: GymConnection {
            parameters:
                observation_space = "gym.spaces.Box(low=np.array([-np.inf], dtype=np.float32), high=np.array([np.inf], dtype=np.float32))";
                action_space = "gym.spaces.Discrete(2)";
        }

        tic: Txc1;
        toc: Txc1;
    connections:
        tic.out --> {  delay = 100ms; } --> toc.in;
        tic.in <-- {  delay = 100ms; } <-- toc.out;
}
