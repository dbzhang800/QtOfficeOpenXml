cmake_minimum_required(VERSION 2.8.12)

project(KArchive)

find_package(ECM 1.8.0 REQUIRED NO_MODULE)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings)
include(KDECMakeSettings)

include(FeatureSummary)
include(GenerateExportHeader)

set(REQUIRED_QT_VERSION 5.2.0)
find_package(Qt5Core ${REQUIRED_QT_VERSION} REQUIRED NO_MODULE)

find_package(ZLIB)
set_package_properties(ZLIB PROPERTIES
    URL "http://www.zlib.net"
    DESCRIPTION "Support for gzip compressed files and data streams"
    TYPE REQUIRED
    PURPOSE "Required by the core KDE libraries and some criti cal kioslaves"
)

find_package(BZip2)
set_package_properties(BZip2 PROPERTIES
    URL "http://www.bzip.org"
    DESCRIPTION "Support for BZip2 compressed files and data streams"
    TYPE RECOMMENDED
    PURPOSE "Support for BZip2 compressed files and data streams"
)

find_package(LibLZMA)
set_package_properties(LibLZMA PROPERTIES
    URL "http://tukaani.org/xz/"
    DESCRIPTION "Support for xz compressed files and data streams"
    PURPOSE "Support for xz compressed files and data streams"
)
include_directories(
    ${ZLIB_INCLUDE_DIR}
)

include(ECMSetupVersion)
include(ECMGenerateHeaders)

set(KF5_VERSION "5.8.0") # handled by release scripts

ecm_setup_version(${KF5_VERSION}
    VARIABLE_PREFIX KARCHIVE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/karchive_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5ArchiveConfigVersion.cmake"
    SOVERSION 5)

add_subdirectory(src)
add_subdirectory(autotests)
add_subdirectory(tests)


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Archive")

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5ArchiveConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5ArchiveConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/karchive_version.h
        DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}
        COMPONENT Devel)

install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/KF5ArchiveConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/KF5ArchiveConfigVersion.cmake"
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        COMPONENT Devel)

install(EXPORT KF5ArchiveTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5ArchiveTargets.cmake
        NAMESPACE KF5::)

feature_summary(WHAT ALL   FATAL_ON_MISSING_REQUIRED_PACKAGES)
