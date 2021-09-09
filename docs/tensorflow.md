# TensorFlow

## Docker
The [tensorflow_cc](https://github.com/FloopCZ/tensorflow_cc/) project provides Docker images that contain a dynamically linkable TensorFlow C++ library.
By installing OMNeT++ into these images, TensorFlow can be used in OMNeT++.

Dockerfiles for images that contain TensorFlow and OMNeT++ can be found in the following GitHub repository: https://github.com/ComNetsHH/omnetpp-ml_dockerfiles

### Usage
#### IDE
To link a project against TensorFlow, the following settings have to be made:
 * Right-click on the project and select *Properties*
 * Select *OMNeT++* -> *Makemake*
 * Select the project folder and then *Options...* on the right-hand side
 * Go to the *Compile* tab and add the following include directories:
   * `/usr/local/include/tensorflow/bazel-bin/tensorflow/include`
   * `/usr/local/include/tensorflow/bazel-bin/tensorflow/include/src`
   * `/usr/local/include/tensorflow/bazel-bin/bin/tensorflow/include`
   * `/usr/local/include/tensorflow/bazel-bin/bin/tensorflow/include/src`
 * Go to the *Link* tab and add the following libraries (in the *more* section):
   * `tensorflow_cc`
   * `protobuf`

#### CLI
To link a project against TensorFlow, add the following options to the *opp_makemake* command:
```
-I/usr/local/include/tensorflow/bazel-bin/tensorflow/include -I/usr/local/include/tensorflow/bazel-bin/tensorflow/include/src -I/usr/local/include/tensorflow/bazel-bin/bin/tensorflow/include -I/usr/local/include/tensorflow/bazel-bin/bin/tensorflow/include/src -ltensorflow_cc -lprotobuf
```

## Arch Linux
The Arch Linux TensorFlow packages [tensorflow](https://archlinux.org/packages/community/x86_64/tensorflow/), [tensorflow-opt](https://archlinux.org/packages/community/x86_64/tensorflow-opt/), [tensorflow-cuda](https://archlinux.org/packages/community/x86_64/tensorflow-cuda/) and [tensorflow-opt-cuda](https://archlinux.org/packages/community/x86_64/tensorflow-opt-cuda/) contain dynamically linkable TensorFlow C++ libraries. These can be used directly in OMNeT++.

### Usage
#### IDE
To link a project against TensorFlow, the following settings have to be made:
 * Right-click on the project and select *Properties*
 * Select *OMNeT++* -> *Makemake*
 * Select the project folder and then *Options...* on the right-hand side
 * Go to the *Compile* tab and add the following include directories:
   * `/usr/include/tensorflow`
 * Go to the *Link* tab and add the following libraries (in the *more* section):
   * `tensorflow_cc`
   * `tensorflow_framework`

#### CLI
To link a project against TensorFlow, add the following options to the *opp_makemake* command:
```
-I/usr/include/tensorflow -ltensorflow_cc -ltensorflow_framework
```
