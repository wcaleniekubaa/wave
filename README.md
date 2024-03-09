
# Wave Library

Wave is a simple C library for reading and writing WAV files. It provides functions to load WAV files from disk or memory, manipulate them, and save them back to disk.

## Features

- Load WAV files from disk or memory
- Read WAV file information such as number of channels, sample rate, and bits per sample
- Handle errors gracefully
- Save WAV files to disk

## Usage

To use the Wave library in your project, include the `wave.h` header file and link against the appropriate library file (`wave_static` for static linking or `wave_shared` for shared linking).

```c
#include <wave/wave.h>

int main() 
{
    // do some magic :)    
}
```

## Building

Wave library uses CMake for building. Follow these steps to build the library:

1. Clone the repository: `git clone https://github.com/wcaleniekubaa/wave.git`
2. Navigate to the repository directory: `cd wave`
3. Create a build directory: `mkdir build && cd build`
4. Run CMake: `cmake ..`
5. Build the library: `make`

Optionally, you can build the examples by enabling the `WAVE_BUILD_EXAMPLES` option in CMake.

## Examples

The Wave library comes with some example programs to demonstrate its usage. You can find them in the `examples` directory.

- `read_wav`: Read information from a WAV file
- `write_empty_wav`: Create an empty WAV file
- `rewrite_wav`: Read and rewrite a WAV file
- `cpp_wrapper`: Test of C++ wrapper

