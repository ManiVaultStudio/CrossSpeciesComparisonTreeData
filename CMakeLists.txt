cmake_minimum_required(VERSION 3.22)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

# -------------------------------------------------------------------------------
# Project Setup
# -------------------------------------------------------------------------------
set(PLUGIN_NAME "CrossSpeciesComparisonTreeData")
project(${PLUGIN_NAME})

# -------------------------------------------------------------------------------
# CMake Setup
# -------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /MD")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif()

# -------------------------------------------------------------------------------
# Dependencies
# -------------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core CONFIG QUIET)

# -------------------------------------------------------------------------------
# Sources & Headers
# -------------------------------------------------------------------------------
set(PLUGIN_SOURCES
    src/CrossSpeciesComparisonTreeData.cpp
    src/InfoAction.cpp
)

set(PLUGIN_HEADERS
    src/CrossSpeciesComparisonTreeData.h
    src/InfoAction.h
    src/CrossSpeciesComparisonTreeData.json
)

source_group("Source Files" FILES ${PLUGIN_SOURCES})
source_group("Header Files" FILES ${PLUGIN_HEADERS})

# -------------------------------------------------------------------------------
# Target Definition
# -------------------------------------------------------------------------------
add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})
add_library(ManiVault::${PLUGIN_NAME} ALIAS ${PLUGIN_NAME})

# -------------------------------------------------------------------------------
# Export header generation
# -------------------------------------------------------------------------------
include(GenerateExportHeader)
generate_export_header(${PLUGIN_NAME}
    BASE_NAME ${PLUGIN_NAME}
    EXPORT_MACRO_NAME "${PLUGIN_NAME}_EXPORT"
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}_export.h"
)

# Add the generated export header to the headers list
list(APPEND PLUGIN_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_NAME}_export.h")

# -------------------------------------------------------------------------------
# Includes & Features
# -------------------------------------------------------------------------------
target_include_directories(${PLUGIN_NAME}
    PRIVATE
        "${CMAKE_CURRENT_BINARY_DIR}"       # <-- include generated *_export.h
        "${ManiVault_INCLUDE_DIR}"
)

target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_20)

# -------------------------------------------------------------------------------
# Link Libraries
# -------------------------------------------------------------------------------
target_link_libraries(${PLUGIN_NAME}
    PRIVATE Qt6::Widgets Qt6::WebEngineWidgets
    PRIVATE ManiVault::Core
)

# -------------------------------------------------------------------------------
# Unity Build (optional)
# -------------------------------------------------------------------------------
if(MV_UNITY_BUILD)
    set_target_properties(${PLUGIN_NAME} PROPERTIES UNITY_BUILD ON)
endif()

# -------------------------------------------------------------------------------
# Install
# -------------------------------------------------------------------------------
set_target_properties(${PLUGIN_NAME} PROPERTIES
    PUBLIC_HEADER "${PLUGIN_HEADERS}"
    AUTOMOC ON
)

install(TARGETS ${PLUGIN_NAME}
    EXPORT ManiVaultPublicExport
    RUNTIME DESTINATION "$<CONFIGURATION>/Plugins/" COMPONENT PLUGINS
    LIBRARY DESTINATION "$<CONFIGURATION>/Plugins/" COMPONENT PLUGINS
    ARCHIVE DESTINATION "$<CONFIGURATION>/lib/"     COMPONENT LINKLIB
    PUBLIC_HEADER DESTINATION "$<CONFIGURATION>/include/${PLUGIN_NAME}/" COMPONENT HEADERS
)

add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${ManiVault_INSTALL_DIR}
)

# -------------------------------------------------------------------------------
# MSVC Debug Configuration
# -------------------------------------------------------------------------------
if(MSVC)
    set_property(TARGET ${PLUGIN_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
        $<IF:$<CONFIG:DEBUG>,
            ${ManiVault_INSTALL_DIR}/Debug,
            $<IF:$<CONFIG:RELWITHDEBINFO>,
                ${ManiVault_INSTALL_DIR}/RelWithDebInfo,
                ${ManiVault_INSTALL_DIR}/Release
            >
        >
    )

    set_property(TARGET ${PLUGIN_NAME} PROPERTY VS_DEBUGGER_COMMAND
        $<IF:$<CONFIG:DEBUG>,
            "${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe",
            $<IF:$<CONFIG:RELWITHDEBINFO>,
                "${ManiVault_INSTALL_DIR}/RelWithDebInfo/ManiVault Studio.exe",
                "${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe"
            >
        >
    )
endif()
