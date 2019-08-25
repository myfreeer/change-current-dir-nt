cmake_minimum_required(VERSION 3.10)
project(jetbrains_push_path C)

set(CMAKE_C_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: Debug Release." FORCE)
endif (NOT CMAKE_BUILD_TYPE)


if (MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MT /wd4996 /wd4068")
    set(CompilerFlags
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
    foreach (CompilerFlag ${CompilerFlags})
        string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
    endforeach ()
    link_libraries(ntdll)
endif ()

if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS "-pipe -Wall -Wextra -nostartfiles -Wl,--gc-sections,--build-id=none -static")
    set(CMAKE_C_FLAGS_DEBUG "-g -D_DEBUG")
    set(CMAKE_C_FLAGS_RELEASE "-s -O3 -Os -fmerge-all-constants -fno-asynchronous-unwind-tables")
    if (CMAKE_SIZEOF_VOID_P EQUAL 4)
        set(CMAKE_C_FLAGS_RELEASE "-Os -s -fmerge-all-constants -fno-asynchronous-unwind-tables")
    endif ()
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_C_FLAGS}")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/test_mingw_ntdll_crt.c"
            "size_t __cdecl strlen(const char *_Str);int main(void) {return strlen(\"str\");}")
    try_compile(NTDLL_RESULT ${CMAKE_CURRENT_BINARY_DIR}
            "${CMAKE_CURRENT_BINARY_DIR}/test_mingw_ntdll_crt.c"
            LINK_LIBRARIES ntdllcrt)
    file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/test_mingw_ntdll_crt.c")
    if(NTDLL_RESULT)
        link_libraries(ntdll ntdllcrt)
    else()
        link_libraries(ntdll)
    endif()
endif ()

set (CMAKE_SHARED_LIBRARY_PREFIX "")
set (CMAKE_SHARED_MODULE_PREFIX "")
set (SRC dll-hijack/hijack.h dll-hijack/native_api.h)

add_library(version SHARED version.c dll-hijack/version.h dll-hijack/version.def ${SRC})
