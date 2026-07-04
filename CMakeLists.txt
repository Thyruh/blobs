cmake_minimum_required(VERSION 3.20)
project(blob C)

set(CMAKE_C_STANDARD 23)

set(SRC_DIR src)
file(GLOB SRC_FILES CONFIGURE_DEPENDS ${SRC_DIR}/*.c)

add_executable(blob ${SRC_FILES})

target_include_directories(blob PRIVATE include)

# Debug / Release modes
target_compile_options(blob PRIVATE
    $<$<CONFIG:Debug>:-O1 -g -DDEBUG>
    $<$<CONFIG:Release>:-O3>
)

# Raylib
if (WIN32)
    target_link_libraries(blob
        ${CMAKE_SOURCE_DIR}/external/libraylib_win.a
        opengl32 gdi32 winmm
    )
else()
    target_link_libraries(blob
        ${CMAKE_SOURCE_DIR}/external/libraylib.a
        m pthread dl GL X11 Xrandr Xi Xxf86vm Xcursor rt
    )
endif()


# cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
# cmake --build build
