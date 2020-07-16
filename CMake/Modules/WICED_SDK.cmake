#
# Copyright (c) 2020 The nanoFramework project contributors
# See LICENSE file in the project root for full license information.
#

# include the WICED SDK project

macro(IncludeWICED_SDK)

    set(NO_WICED_SDK_SOURCE TRUE)
    if(NO_WICED_SDK_SOURCE)
        if(NOT "${WICED_SDK_SOURCE}" STREQUAL "")
            set(NO_WICED_SDK_SOURCE FALSE)
        endif()
    endif()

    if(NO_WICED_SDK_SOURCE)
        # no WICED SDK source specified, download it from nanoFramework fork

        # check for Git (needed here for advanced warning to user if it's not installed)
        find_package(Git)

        #  check if Git was found, if not report to user and abort
        if(NOT GIT_EXECUTABLE)
            message(FATAL_ERROR "error: could not find Git, make sure you have it installed.")
        endif()

        message(STATUS "WICED SDK from GitHub repo")

        # need to setup a separate CMake project to download the code from the GitHub repository
        # otherwise it won't be available before the actual build step
        configure_file("${PROJECT_SOURCE_DIR}/CMake/WICED_SDK.CMakeLists.cmake.in"
                    "${CMAKE_BINARY_DIR}/WICED_SDK_Download/CMakeLists.txt")

        # setup CMake project for WICED SDK download
        execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
                        RESULT_VARIABLE result
                        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/WICED_SDK_Download")

        # run build on WICED SDK download CMake project to perform the download
        execute_process(COMMAND ${CMAKE_COMMAND} --build .
                        RESULT_VARIABLE result
                        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/WICED_SDK_Download")

        # add WICED SDK as external project
        # need to specify nanoframework as the active branch
        ExternalProject_Add( 
            WICED_SDK
            PREFIX WICED_SDK
            SOURCE_DIR ${CMAKE_BINARY_DIR}/WICED_SDK_Source
            GIT_REPOSITORY  https://github.com/nanoframework/WICED_Wi-Fi_43xxx
            GIT_TAG "nf-build"  # target specific branch        
            GIT_SHALLOW 1   # download only the tip of the branch, not the complete history
            TIMEOUT 10
            LOG_DOWNLOAD 1
            # Disable all other steps
            INSTALL_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
        )      

        # get source dir for WICED SDK CMake project
        ExternalProject_Get_Property(WICED_SDK SOURCE_DIR)

    else()
        # WICED SDK source was specified

        # sanity check is source path exists
        if(EXISTS "${WICED_SDK_SOURCE}/")
            message(STATUS "WICED SDK source from: ${WICED_SDK_SOURCE}")

            # check if we already have the sources, no need to copy again
            if(NOT EXISTS "${CMAKE_BINARY_DIR}/WICED_SDK_Source")
                file(COPY "${WICED_SDK_SOURCE}/" DESTINATION "${CMAKE_BINARY_DIR}/WICED_SDK_Source")
            else()
                message(STATUS "Using local cache of WICED SDK source from ${WICED_SDK_SOURCE}")
            endif()
        else()
            message(FATAL_ERROR "Couldn't find WICED SDK source at ${WICED_SDK_SOURCE}/")
        endif()

        # add WICED SDK as external project
        ExternalProject_Add(
            WICED_SDK
            PREFIX WICED_SDK
            SOURCE_DIR ${CMAKE_BINARY_DIR}/WICED_SDK_Source
            # Disable all other steps
            INSTALL_COMMAND ""
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
        )

        # get source dir for WICED SDK CMake project
        ExternalProject_Get_Property(WICED_SDK SOURCE_DIR)

    endif()

endmacro()
