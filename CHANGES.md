# Coli Game Engine 

![License](https://img.shields.io/badge/license-MIT-blue)
![C++20](https://img.shields.io/badge/C++-20-blue)
![CMake](https://img.shields.io/badge/CMake-3.14+-blue)
![Doxygen](https://img.shields.io/badge/docs-doxygen-blue)

## [Main Page](README.md)

## Changelogs

### v0.2.8 - **Current**

- Systems:
  - Now systems must request components using templates
- Scenes:
  - Removed ordering method from scene
  - Changed filtering method in scene
  - Added verifying for scenes
- Tests:
  - Removed grouping tests
  - Tests for new physics
- Dependencies:
  - New dependency: `bullet3`
- Physics:
  - Added new physical opportunities: `World`, `Body`, and 
    geometry `Shape`

### v0.2.7

- Fixes:
  - Fixed the `MeshTest.CreateAndUnique` result checking
  - Replaced the `_NODISCARD` macro to the `[[nodiscard]]` attribute
    on some functions
  - Fixed `Hash` random results
  - Removed the second template instance of `Hash<float_type>`
- Docs:
  - Totally reworked and improved all docs in all headers

### v0.2.6

- Fixes:
  - Fixed the normals detecting in vertices
  - Removed the redundant explicit template instances of `Vertex`
  - Moved the math hash functions from `std::hash` to `Coli::Utility::Hash`
- Tests:
  - Fixed comparing in the `MeshTest.CreateAlreadyUnique` test

### v0.2.5 

- Geometry:
  - Added hash functions for `glm::vec`, `glm::qua`, and `glm::mat`
  - Also, here is a hash func for `Coli::Geometry::Vertex`
  - Added `Mesh` class
- Tests:
  - Added tests for `Mesh`

### v0.2.4

- Graphics:
  - New `Vertex Array` resource
- Tests:
  - New `Vertex Array` tests
- Geometry:
  - New namespace `Coli::Geometry` for geometry features
  - New `Vertex` struct and its traits `VertexTraits`

### v0.2.3

- Graphics:
  - New `Program` resource
- Tests:
  - Added tests for the new `Program` resource
- Docs:
  - Now the `BasicShader` class has a description
- Errors:
  - The `BasicShader` class throws an exception if the compilation failed

### v0.2.2

- Graphics:
  - New resource: `shader`
- Tests:
  - Added tests for the `shader` resource
- Fixes:
  - Removed `glGetError()` output in `BufferFactory::create()`

### v0.2.1

- Graphics:
  - New `buffer` and `storage` resources
  - RAII resource binding wrapper
- Tests:
  - Added tests for `buffer`
  - Moved `components` tests into `object`
  - Upgraded, grouped, and refactored all tests

### v0.2.0

- Graphics:
  - New base class for graphic resources 
  - Added base for graphics system (context and window) powered by OpenGL
  - New dependency [`glfw3`](https://github.com/glfw/glfw)
  - New target `coli-glad`. The source code was generated [here](https://glad.dav1d.de/)
- Tests:
  - Added context test
  - Also added a few window tests

### v0.1.3

- QoL:
  - The library version is provided to code via dynamically generated `version.h`
  - Improved the C++ standard control
  - Now `GTest` is not required to be installed as a package
- Tests:
  - Added the new test case `version`

### v0.1.2

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