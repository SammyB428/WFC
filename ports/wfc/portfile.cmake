include(vcpkg_common_functions)

if(NOT VCPKG_TARGET_IS_WINDOWS)
    message(FATAL_ERROR "\n${PORT} does not support your system, only Windows for now. Please open a ticket issue on github.com/microsoft/vcpkg if necessary\n")
endif()

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO SammyB428/WFC
    REF dd41bd5e3f2ed8daa9d87adde938b156317bc3b9
    SHA512 d02c7cde8cfed24db5ca52e46e15f866d1dcec3f42013d059ad60fc98bedeec3409b8dad768ad53918a4f152ee07442dcbe9234aa2e625daba4038f1ef24efc9
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
