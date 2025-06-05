# Simple SDL2 finder using pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)

set(SDL2_INCLUDE_DIR ${SDL2_INCLUDE_DIRS})
set(SDL2_LIBRARY ${SDL2_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SDL2 DEFAULT_MSG SDL2_LIBRARY SDL2_INCLUDE_DIR)

mark_as_advanced(SDL2_INCLUDE_DIR SDL2_LIBRARY)
