remove_definitions(-DQT_NO_CAST_FROM_ASCII)

include(ECMAddTests)

find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)

if(NOT Qt5Test_FOUND)
    message(STATUS "Qt5Test not found, autotests will not be built.")
    return()
endif()

ecm_add_tests(
    karchivetest.cpp
    kfiltertest.cpp
    deprecatedtest.cpp
    LINK_LIBRARIES KF5::Archive Qt5::Test
)

target_compile_definitions(deprecatedtest PRIVATE KARCHIVE_DEPRECATED=)
target_link_libraries(kfiltertest ${ZLIB_LIBRARIES})

########### klimitediodevicetest ###############

ecm_add_test(
    klimitediodevicetest.cpp
    ../src/klimitediodevice.cpp
    TEST_NAME klimitediodevicetest
    LINK_LIBRARIES Qt5::Test
)
target_include_directories(klimitediodevicetest
    PRIVATE $<TARGET_PROPERTY:KF5Archive,INTERFACE_INCLUDE_DIRECTORIES>)
