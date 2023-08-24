# AGP Demo
This is a basic OpenGL application that demonstrates shader techniques that can be used for rendering water surfaces and reflective surfaces. It was completed as part of a module at University.

## Build instructions
Install [vcpkg](https://vcpkg.io/en/) and run the following command to install library dependencies:

 ```
vcpkg install freeglut sfml glew
 ```

 Then cd into the project root folder and run the following cmake commands:
 ```
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=<path_to_vcpkg_dir>\scripts\buildsystems\vcpkg.cmake
cmake --build build --config Release
 ```

Replacing the <path_to_vcpkg_dir> with the path to your vcpkg installation folder. This will place the executable and resource files into the /build/Release/ folder. You can build the Debug build by specifying --config Debug instead.

## Screenshots

## Water shader

The water shader uses a normal map to simulated the ripple effect you see when looking at water. It animates this normal map over the surface of a large quad by shifting it in both the x and y directions (separately) and then blending the result together (i.e. normalize(normal1 + normal2)). The sampled normal is then used to reflect the view vector and the result of which is used to index into a cube map. A Fresnel term is approximated by using (V.N)^2 where V = view vector and N = surface normal. The refraction vector is not calculated because generally you cannot see the bottom of the ocean because of dirt etc. blocking the light rays. Instead it uses a constant colour to describe the result of a refracted ray i.e. (0.2, 0.1, 0.0).