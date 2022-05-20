message("Creating package(s)...")

if(NOT PKG_VENDOR)
    message("PKG_VENDOR not specified, exiting")
    return()
endif()

if(NOT PKG_SUMMARY)
    message("PKG_SUMMARY not specified, exiting")
    return()
endif()

if(NOT PKG_DESCRIPTION)
    message("PKG_DESCRIPTION not specified, exiting")
    return()
endif()

if(NOT PKG_CONTACT)
    message("PKG_CONTACT not specified, exiting")
    return()
endif()

if(NOT PKG_DEPENDS)
    set(PKG_DEPENDS "")
endif()

set(CPACK_DEB_COMPONENT_INSTALL ON)

# generate DEB package
#if("${CMAKE_BUILD_TYPE}" STREQUAL "RELEASE")

    string(TIMESTAMP DATETIME_NOW %Y%m%d%H%M UTC)

    if(NOT DEFINED ARCH)

        execute_process(
            COMMAND "/usr/bin/dpkg" "--print-architecture"
            RESULT_VARIABLE RESULT
            OUTPUT_VARIABLE ARCH
        )

        string(STRIP "${ARCH}" ARCH)

    endif()

    if("${ARCH}" STREQUAL "")
        set(ARCH "amd64")
    endif()

    set(VERSION_MAJOR "1")
    set(VERSION_MINOR "0")
    set(VERSION_PATCH "${DATETIME_NOW}")

    set(CPACK_GENERATOR "DEB")
    set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE ${ARCH})
    
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "")
    set(CPACK_PACKAGE_VENDOR ${PKG_VENDOR})
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PKG_SUMMARY})
    set(CPACK_PACKAGE_DESCRIPTION ${PKG_DESCRIPTION})
    set(CPACK_PACKAGE_CONTACT ${PKG_CONTACT})

    set(PROJECT_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}-${VERSION_PATCH}")

    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${PROJECT_VERSION}_${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}")

    include(CPack)

#endif()
