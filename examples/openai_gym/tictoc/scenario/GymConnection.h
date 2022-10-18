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

#pragma once

#include <zmq.hpp>
#include <omnetpp.h>
#include "veinsgym.pb.h"


class GymConnection : public omnetpp::cSimpleModule {
public:
    void initialize() override;
    veinsgym::proto::Reply communicate(veinsgym::proto::Request request);
    ~GymConnection();
private:
    zmq::context_t context = zmq::context_t(1);
    zmq::socket_t socket = zmq::socket_t(context, zmq::socket_type::req);
};
