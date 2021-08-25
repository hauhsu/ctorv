# Build Project

This project uses CMake. 

```
$ mkdir build
$ cmake -S . -B build
```

# YCM Configuration

After building with CMake, copy *compile_commands.json* to root directory.
YCM will read the file and set compile options base on the file.
