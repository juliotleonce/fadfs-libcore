cmake_minimum_required(VERSION 4.2)
project(fadfs_core LANGUAGES C VERSION 0.1.0)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "$ENV{HOME}/.local" CACHE PATH "..." FORCE)
endif()

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}")

find_package(xstdlib REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)

add_library(fadfs_core SHARED
        include/public/api.h
        include/internal/typedef.h
        src/api.c
        src/inode.c
        src/fadisk.c
        src/bitmap.c
        src/path.c
        src/dir.c
        include/internal/fadisk.h
        src/state.c
        include/internal/state.h
        include/internal/constant.h
        include/internal/superblock.h
        src/superblock.c
        include/public/error.h
)

target_include_directories(fadfs_core PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include/fadfs_core>
)

target_link_libraries(fadfs_core PRIVATE xstdlib::xstdlib)

include(GNUInstallDirs)

install(FILES
        include/public/api.h
        include/public/error.h
        DESTINATION include/fadfs_core
)

install(TARGETS fadfs_core
        EXPORT fadfs_core_targets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION include
)

install(EXPORT fadfs_core_targets
        FILE fadfs_coreTargets.cmake
        NAMESPACE fadfs_core::
        DESTINATION lib/cmake/fadfs_core
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/fadfs_coreConfig.cmake"
        "include(\${CMAKE_CURRENT_LIST_DIR}/fadfs_coreTargets.cmake)\n")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/fadfs_coreConfig.cmake"
        DESTINATION lib/cmake/fadfs_core)