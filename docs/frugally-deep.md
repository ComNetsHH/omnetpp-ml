# frugally-deep

[frugally-deep](https://github.com/Dobiasd/frugally-deep) is a header-only library for performing inference on Keras models in C++.
The neural network can be created and trained using Python and TensorFlow/Keras.
The model will then be saved and converted into a frugally-deep specific JSON format.
Using a few simple C++ commands, this model can be loaded in an OMNeT++ simulation and a prediction can be performed.

## Prerequisites
As frugally-deep is a header-only library, it does not have to be built, but only downloaded and made available during compilation of the OMNeT++ project.
This repository contains frugally-deep and its required dependencies as Git submodules in the [/lib](/lib) folder.
To check them out, run the following commands:

```
git submodule init
git submodule update
```

Additionally, a working Python installation with the latest version of TensorFlow is required to create the neural network.

## Usage
### Creating and Training the Neural Network
The neural network can be created and trained using Python and TensorFlow/Keras.
For this, the Keras model will be created and trained as usual.
In the end, it will be saved using the `model.save()` command.

The example in the [/examples/frugally-deep](/examples/frugally-deep) folder contains the necessary Python script in its `python` folder.
The script should be executed from the root folder of the example by running the following command:
```
python python/create_model.py
```

Afterwards, there should be a new file in the current working directory called `keras_model.h5` that contains the trained neural network.

### Converting the Neural Network
In order to use the neural network that was created in the previous step, it has to be converted into a frugally-deep specific JSON format.
The necessary tool is included in the frugally-deep repository.
From the example project, this tool can be called to convert the neural network using the following command:

```
python ../../../lib/frugally-deep/keras_export/convert_model.py keras_model.h5 fdeep_model.json
```

### Calling the Neural Network in OMNeT++
To use the neural network in an OMNeT++ simulation, the following code has to be included at the respective place:
```c++
// Source: https://github.com/Dobiasd/frugally-deep
//         (Copyright (c) 2016 Tobias Hermann / MIT License)

#include <fdeep/fdeep.hpp>
...
const auto model = fdeep::load_model("fdeep_model.json");
const auto result = model.predict(
	{fdeep::tensor(fdeep::tensor_shape(static_cast<std::size_t>(4)),
	std::vector<float>{1, 2, 3, 4})});
std::cout << fdeep::show_tensors(result) << std::endl;
```

For performance reasons, it is advised to load the neural network only once and keep the instance of the model over the entire simulation.
The `predict()` function can then be called whenever it is needed.

### Building the OMNeT++ Simulation
To build the OMNeT++ simulation, the compiler has to be able to find the header files of frugally-deep and its dependencies.
Therefore, these paths have to be passed as search paths to the compiler.
How this is done depends on whether the simulation is build using the OMNeT++ IDE or the command-line.

#### IDE
To add the include directories of frugally-deep and its dependencies to the search path of the compiler, the following settings have to be made:
 * Right-click on the project and select *Properties*
 * Select *OMNeT++* -> *Makemake*
 * Select the project folder and then *Options...* on the right-hand side
 * Go to the *Compile* tab and add the following include directories:
   * `../../../lib/frugally-deep/include`
   * `../../../lib/eigen`
   * `../../../lib/FunctionalPlus/include`
   * `../../../lib/json/include`

#### CLI
To add the include directories of frugally-deep and its dependencies to the search path of the compiler, add the following options to the *opp_makemake* command:
```
-I../../../lib/frugally-deep/include -I../../../lib/eigen -I../../../lib/FunctionalPlus/include -I../../../lib/json/include
```

### Running the Simulation
The OMNeT++ simulation can now be built and executed as usual.

## Acknowledgement
This method was first presented in the session ["Inference of packet-error rates using SNIR patterns via neural networks" by Chimera Solutions
in partnership with the OMNeT++ Team](https://www.youtube.com/watch?v=ed1_mxNrQYs) ([Slides](https://summit.omnetpp.org/2022/assets/pdf/pres-nnwifi.pdf)) on the [OMNeT++ Community Summit 2022](https://summit.omnetpp.org/2022/).
