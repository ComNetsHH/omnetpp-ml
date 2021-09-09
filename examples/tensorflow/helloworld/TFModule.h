//
// Copyright (c) 2021 Institute of Communication Networks (ComNets),
//                    Hamburg University of Technology (TUHH),
//                    https://www.tuhh.de/comnets
// Copyright (c) 2021 Daniel Stolpmann <daniel.stolpmann@tuhh.de>
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
// This file incorporates work covered by the following copyright and
// permission notice:
//
//     Copyright (c) 2018 Patrick Wieschollek <mail@patwie.com>
//
//     Licensed under the Apache License, Version 2.0 (the "License");
//     you may not use this file except in compliance with the License.
//     You may obtain a copy of the License at
//
//         http://www.apache.org/licenses/LICENSE-2.0
//
//     Unless required by applicable law or agreed to in writing, software
//     distributed under the License is distributed on an "AS IS" BASIS,
//     WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//     See the License for the specific language governing permissions and
//     limitations under the License.
//

// Based on: https://github.com/PatWie/tensorflow-cmake/blob/master/examples/simple/example.cc

#ifndef TFMODULE_H_
#define TFMODULE_H_

#include <omnetpp.h>

using namespace omnetpp;

class TFModule : public cSimpleModule {
protected:
    virtual void initialize() override;
};

#endif /* TFMODULE_H_ */
