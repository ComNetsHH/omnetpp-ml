//
// Copyright (c) 2022 Institute of Communication Networks (ComNets),
//                    Hamburg University of Technology (TUHH),
//                    https://www.tuhh.de/comnets
// Copyright (c) 2022 Daniel Stolpmann <daniel.stolpmann@tuhh.de>
//
// Distributed under the MIT License.
// (See accompanying LICENSE file or at
//  https://opensource.org/licenses/MIT)
//

#ifndef FDMODULE_H_
#define FDMODULE_H_

#include <omnetpp.h>

using namespace omnetpp;

class FDModule : public cSimpleModule {
protected:
    virtual void initialize() override;
};

#endif /* FDMODULE_H_ */
