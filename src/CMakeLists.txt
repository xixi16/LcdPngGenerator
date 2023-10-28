cmake_minimum_required(VERSION 3.5.0)
project(LcdPngGenerator VERSION 0.1.0 LANGUAGES C CXX)

set(PNG_INCLUDE_DIR "E:/msys64/mingw64/include") 
set(PNG_LIBRARIES "E:/msys64/mingw64/lib/libpng.dll.a")
set(ZLIB_LIBRARIES "E:/msys64/mingw64/lib/libz.dll.a")

include_directories("E:/msys64/mingw64/include")

add_executable(LcdPngGenerator CUtility.cpp LcdPngGenerator.cpp)

target_link_libraries(${PROJECT_NAME} 
                            ${PNG_LIBRARIES}
                            ${ZLIB_LIBRARIES})


