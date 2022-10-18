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

// Based on: https://github.com/omnetpp/omnetpp/blob/master/samples/tictoc/txc1.cc

#include <cstring>
#include <omnetpp.h>

#include "GymConnection.h"

using namespace omnetpp;

/**
 * Derive the Txc1 class from cSimpleModule. In the Tictoc1 network,
 * both the `tic' and `toc' modules are Txc1 objects, created by OMNeT++
 * at the beginning of the simulation.
 */
class Txc1 : public cSimpleModule
{
  protected:
    // The following redefined virtual function holds the algorithm.
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// The module class needs to be registered with OMNeT++
Define_Module(Txc1);

void Txc1::initialize()
{
    // Initialize is called at the beginning of the simulation.
    // To bootstrap the tic-toc-tic-toc process, one of the modules needs
    // to send the first message. Let this be `tic'.

    // Am I Tic or Toc?
    if (strcmp("tic", getName()) == 0) {
        // create and send first message on gate "out". "tictocMsg" is an
        // arbitrary string which will be the name of the message object.
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
    }
}

void Txc1::handleMessage(cMessage *msg)
{
    // The handleMessage() method is called whenever a message arrives
    // at the module. Here, we just send it to the other module, through
    // gate `out'. Because both `tic' and `toc' does the same, the message
    // will bounce between the two.

    if (strcmp("tic", getName()) == 0) {
        veinsgym::proto::Request request;
        request.set_id(1);

        std::array<double, 1> observation = {0.5};
        auto *values = request.mutable_step()->mutable_observation()->mutable_box()->mutable_values();
        *values = {observation.begin(), observation.end()};

        double reward = 0.1;
        request.mutable_step()->mutable_reward()->mutable_box()->mutable_values()->Add();
        request.mutable_step()->mutable_reward()->mutable_box()->set_values(0, reward);

        cModule *gym_connection = getModuleByPath("gym_connection");
        auto response = dynamic_cast<GymConnection*>(gym_connection)->communicate(request);

        EV_INFO << "Action: " << response.action().discrete().value() << "\n";

        send(msg, "out"); // send out the message
    } else {
        send(msg, "out"); // send out the message
    }
}
