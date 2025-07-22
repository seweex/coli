# Coli Game Engine

![License](https://img.shields.io/badge/license-MIT-blue)
![C++20](https://img.shields.io/badge/C++-20-blue)
![CMake](https://img.shields.io/badge/CMake-3.14+-blue)
![Doxygen](https://img.shields.io/badge/docs-doxygen-blue)

## Introduction

It is a lightweight open-source game engine library that makes easy to
develop any games using the simple tool. The main goal of this project
is to be simple and useful solution for game developers.

Made with simplicity and performance in mind, it provides:

- Uses modern features of C++20 
- Simply entity management
- Fast math operations
- Full open-source 
- Documented API

Perfect for 2D and 3D game development

It uses modern C++20 and based on ECS (Entity-Component-System).

## Contents

- **[Acknowledgments](#acknowledgments)**
- **[Examples](#examples)**
- **[Setup](#setup)**
  - **[Requirements](#requirements)**
  - **[Installation](#installation)**
  - **[CMake Options](#cmake-options)**
- **[Tests](#tests)**
- **[Documentation](#documentation)**
- **[Roadmap](#roadmap)**
- **[Changelog](#changelog)**
- **[License](#license)**

## Acknowledgments

This project uses the following open-source solutions:

- **[EnTT](https://github.com/skypjack/entt)**: Based on provided ECS (License: MIT)
- **[glm](https://github.com/g-truc/glm)**: Uses the glm math (License: Modified MIT)
- **[glfw](https://github.com/glfw/glfw)**: Manages windows and the context (License: Zlib)
- **[glad](https://glad.dav1d.de/)**: Loads OpenGL functions. Automatically generated.

## Examples

Code of a very simple game:
```cpp
#include <coli/game-engine.h>
#include <memory>
#include <vector>

using namespace Coli;

Generic::Engine engine;

// make your component class
class SpecialComponent { /* ... */ };

// make your system class
class SpecialSystem :
    public Generic::SystemBase
{
public:
    /* ... */
    
    void operator()(std::vector<Game::ObjectHandle>& objects) override
    {
        for (auto& object : objects)
        {
            try {
                auto& specialComponent = object.get<SpecialComponent>();
                /* ... */
            }
            // no `SpecialComponent` in this `object`
            catch (std::invalid_argument const& exception) {
                continue;
            }
        }
        
        engine.stop();
    }
};

int main()
{
    // create a scene
    auto scene = std::make_shared<Game::Scene>();

    // make it active
    engine.active_scene(scene);
    
    // create a system of your type
    engine.make_system<SpecialSystem>();

    // then create a few objects
    Game::ObjectHandle object1 = scene->create();
    Game::ObjectHandle object2 = scene->create();
    
    // you can add components
    object1.emplace<SpecialComponent>();

    // launch the game loop
    engine.run();
}
```

## Setup

### Requirements

Before you install the `coli` library make sure you have all the following
libs, and it is able to `find_package()` for all of them:

- `EnTT`
- `glm`
- `glfw`

Also, you must have:
- `C++20` or higher compiler 
- `CMake` 3.14 or higher

### Installation

Clone and build the library:
```shell
git clone https://github.com/seweex/coli.git
cd coli
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX="the/coli/dir"
cmake --build . --target=build 
```

Then import this library to your project. 
Add this to your main `CMakeLists.txt`:
```cmake
find_package(coli)
target_include_directories(<your-target> PRIVATE coli::game-engine)
```

Configure your main project passing the `CMAKE_PREFIX_PATH` as the directory
you have passed on the `coli` library configuration as `CMAKE_INSTALL_PREFIX`
or make sure `CMake` can find the `coli` library:
```shell
cmake .. -DCMAKE_PREFIX_PATH="the/coli/dir"
```

If you build the `coli` library as dynamic on `Windows` you have to copy the
`.dll` library file to your executable file.

### CMake Options

You can pass a few of the following options to `CMake` when you configure 
the `coli` library to customize your build:

- **COLI_FORCE_SINGLE_FLOAT**: Forced `float` type instead of default `double` if 
  set to **ON**
- **COLI_BUILD_DYNAMIC**: Set to **ON** for get `coli` as the dynamic library,
otherwise it will be static.
- **COLI_BUILD_DOCS**: Set to **ON** for generate the `doxygen` [documentation](#documentation).
- **COLI_BUILD_TESTS**: Set to **ON** for enable [tests](#tests).

Pass options like:
```shell
cmake .. -D[param]=ON -D[params...]=ON 
```
where `param` and `params` are the followed options 

## Tests

To compile and run the tests you must set the option
`COLI_BUILD_TESTS` to `ON`. \
To run the test if you have the `coli` library built with that option: 
```shell
cd coli/build
ctest -C Debug
```

## Documentation

The `coli` library is based on the `doxygen` documentation.
You can make it when you build the library, make sure you have 
`doxygen` installed and set the `COLI_BUILD_DOCS` option to `ON`:
```shell
cmake .. -DCMAKE_INSTALL_PREFIX="the/coli/dir" -DCOLI_BUILD_DOCS=ON
```

After you build the library you can find the docs at 
`<install-dir>/docs/html/index.html`

## Roadmap

There is a lot of work left. Some future features:

- More components and their systems:
  - Collisions and colliders
  - Physics
  - Transform
  - Graphics
  - Input

## Changelog

You can view the history of changes [there](CHANGES.md)

## License

This code released under the [MIT license](LICENSE).