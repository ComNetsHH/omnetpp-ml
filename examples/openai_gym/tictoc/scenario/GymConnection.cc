//
// Copyright (C) 2020 Dominik S. Buse <buse@ccs-labs.org>, Max Schettler <schettler@ccs-labs.org>
//
// Documentation for these modules is at http://veins.car2x.org/
//
// SPDX-License-Identifier: GPL-2.0-or-later
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//

#include "GymConnection.h"

Define_Module(GymConnection);

void GymConnection::initialize()
{
    // determine host and port from params and ENV variables
    std::string host = par("host");
    int port = par("port");
    if (host == "") { // param empty, check environment
        if (std::getenv("VEINS_GYM_HOST") != nullptr) {
            host = std::getenv("VEINS_GYM_HOST");
        } else {
            // param and environment empty, fail
            throw omnetpp::cRuntimeError("Gym host not configured! Change your ini file or set VEINS_GYM_HOST");
        }
    }

    if (port < 0) { // param "empty", check environment
        if (std::getenv("VEINS_GYM_PORT") != nullptr) {
            port = std::atoi(std::getenv("VEINS_GYM_PORT"));
        } else {
            // param and environment empty, fail
            throw omnetpp::cRuntimeError("Gym port not configured! Change your ini file or set VEINS_GYM_PORT");
        }
    }

    EV_INFO << "Connecting to server 'tcp://" << host << ":" << port << "'\n";
    socket.connect("tcp://" + host + ":" + std::to_string(port));

	veinsgym::proto::Request init_request;
	*(init_request.mutable_init()->mutable_observation_space_code()) = par("observation_space").stdstringValue();
	*(init_request.mutable_init()->mutable_action_space_code()) = par("action_space").stdstringValue();
	communicate(init_request); // ignore (empty) reply
}

veinsgym::proto::Reply GymConnection::communicate(veinsgym::proto::Request request)
{
    std::string request_msg = request.SerializeAsString();
    socket.send(zmq::message_t(request_msg.data(), request_msg.size()), zmq::send_flags::none);
    zmq::message_t response_msg;
    socket.recv(response_msg);
    std::string response(static_cast<char*>(response_msg.data()), response_msg.size());
    veinsgym::proto::Reply reply;
    reply.ParseFromString(response);
    return reply;
}

GymConnection::~GymConnection()
{
    veinsgym::proto::Request request;
    request.set_id(1);
    *(request.mutable_shutdown()) = {};
    auto response = communicate(request);
}
