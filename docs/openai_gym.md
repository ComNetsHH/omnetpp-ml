# OpenAI Gym

In [[3]](/references.md), M. Schettler et al. introduced an OpenAI Gym wrapper for the OMNeT++ based Veins simulator called *Veins-Gym*.
The source code of the wrapper can be found on GitHub [here](https://github.com/tkn-tub/veins-gym) and an example project [here](https://github.com/tkn-tub/serpentine-env).
Even though the wrapper is designed for the Veins simulator, it can be used for any OMNeT++ simulation.

The wrapper is written in Python and consists of a class that implements the OpenAI Gym environment interface.
This interface was introduced to standardize the way reinforcement learning agents interact with an environment to allow researchers to combine different reinforcement learning agents and environments.

Veins-Gym starts a Veins/OMNeT++ simulation binary and communicates with it via [ZeroMQ](https://zeromq.org) over TCP.
All messages are serialized using [Protocol Buffers](https://developers.google.com/protocol-buffers).

## Prerequisites
### C++
On the C++/OMNeT++ side, the following libraries are required besides a working OMNeT++ installation:
 * [ZeroMQ](https://zeromq.org)
 * [Protocol Buffers](https://developers.google.com/protocol-buffers) incl. compiler

On Linux, these libraries can be installed via the package manager of your distribution. For example, on Ubuntu 22.04 LTS, they can be installed using the following command:
```bash
sudo apt-get install libzmq3-dev libprotobuf-dev protobuf-compiler
```

### Python
On the Python side, the following modules are required:
 * [veins-gym](https://pypi.org/project/veins-gym/)
 * [gym](https://pypi.org/project/gym/)
 * [protobuf](https://pypi.org/project/protobuf/) (Ideally, the version of the Python package should match the version of the Protocol Buffers compiler and C++ library)

The modules can be installed easily via pip.
Alternatively, a Pipfile for setting up a Python virtual environment via Pipenv can be found [here](/examples/openai_gym/tictoc/Pipfile).

## Usage
### Folder Structure
The following folder structure is assumed:
 * main.py
 * scenario
   * run
   * Makefile
   * omnetpp.ini
   * network.ned
   * GymConnection.cc
   * GymConnection.h
   * GymConnection.ned
   * veinsgym.pb.cc
   * veinsgym.pb.h
   * ...
 * protobuf
   * veinsgym.proto

The meaning and content of the different files will be explained later.

### Writing the Python Code
In order to use Veins-Gym as an environment, it first has to be registered in the Python code (`main.py`) using the following code snippet:
```python
import gym
import veins_gym

gym.register(
  id="veins-v1",
  entry_point="veins_gym:VeinsEnv",
  kwargs={
    "scenario_dir": "scenario",
    "run_veins": True,
    "port": None,
    "timeout": 3.0,
    "print_veins_stdout": True,
    "action_serializer": veins_gym.serialize_action_discete,
    "veins_kwargs": None,
    "user_interface": "Cmdenv",
    "config": "General"
  }
)
```

The following parameters can be set in the kwargs dictionary:
 * `scenario_dir` defines the directory were the OMNeT++ binary can be found relative to the current working directory (in this case "scenario").
 * `run_veins` defines whether the OMNeT++ binary should be launched by Veins-Gym (defaults to true). If false, the simulation has to be started separately.
 * `port` defines the TCP port for communicating with OMNeT++ (defaults to a random port that will be passed to the OMNeT++ binary as a command-line argument).
 * `timeout` defines the timeout in seconds for waiting for a reply from OMNeT++ (defaults to 3s).
 * `print_veins_stdout` defines whether the output of OMNeT++ should be printed to the command-line (defaults to false).
 * `action_serializer` defines the action serializer that should be used (defaults to the standard serializer for discrete actions).
 * `veins_kwargs` defines extra command-line arguments that should be passed to the OMNeT++ binary (defaults to none).
 * `user_interface` defines the OMNeT++ user interface that should be used (defaults to "Cmdenv").
 * `config` defines the config from the omnetpp.ini that should be executed (defaults to "General").

The environment can then be created using:
```python
env = gym.make("veins-v1")
```

It can now be used in the standard OpenAI Gym manner:
```python
done = False
observation = env.reset()
while not done:
    action = env.action_space.sample()
    observation, reward, done, info = env.step(action)
```
`env.action_space.sample()` should be replaced by the call to the agent to get the next action based on the last received observation.

### Writing the OMNeT++ code
On the OMNeT++ side, the communication with Veins-Gym is handled by the GymConnection module that can be found [here](https://github.com/tkn-tub/serpentine-env/blob/master/src/serpentine/GymConnection.ned).
The module consists of the three files GymConnection.cc, GymConnection.h and GymConnection.ned.
The module has to be included globally in the network with the name `gym_connection`.
The name is fixed and cannot be changed.
This can be done by adding it to the `network.ned` file like this:
```
network Network
{
  submodules:
    gym_connection: GymConnection {
      parameters:
        observation_space = "gym.spaces.Box(low=np.array([-np.inf], dtype=np.float32), high=np.array([np.inf], dtype=np.float32))";
        action_space = "gym.spaces.Discrete(2)";
    }
}
```

The `observation_space` and `action_space` parameters define the dimensions and limits of the observations and actions as OpenAI Gym spaces.
The parameters contain Python code that will be sent to the Python side and evaluated there when calling reset().
A description of the OpenAI Gym spaces can be found [here](https://www.gymlibrary.dev/api/spaces/).
Please note that an `action_serializer` has to be provided on the Python side for non-trivial action spaces [[ref]](https://github.com/tkn-tub/veins-gym/blob/master/doc/getting_started.md#data-type-descriptions-of-the-interface-to-the-agent).

The module provides a single function called `communicate()` that can be called from any other module.
This function sends a request with the current observation and reward to the Python side and waits for a reply with the next action.
For this, the following code needs to be added to the module that interacts with the agent.

```c++
veinsgym::proto::Request request;
request.set_id(1);

std::array<double, 1> observation = {0.5};
auto *values = request.mutable_step()->mutable_observation()->mutable_box()->mutable_values();
*values = {observation.begin(), observation.end()};

double reward = 0.1;
request.mutable_step()->mutable_reward()->mutable_box()->mutable_values()->Add();
request.mutable_step()->mutable_reward()->mutable_box()->set_values(0, reward);

cModule *gym_connection = getModuleByPath("gym_connection");
auto reply = dynamic_cast<GymConnection*>(gym_connection)->communicate(request);

EV_INFO << "Action: " << reply.action().discrete().value() << "\n";
```

This code first creates a request.
The request contains an observation as a double array with a single element with the value 0.5 and a scalar reward of 0.1.
These are sent to the Python side upon which the OMNeT++ simulation blocks.
As soon the Python side sends its reply, which contains the next action, the execution of the OMNeT++ simulation is continued and the action is received.
The simulation is executed until the `communicate()` command is reached again.

### Compiling the Protocol Buffers Message Classes
In order for the OMNeT++ simulation to communicate with the Python side of the Veins-Gym, it needs to be aware of the message format.
As the communication is done by exchanging Protocol Buffers messages via TCP, the respective Protocol Buffers definition has to be converted into a C++ class.
The Protocol Buffers definition `veinsgym.proto` can be found in the [`protobuf` folder in the Veins-Gym repository](https://github.com/tkn-tub/veins-gym/tree/master/protobuf).
Under the assumption that the Protocol Buffers definition is located in a subdirectory called `protobuf` and the OMNeT++ simulation code is located in a directory called `scenario`, the conversion can be done using the following command:
```
protoc --proto_path protobuf --cpp_out scenario veinsgym.proto
```

### Building the OMNeT++ simulation binary
As the GymConnection module includes the ZeroMQ library and the generated Protocol Buffers class, the OMNeT++ project has to be linked to the respective libraries.
This can be done as follows using either the OMNeT++ IDE or the command-line:

#### IDE
Perform the following steps in the OMNeT++ IDE:
 * Right-click on the project and select *Properties*
 * Select *OMNeT++* -> *Makemake*
 * Select the project folder and then *Options...* on the right-hand side
 * Go to the *Link* tab and add the following libraries (in the *more* section):
   * `zmq`
   * `protobuf`

The simulation binary can then be build using the IDE.

#### CLI
Add the following options to the *opp_makemake* command:
```
-lzmq -lprotobuf
```
The simulation binary can then be build using the *make* command.

Assuming the OMNeT++ simulation code is located in a directory called `scenario`, the following commands are needed to build the simulation binary:
```bash
cd scenario
opp_makemake -lzmq -lprotobuf
make -j$(nproc)
cd ..
```

Please note that further options might have to be added to the *opp_makemake* command if frameworks such as INET are used.

### Running the Simulation
Veins-Gym executes a binary called `run` in the provided scenario directory.
By default, the following parameters are passed to the binary:
```
-uCmdenv -cGeneral --seed-set=0 --*.manager.seed=0 --*.gym_connection.port=32907
```

The -u and -c arguments set the OMNeT++ user interface and the config from the omnetpp.ini respectively.
These parameters can be changed during registration of the Gym environment in the Python code.
The seed can be set by calling `env.seed()`.

As the OMNeT++ binary is typically not called `run`, a simple helper script can be written and stored under this name without any file extension.
The script should look like this:
```bash
#!/bin/bash

exec ./scenario $@
```

This script passes all parameters to an OMNeT++ binary with the name `scenario`.
The name of the OMNeT++ binary can be changed if needed.

The script has to be made executable using the following command:
```bash
chmod +x run
```

Alternatively to the script, a symbolic link to the simulation binary can be used.

Now, the Python code in `main.py` can be executed, which should start the OMNeT++ simulation and interact with it.
