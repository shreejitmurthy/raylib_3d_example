# 3D Scene Demo
This is a demo (or template/starting point, rather) for a 3D game. It comes with raylib and ReactPhysics3D installed locally, so all that's needed is to build and run. It also comes with a basic cube model made in blender and a texture from LearnOpenGL (https://learnopengl.com/img/textures/container2.png)

Tested on macOS.

<img width="693" alt="image" src="https://github.com/shreejitmurthy/raylib_3d_example/assets/87323435/4da66602-054c-44a2-9fa1-db4a1e1e1048">

## Build and Run
1. Ensure you have a C++ compiler and CMake, working in an IDE (such as CLion) helps with not only this but everything else. 
2. Clone or download this repo.
3. Create a build directory. 
4. `cd` into the buld directory and run `cmake ..`
5. After that, run `cmake --build . --target raylib_3d_example`
6. Then run, should all be good.

Here's what those commands look like:
```
git clone https://github.com/shreejitmurthy/raylib_3d_example.git raylib_3d
mkdir build
cd build
cmake ..
cmake --build . --target raylib_3d_example
./raylib_3d_example
```

raylib: https://github.com/raysan5/raylib
ReactPhysics3D: https://github.com/DanielChappuis/reactphysics3d

Let me know of any licensing issues that I've made here, I'm not a professinal and don't have "real-world" experience with this stuff.

## TODO:
- [ ] Informative commenting
- [ ] Test on Windows
- [ ] Player controller (maybe)
