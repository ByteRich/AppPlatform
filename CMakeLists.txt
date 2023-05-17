cmake_minimum_required(VERSION 3.25)
project(UBytesAppPlatform CXX)

set(APP_NAME AppPlatform)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE PUBLIC_SOURCES "include/*.hpp")

add_library(${APP_NAME} INTERFACE ${PUBLIC_SOURCES})
add_library(${APP_NAME}_Internal INTERFACE ${PUBLIC_SOURCES})

target_include_directories(${APP_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${APP_NAME}_Internal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link libraries only for the general target library.
target_link_directories(${APP_NAME} INTERFACE
	$<$<CONFIG:Debug>:${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug>
	$<$<CONFIG:Release>:${CMAKE_CURRENT_SOURCE_DIR}/bin/Release>
)

target_link_libraries(${APP_NAME} INTERFACE UBytesAppPlatform)