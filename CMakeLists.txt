cmake_minimum_required(VERSION 3.10)

project(samp-onebot)
set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
    add_definitions(-DLINUX -DSYSTEM_NAME="GNU/Linux")
    set(ICONV_ROOT "${CMAKE_SOURCE_DIR}/lib/iconv")
    set(ICONV_INCLUDE_DIR "${ICONV_ROOT}/include")
    set(ICONV_LIB_DIR "${ICONV_ROOT}/bin")

    # flags
    set(FLAGS "-m32 -fPIC -O2 -fno-stack-protector -Wall -Wno-attributes -Wno-strict-aliasing -Wno-sequence-point")
    set(CMAKE_C_FLAGS ${FLAGS})
    set(CMAKE_CXX_FLAGS ${FLAGS})
    set(CMAKE_SHARED_LINKER_FLAGS "-fshort-wchar -shared -static-libgcc -static-libstdc++")
    
    link_directories(/usr/lib/i386-linux-gnu)
    set(CMAKE_LIBRARY_PATH /usr/lib/i386-linux-gnu)
endif()

if(WIN32)
    add_definitions(-DWINDOWS_IGNORE_PACKING_MISMATCH -DWIN32 -D_WIN32 -DSYSTEM_NAME="Windows")

    # flags
    set(USE_MSVC_RUNTIME_LIBRARY_DLL true)
    set(CMAKE_C_FLAGS "/Zp1 /W4")
    set(CMAKE_CXX_FLAGS "/Zp1 /W4 /EHsc")

    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MTd")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MT")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MTd")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /MT")

    set(CMAKE_SHARED_LINKER_FLAGS "/NOLOGO /MACHINE:X86")
endif()

set(PLUGIN_SDK_DIR "${PROJECT_SOURCE_DIR}/lib/plugin_sdk")
file(GLOB_RECURSE SRC_FILES
    "${PROJECT_SOURCE_DIR}/lib/plugin_sdk/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/src/*.hpp"
)


add_subdirectory(lib)
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC_FILES} ${PROJECT_SOURCE_DIR}/src/exports.def)
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${PLUGIN_SDK_DIR})

if(UNIX AND NOT APPLE)
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${ICONV_INCLUDE_DIR})
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC nlohmann_json::nlohmann_json TwoBot)

# Set linker flags for Windows .def file
if(WIN32)
    set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "/DEF:${PROJECT_SOURCE_DIR}/src/exports.def")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY D:/omp-server/plugins)

    # the post-build command to copy the .dll file only
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
        "D:/omp-server/plugins/$<TARGET_FILE_NAME:${CMAKE_PROJECT_NAME}>"
    )
endif()
