cmake_minimum_required(VERSION 3.22.1)

project("libgrabber")

add_library(${CMAKE_PROJECT_NAME} SHARED
    grabber.cpp
    grabber/core/ElfReader.cpp
    grabber/core/SymbolProcessor.cpp
    grabber/parser/FunctionParser.cpp
    grabber/parser/NamespaceParser.cpp
    grabber/utils/StringUtils.cpp
    grabber/utils/TypeUtils.cpp
    grabber/output/HeaderWriter.cpp
    grabber/analyzer/RTTIFieldParser.cpp
    grabber/analyzer/DWARFFieldParser.cpp
    grabber/analyzer/SymbolFieldParser.cpp
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    android
    log
)


include(FetchContent)

FetchContent_Declare(
    elfio
    GIT_REPOSITORY https://github.com/serge1/ELFIO.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(elfio)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${elfio_SOURCE_DIR}
)