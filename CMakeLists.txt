cmake_minimum_required(VERSION 3.2)

project(uplink)

include_directories(
    headers
    sources
    dependencies/headers
)

file(GLOB_RECURSE uplink_SOURCES headers/*)

list(APPEND uplink_SOURCES
    sources/uplink.cpp
)

set(uplink_LIBS)

if(WIN32)
    list(APPEND uplink_LIBS
        Gdiplus
        Shlwapi
        glfw3
        OpenGL32
    )

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        link_directories(
            dependencies/binaries/win64
        )
    endif()
elseif(APPLE)
    list(APPEND uplink_LIBS
        "-framework CoreFoundation"
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework CoreVideo"
        "-framework IOKit"
        glfw3
    )
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        link_directories(
            dependencies/binaries/osx
        )
    endif()
endif()

macro(uplink_app name) # source ...
    add_executable(${name} ${uplink_SOURCES} "${ARGN}")
    target_link_libraries(${name} ${uplink_LIBS})
    target_compile_features(${name} PRIVATE
        cxx_lambdas
    )
endmacro()

uplink_app(example-server examples/example-desktop-server.cpp)

file(COPY

    examples/auto-level-button.png
    examples/example-desktop-server.cpp

    DESTINATION . # relative to build directory
)
