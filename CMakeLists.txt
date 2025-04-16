cmake_minimum_required(VERSION 3.22)

option(MV_UNITY_BUILD "Combine target source files into batches for faster compilation" OFF)

# -----------------------------------------------------------------------------
# CrossSpeciesComparisonTreeData Plugin
# -----------------------------------------------------------------------------
project(CrossSpeciesComparisonTreeData)

# -----------------------------------------------------------------------------
# CMake Options
# -----------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DWIN32 /EHsc /MP /permissive- /Zc:__cplusplus")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:LIBCMT")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
endif(MSVC)

# -----------------------------------------------------------------------------
# Dependencies
# -----------------------------------------------------------------------------
find_package(Qt6 COMPONENTS Widgets WebEngineWidgets REQUIRED)
find_package(ManiVault COMPONENTS Core CONFIG QUIET)

# -----------------------------------------------------------------------------
# Source files
# -----------------------------------------------------------------------------
set(CROSSSPECIESCOMPARISONTREEDATA_SOURCES
    src/CrossSpeciesComparisonTreeData.h
    src/CrossSpeciesComparisonTreeData.cpp
    src/CrossSpeciesComparisonTreeData.json
    src/InfoAction.h
    src/InfoAction.cpp
)

set(Tree_DATA_HEADERS
    src/CrossSpeciesComparisonTreeData.h
)

source_group(Plugin FILES ${CROSSSPECIESCOMPARISONTREEDATA_SOURCES})

# -----------------------------------------------------------------------------
# CMake Target
# -----------------------------------------------------------------------------
# Create dynamic library for the plugin FIRST
add_library(${PROJECT_NAME} SHARED 
    ${CROSSSPECIESCOMPARISONTREEDATA_SOURCES} 
)

# -----------------------------------------------------------------------------
# Target include directories
# -----------------------------------------------------------------------------
target_include_directories(${PROJECT_NAME} PRIVATE "${MV_INSTALL_DIR}")

# -----------------------------------------------------------------------------
# Target properties
# -----------------------------------------------------------------------------
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    FOLDER DataPlugins
)

# Generate export header
include(GenerateExportHeader)
generate_export_header(${PROJECT_NAME})

# Retrieve the file name of the generated export header
file(GLOB EXPORT_HEADER_FILE_NAME
    RELATIVE ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/*_export.h)
list(APPEND Tree_DATA_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${EXPORT_HEADER_FILE_NAME})

# Set PUBLIC_HEADER property and install headers
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${Tree_DATA_HEADERS}")
install(FILES ${Tree_DATA_HEADERS} DESTINATION include)  # Add this to fix the warning

# Enable unity build
if(MV_UNITY_BUILD)
    set_target_properties(${PROJECT_NAME} PROPERTIES UNITY_BUILD ON)
endif()

# -----------------------------------------------------------------------------
# Target library linking
# -----------------------------------------------------------------------------
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Widgets 
    Qt6::WebEngineWidgets
    ManiVault::Core
)

# -----------------------------------------------------------------------------
# Target installation
# -----------------------------------------------------------------------------
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION Plugins COMPONENT PLUGINS
    LIBRARY DESTINATION Plugins COMPONENT PLUGINS
    ARCHIVE DESTINATION Plugins COMPONENT PLUGINS
    PUBLIC_HEADER DESTINATION include  # Add this to fix the warning
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}"
        --install ${CMAKE_CURRENT_BINARY_DIR}
        --config $<CONFIGURATION>
        --prefix ${ManiVault_INSTALL_DIR}/$<CONFIGURATION>
)

# -----------------------------------------------------------------------------
# Miscellaneous
# -----------------------------------------------------------------------------
if(MSVC)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug,${ManiVault_INSTALL_DIR}/release>)
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_COMMAND $<IF:$<CONFIG:DEBUG>,${ManiVault_INSTALL_DIR}/debug/ManiVault\ Studio.exe,${ManiVault_INSTALL_DIR}/release/ManiVault\ Studio.exe>)
endif()
