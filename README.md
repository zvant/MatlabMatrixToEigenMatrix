# MatlabMatrixToEigenMatrix
Transport data between .mat file and Eigen::Matrix object.

Sample code are provided. Please test and report bugs.

Matlab and libeigen3 are required.

How to build:

`cd path_to_directory`

`cp CMakeLists.txt.example CMakeLists.txt`

Then edit the CMakeLists.txt to specify your own Matlab path.

`cd build`

`cmake ..`

`make`
