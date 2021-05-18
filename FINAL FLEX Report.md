# FINAL FLEX Report
Jose Chavez (jac23932) and Jeff Cochran (jdc6284)
Final Project Report: Physical Simulation for Computer Graphics CS384P

## Overview
Our  project  was  built  using  C++,  Box2D,  and SFML (as a submodule). Box2D, a 2D physics game engine, was used to handle collisions between objects while SFML was used to handle rendering. Below, we discuss more in detail our process for running scenarios.

## Building and Running
Building   the   code   requires `cmake` and the installed  Box2D  libraries. SFML  is  a  sub-module, so one   must   first   initialize   with `git submodule init` and then call `git submodule update`.

### Box2D on MAC
If   running   ona  Mac,   to  use  the  latest  version  of  Box2D, one  must  first  install  Box2D  using  Homebrew with `brew install box2d`. Unfortunately, Box2D has  not  updated  some  of  it’s  scripts for MacOS. Therefore,   before   building the code, one  must  specify  environment  variables `BOX2D_INCLUDE_DIR` and `BOX2D_LIB`, both of which are specified in the `CMakeList.txt` file. This is done with the following commands:

`export BOX2D_INCLUDE_DIR=/usr/local/Cellar/box2d/2.4.1/include/`
`export BOX2D_LIB=/usr/local/Cellar/box2d/2.4.1/lib/libbox2d.a`

### Box2D for Linux/Windows
`brew install box2d` will properly set up libraries.

### Building with CMake
Create a folder `build`, and inside that folder, run `cmake ..` or `cmake -G Xcode ..` if using XCode. 
Then, `make -j 8` inside of tbe build folder.

### Running `final_flex`
Go to the `/bin` folder. There you will see the executable `final_flex`.
To run the dogbone stretch scenario, , execute the following command.
```
./final_flex dogbone 0.4 1.0 0.00001 0.3 0.04 20.0 1
```

#### Arguments
1. The first argument is the scenario argument, of which there are 3:  `dogbone`, `vnotch`, and `lbracket`. For example:
`./final_flex lbracket 0.4 1.0 0.1 0.5 0.04 20.0 1`
and 
`./final_flex vnotch 0.4 1.0 0.1 0.5 0.04 20.0 1`

2. The particle density argument, which alters how dense particles are created in a blob.  A higher value  of  0.6  creates  more  particles whereas a value of 0.1 is the minimum number of particles to take up the space of the blob.

![](https://i.imgur.com/kh9HCKS.png)

The blob on the left has a density of 0.4 and the blob on the right has a density of 0.2.

4. The  time-step  value  used  in  the  simulation. A  passed  in  value  of  1.0  becomes 1.0 / 600.0 in the code.
5. The strain compliance value.
6. The fracture strain value (default 0.5).
7. The yield strain value (default 0.04).
8. The linear damping value (default 20.0).
9. The number of constraint iterations (default1).

## Description of Contents
#### third-party
Contains a git submodule of SFML. Must be initialized and updated using the steps above.

#### Scenario.hpp
This header contains the `struct ScenarioData`, which stores individual parameters that may get passed into different Scenarios. Upon initialization of a scenario, this struct will be populated and passed into a `Scenario` subclass. Some values contain default values.

#### scenario.hpp/.cpp
These files define the `Scenario` abstract class and the three subclasses: `LBracketBreak`, `VNotchBreak`, and `DogBoneStretch`. Each subclass overwrites a `run` virtual method. The subclasses takek in a `struct ScenarioData` and a `b2World`.

#### SphereBVH.hpp/.cpp
Defines a class that represents a BVH (Bounding Volume Hierachy) for elements in a `blob`. It is a top-down bounding volume hierarchy that uses spherical primitives. Its leaves are the particles of a `blob` and it is used to determine the particles that fall within the sphere of influence of any given particle. A BVH is construted for after each `blob` is constructed.

#### StrainLink.hpp/.cpp
Defines a class that represents a strain link between two particles.

#### blob.hpp/.cpp
Defines a base class `blob`, which has several base methods. There are 4 used subclasses: `rectangle`, `vnotch`, `bracket`, and `dogbone`.

#### constraints.hpp + utils.hpp/.cpp
These files declare a `utils` namespace and `FixtureUserData`.

#### fixture.hpp/.cpp
Defines a class that represents a moving fixture, which is essentially a Box2D rectangle.

#### params.hpp/.cpp
Defines a namespace `params` used to store project global variables. Some of these variables are set in the command line arguments mentioned above.

#### particle.hpp/.cpp
Defines a class that represents a linkable particle, which is represented as a Box2D Circle.


## Course Evaluation
I, Jose Chavez, affirm that I have completed the course instructor course evaluation before it closed. Below is a screenshot of my completed surveys.
![](https://i.imgur.com/vwfEvEw.png)


I, Jeff Cochran, affirm that I have completed the course instructor course evaluation before it closed. Below is a screenshot of my completed surveys.
![](https://i.imgur.com/SxDIx8J.png)

