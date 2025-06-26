set(CMAKE_C_CLANG_TIDY)

if(DOWNLOAD_DEPENDENCIES)
    include(FetchContent)
    FetchContent_Declare(
        miniaudio
        URL https://github.com/mackron/miniaudio/archive/refs/tags/0.11.22.tar.gz
        URL_MD5 4944268151ad037f148b089237566d05
    )
    FetchContent_MakeAvailable(miniaudio)
else()
    add_library(miniaudio STATIC
        miniaudio/extras/miniaudio_split/miniaudio.c
    )
    target_include_directories(miniaudio PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/miniaudio/extras/miniaudio_split")
endif()

# Disable most features since we don't need them.
target_compile_definitions(miniaudio PUBLIC
    MA_ENABLE_ONLY_SPECIFIC_BACKENDS
    MA_NO_DECODING
    MA_NO_ENCODING
    MA_NO_WAV
    MA_NO_FLAC
    MA_NO_MP3
    MA_NO_DEVICE_IO
    MA_NO_RESOURCE_MANAGER
    MA_NO_GENERATION
    MA_NO_THREADING
    MA_NO_RUNTIME_LINKING
)

if(DOWNLOAD_DEPENDENCIES)
    include(FetchContent)
    FetchContent_Declare(
        libsmacker
        URL https://github.com/foxtacles/libsmacker/archive/b3d4e97e0c95d5259d858495a5addd2d93bce5f4.tar.gz
        URL_MD5 7f822319c489ec1a8e41c9f1e2629195
    )
    FetchContent_MakeAvailable(libsmacker)
else()
    set(libsmacker_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libsmacker")
endif()

add_library(libsmacker STATIC
    ${libsmacker_SOURCE_DIR}/smacker.c
)
target_include_directories(libsmacker PUBLIC ${libsmacker_SOURCE_DIR})

if(DOWNLOAD_DEPENDENCIES)
    include(FetchContent)
    FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui"
        GIT_TAG f5befd2d29e66809cd1110a152e375a7f1981f06
    )
    FetchContent_MakeAvailable(imgui)
else()
    set(imgui_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
endif()

add_library(imgui STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui PUBLIC SDL3::Headers)
target_link_libraries(imgui PRIVATE SDL3::SDL3)
set_property(TARGET imgui PROPERTY CXX_CLANG_TIDY "")

# Lua
set(lua_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lua/src")
add_library(lua STATIC
    ${lua_SOURCE_DIR}/lapi.c
    ${lua_SOURCE_DIR}/lcode.c
    ${lua_SOURCE_DIR}/lctype.c
    ${lua_SOURCE_DIR}/ldebug.c
    ${lua_SOURCE_DIR}/ldo.c
    ${lua_SOURCE_DIR}/ldump.c
    ${lua_SOURCE_DIR}/lfunc.c
    ${lua_SOURCE_DIR}/lgc.c
    ${lua_SOURCE_DIR}/llex.c
    ${lua_SOURCE_DIR}/lmem.c
    ${lua_SOURCE_DIR}/lobject.c
    ${lua_SOURCE_DIR}/lopcodes.c
    ${lua_SOURCE_DIR}/lparser.c
    ${lua_SOURCE_DIR}/lstate.c
    ${lua_SOURCE_DIR}/lstring.c
    ${lua_SOURCE_DIR}/ltable.c
    ${lua_SOURCE_DIR}/ltm.c
    ${lua_SOURCE_DIR}/lundump.c
    ${lua_SOURCE_DIR}/lvm.c
    ${lua_SOURCE_DIR}/lzio.c

# Lua libraries
    ${lua_SOURCE_DIR}/lauxlib.c
    ${lua_SOURCE_DIR}/lbaselib.c
    ${lua_SOURCE_DIR}/lcorolib.c
    ${lua_SOURCE_DIR}/ldblib.c
    ${lua_SOURCE_DIR}/liolib.c
    ${lua_SOURCE_DIR}/lmathlib.c
    ${lua_SOURCE_DIR}/loadlib.c
    ${lua_SOURCE_DIR}/loslib.c
    ${lua_SOURCE_DIR}/lstrlib.c
    ${lua_SOURCE_DIR}/ltablib.c
    ${lua_SOURCE_DIR}/lutf8lib.c
    ${lua_SOURCE_DIR}/linit.c
)

if (LINUX)
    target_compile_definitions(lua PRIVATE LUA_USE_LINUX)
elseif (UNIX)
    target_compile_definitions(lua PRIVATE LUA_USE_POSIX)
endif()
