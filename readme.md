# Search Tree

## Program Purpose

This program builds self-balancing binary search tree to contain numbers and count the specified subsets of them.

## How to build

Firstly clone this repo and go root directory.
```
git clone git@github.com:R0flan4k/SearchTree.git
cd SearchTree
```

Next download conan into virtual environments:
```
python3 -m venv .venv
source .venv/bin/activate
pip3 install -r requirements.txt
```

Detect conan profile:
```
conan profile detect --force
```

Now you can build the project:
```
conan install . --build=missing -s compiler.cppstd=gnu20
cmake . -B build/Release -DCMAKE_TOOLCHAIN_FILE=build/Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build/Release
```
