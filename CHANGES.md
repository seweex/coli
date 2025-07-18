# Coli Game Engine 

![License](https://img.shields.io/badge/license-MIT-blue)
![C++20](https://img.shields.io/badge/C++-20-blue)
![CMake](https://img.shields.io/badge/CMake-3.14+-blue)
![Doxygen](https://img.shields.io/badge/docs-doxygen-blue)

## [Main Page](README.md)

## Changelogs

### v0.1.2 - **Current**

- Components:
  - Added `Trasform` component. It provides 2D and 3D world transformation based on
  position, scale and rotation
  - Now `Object` can destroy components
- Tests:
  - Added components tests
  - Fixed access modifier to the global variables. Now each test has their own 
  variables
- Types:
  - Added namespace `Types` that helps with 2D and 3D math types like vectors
  and rotators. Also there are a function that makes default rotation for any
  dimensions mode (2D and 3D)
- Options:
  - There is a new build option `COLI_FORCE_SINGLE_FLOAT`. The `coli` library
  uses `double` in default. This new option sets default floating point type
  to `float`

### v0.1.1

- Added `CHANGES.md` file to track the changes

### v0.1.0

- Added:
  - `Objects` class that provides components manipulations 
  - `Scene` for objects manipulations
  - `Engine` processing systems and active scene in the game loop
  - `Layer` component for ordering objects processing
  - `README.md` file to preview the project

- Provided:
  - Optional installation as a dynamic library
  - Optional documentation
  - Optional tests