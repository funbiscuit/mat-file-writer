MatFileWriter
-------------

Simple single-header library to create MAT-files (version 5).

Supported data formats
----------------------

* integer: signed/unsigned (8bit, 16bit, 32bit and 64bit)
* floating point: single/double
* char arrays

Basic usage
-----------

Sample CMakeLists:
```cmake
add_subdirectory(matfile-writer)
target_link_libraries(target PRIVATE MatFileWriter::MatFileWriter)
```
In your code:
```c++
#define MAT_FILE_WRITER_IMPLEMENTATION
#include "mat-file-writer.h"

// ...

MatFileWriter writer("out.mat");
writer.matrix("chars", "Literal can also be written!", 4, 7);
```
For more examples look in `example/example.cpp`
