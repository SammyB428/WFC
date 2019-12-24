include(vcpkg_common_functions)

if(NOT VCPKG_TARGET_IS_WINDOWS)
    message(FATAL_ERROR "\n${PORT} does not support your system, only Windows for now. Please open a ticket issue on github.com/microsoft/vcpkg if necessary\n")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SammyB428/WFC
    REF 4e2c59d0ff88c855f4725bc95177004ea9c1ed65
    SHA512 6bd2a41429d40eb753e86326094583d4356ecc5f54a526048f1dbe0070444e5e8a7c1727591314a0a51d43c6955a21946d097918738f21374391ae3d792f9eb1
    HEAD_REF master
)

vcpkg_install_msbuild(
    SOURCE_PATH ${SOURCE_PATH}
    PROJECT_SUBPATH lib/WFC.sln
    INCLUDES_SUBPATH INCLUDE
    LICENSE_SUBPATH LICENSE
    REMOVE_ROOT_INCLUDES
    USE_VCPKG_INTEGRATION
    PLATFORM x64
    RELEASE_CONFIGURATION "STL Unicode Release"
    DEBUG_CONFIGURATION "STL Unicode Debug"
)

file(COPY ${SOURCE_PATH}/include DESTINATION ${CURRENT_PACKAGES_DIR} FILES_MATCHING PATTERN *.h)
file(COPY ${SOURCE_PATH}/include DESTINATION ${CURRENT_PACKAGES_DIR} FILES_MATCHING PATTERN *.hpp)
