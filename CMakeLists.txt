cmake_minimum_required(VERSION 3.22)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

# -------------------------------------------------------------------------------
# Project Setup
# -------------------------------------------------------------------------------
set(PLUGIN_NAME "CrossSpeciesComparisonTreeData")
project(${PLUGIN_NAME} VERSION 1.0 LANGUAGES CXX)

# -------------------------------------------------------------------------------
# CMake Setup
# -------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    add_compile_options(
        /DWIN32
        /EHsc
        /MP
        /permissive-
        /Zc:__cplusplus
    )
    
    # Use generator expressions for configuration-specific settings
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# -------------------------------------------------------------------------------
# Dependencies
# -------------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core CONFIG REQUIRED)  # Changed from QUIET to REQUIRED

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

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})

# -------------------------------------------------------------------------------
# Target Definition
# -------------------------------------------------------------------------------
add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES} ${PLUGIN_HEADERS})
add_library(ManiVault::${PLUGIN_NAME} ALIAS ${PLUGIN_NAME})

# Export header
include(GenerateExportHeader)
generate_export_header(${PLUGIN_NAME}
    BASE_NAME CrossSpeciesComparisonTreeData
    EXPORT_MACRO_NAME CROSSSPECIESCOMPARISONTREEDATA_EXPORT
    EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/CrossSpeciesComparisonTreeData_export.h"
)

list(APPEND PLUGIN_HEADERS "${CMAKE_CURRENT_BINARY_DIR}/CrossSpeciesComparisonTreeData_export.h")

# -------------------------------------------------------------------------------
# Target Properties
# -------------------------------------------------------------------------------
target_include_directories(${PLUGIN_NAME} PRIVATE 
    ${CMAKE_CURRENT_BINARY_DIR}
    ${ManiVault_INCLUDE_DIR}
)

target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_20)

target_link_libraries(${PLUGIN_NAME} PRIVATE 
    Qt6::Widgets 
    Qt6::WebEngineWidgets
    ManiVault::Core
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
include(GNUInstallDirs)

set_target_properties(${PLUGIN_NAME} PROPERTIES
    PUBLIC_HEADER "${PLUGIN_HEADERS}"
    AUTOMOC ON
)

install(TARGETS ${PLUGIN_NAME}
    EXPORT ManiVaultPublicExport
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}/Plugins"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/Plugins"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PLUGIN_NAME}"
)

# -------------------------------------------------------------------------------
# MSVC Debug Configuration
# -------------------------------------------------------------------------------
if(MSVC)
    # Simplified debugger configuration
    set(MV_STUDIO_EXECUTABLE "$<IF:$<CONFIG:Debug>,${ManiVault_INSTALL_DIR}/Debug/ManiVault Studio.exe,${ManiVault_INSTALL_DIR}/Release/ManiVault Studio.exe>")
    
    set_property(TARGET ${PLUGIN_NAME} PROPERTY 
        VS_DEBUGGER_WORKING_DIRECTORY "${ManiVault_INSTALL_DIR}/$<CONFIG>")
    set_property(TARGET ${PLUGIN_NAME} PROPERTY 
        VS_DEBUGGER_COMMAND "${MV_STUDIO_EXECUTABLE}")
endif()
