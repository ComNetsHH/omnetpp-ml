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

simple FDModule
{
}

network Network
{
    submodules:
        fd_module: FDModule;
}
