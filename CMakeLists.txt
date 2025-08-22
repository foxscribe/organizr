cmake_minimum_required(VERSION 3.16)  
project(organizr LANGUAGES CXX)

# ------------------------------
# Compiler and Standard Settings
# ------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------------
# Dependencies
# ------------------------------
# D++ (Discord C++ Library)
find_package(dpp REQUIRED)

# libpqxx (C++ bindings for PostgreSQL)
find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)

# ------------------------------
# Executable
# ------------------------------
add_executable(organizr
    src/bot.cpp
    src/strings.h
    src/list.h
    src/db.h
    src/db.cpp
)

target_include_directories(organizr
    PRIVATE ${PQXX_INCLUDE_DIRS}
)

target_link_libraries(organizr
    PRIVATE 
    dpp::dpp
    ${PQXX_LIBRARIES}
)

# ------------------------------
# Platform Specifics
# ------------------------------
# pthread is required on Linux/BSD, but on macOS it's part of libc
if(UNIX AND NOT APPLE)
    target_link_libraries(organizr PRIVATE pthread)
endif()

# On *BSD systems, sometimes linking to execinfo is required for backtraces
if(CMAKE_SYSTEM_NAME MATCHES ".*BSD")
    find_library(EXECINFO_LIB execinfo)
    if(EXECINFO_LIB)
        target_link_libraries(organizr PRIVATE ${EXECINFO_LIB})
    endif()
endif()

# ------------------------------
# Install Rules
# ------------------------------
install(TARGETS organizr RUNTIME DESTINATION bin)
