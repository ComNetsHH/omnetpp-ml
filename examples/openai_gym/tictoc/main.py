#!/usr/bin/env python3

#
# Copyright (c) 2022 Institute of Communication Networks (ComNets),
#                    Hamburg University of Technology (TUHH),
#                    https://www.tuhh.de/comnets
# Copyright (c) 2022 Daniel Stolpmann <daniel.stolpmann@tuhh.de>
# Copyright (c) 2021 Dominik S. Buse <buse@ccs-labs.org>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#

# Based on: https://github.com/tkn-tub/serpentine-env/blob/master/examples/random_agent.py

import gym
import veins_gym


def main():
    # Register veins-gym environment
    gym.register(
        id="veins-v1",
        entry_point="veins_gym:VeinsEnv",
        kwargs={
            "scenario_dir": "scenario",
            "print_veins_stdout": True
        },
    )

    # Initialize environment
    print("[Python] Initialize environment")
    env = gym.make("veins-v1")

    # Main loop
    print("[Python] Start")
    done = False
    rewards = []
    observation = env.reset()
    while not done:
        print("[Python] Observation:", observation)

        action = env.action_space.sample()
        print("[Python] Action:", action)

        observation, reward, done, info = env.step(action)

        rewards.append(reward)
        print("[Python] Reward:", reward)

        print("-------------------------")

    # Output statistics
    print("[Python] Number of steps taken:", len(rewards))
    print("[Python] Mean reward:", sum(rewards) / len(rewards))


if __name__ == "__main__":
    main()
