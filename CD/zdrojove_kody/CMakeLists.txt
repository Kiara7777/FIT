cmake_minimum_required(VERSION 3.8.1)

#jak se projekt jmenuje
project(xskuto00_IBP)

INCLUDE_DIRECTORIES("C:/MinGW/msys/1.0/local/include")

# kdzy je potreba tak zavolej moc automaticky
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#vsechny zdrojove soubory
file(GLOB SOURCES
    src/*.h
    src/*.cpp
)

#chci to vsechno ulozit do...
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Widgets finds its own dependencies.
find_package(Qt5Widgets REQUIRED)

add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

qt5_use_modules(${PROJECT_NAME} Widgets)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} C:/MinGW/msys/1.0/local/lib/libexiv2.dll.a)