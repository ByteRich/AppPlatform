cmake_minimum_required(VERSION 3.25)
include(FetchContent)

project(UBytesAppPlatform LANGUAGES CXX VERSION 1.1.0)

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

set(HAS_BINARIES FALSE)
if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/Release")
	if (WIN32)
		set(BINARIES_URL_OS_PREFIX "x64-windows")
	else()
		message(FATAL_ERROR "AppPlatform doesn't support platforms other than Windows yet.")
	endif()

	message(STATUS "AppPlatform binaries not found, downloading...")
	FetchContent_Declare(
		AppPlatformBinaries
		URL        https://github.com/UnstableBytes/AppPlatform/releases/download/v${CMAKE_PROJECT_VERSION}/${BINARIES_URL_OS_PREFIX}-binaries.zip
		SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin
	)
	FetchContent_MakeAvailable(AppPlatformBinaries)
endif()
