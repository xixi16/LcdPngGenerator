cmake_minimum_required(VERSION 3.5.0)
project(LcdPngGenerator VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory(src)
add_subdirectory(test)

set(PNG_INCLUDE_DIR "E:/msys64/mingw64/include") 
set(PNG_LIBRARIES "E:/msys64/mingw64/lib/libpng.dll.a")
set(ZLIB_LIBRARIES "E:/msys64/mingw64/lib/libz.dll.a")

include_directories(${PNG_INCLUDE_DIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
