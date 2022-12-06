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
// This file incorporates work covered by the following copyright and
// permission notice:
//
//     Copyright (c) 2016 Tobias Hermann
//
//     Distributed under the MIT License.
//     (See accompanying LICENSE file or at
//      https://opensource.org/licenses/MIT)
//

#include "FDModule.h"

#include <fdeep/fdeep.hpp>

Define_Module(FDModule);

void FDModule::initialize() {
    const auto model = fdeep::load_model("fdeep_model.json");
    const auto result = model.predict(
        {fdeep::tensor(fdeep::tensor_shape(static_cast<std::size_t>(4)),
        std::vector<float>{1, 2, 3, 4})});
    EV_INFO << fdeep::show_tensors(result);
}
