cmake_minimum_required(VERSION 3.2)
PROJECT(obs-stream-effects)

################################################################################
# Version
################################################################################
SET(VERSION_MAJOR 0)
SET(VERSION_MINOR 3)
SET(VERSION_PATCH 0)
SET(VERSION_STR "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

configure_file(
	"${PROJECT_SOURCE_DIR}/source/version.h.in"
	"${PROJECT_BINARY_DIR}/source/version.h"
)

################################################################################
# Configuration
################################################################################
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

SET(INSTALL_DIR "${PROJECT_BINARY_DIR}/distribute" CACHE PATH "Installation directory")
IF(NOT DEFINED PACKAGE_PREFIX)
	SET(PACKAGE_PREFIX "")
ENDIF()
IF(NOT DEFINED PACKAGE_SUFFIX)
	SET(PACKAGE_SUFFIX ".${VERSION_STR}")
ENDIF()

################################################################################
# Code
################################################################################

# Utility
SET(UTILITY_SOURCE
	"${PROJECT_SOURCE_DIR}/source/gs-helper.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-effect.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-indexbuffer.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-mipmapper.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-rendertarget.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-texture.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-vertex.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-vertexbuffer.cpp"
	"${PROJECT_SOURCE_DIR}/source/gs-mipmapper.cpp"
	"${PROJECT_SOURCE_DIR}/source/util-distancefield-generator.cpp"
	"${PROJECT_SOURCE_DIR}/source/util-math.cpp"
)
SET(UTILITY_HEADER
	"${PROJECT_SOURCE_DIR}/source/gs-helper.h"
	"${PROJECT_SOURCE_DIR}/source/gs-effect.h"
	"${PROJECT_SOURCE_DIR}/source/gs-indexbuffer.h"
	"${PROJECT_SOURCE_DIR}/source/gs-mipmapper.h"
	"${PROJECT_SOURCE_DIR}/source/gs-rendertarget.h"
	"${PROJECT_SOURCE_DIR}/source/gs-texture.h"
	"${PROJECT_SOURCE_DIR}/source/gs-vertex.h"
	"${PROJECT_SOURCE_DIR}/source/gs-vertexbuffer.h"
	"${PROJECT_SOURCE_DIR}/source/gs-mipmapper.h"
	"${PROJECT_SOURCE_DIR}/source/util-distancefield-generator.h"
	"${PROJECT_SOURCE_DIR}/source/util-math.h"
)

# Filters
SET(FILTER_SOURCE
	"${PROJECT_SOURCE_DIR}/source/filter-displacement.cpp"
	"${PROJECT_SOURCE_DIR}/source/filter-blur.cpp"
	"${PROJECT_SOURCE_DIR}/source/filter-shape.cpp"
	"${PROJECT_SOURCE_DIR}/source/filter-stroke.cpp"
	"${PROJECT_SOURCE_DIR}/source/filter-transform.cpp"
)
SET(FILTER_HEADER
	"${PROJECT_SOURCE_DIR}/source/filter-displacement.h"
	"${PROJECT_SOURCE_DIR}/source/filter-blur.h"
	"${PROJECT_SOURCE_DIR}/source/filter-shape.h"
	"${PROJECT_SOURCE_DIR}/source/filter-stroke.h"
	"${PROJECT_SOURCE_DIR}/source/filter-transform.h"
)

# Sources
SET(SOURCE_SOURCE
	"${PROJECT_SOURCE_DIR}/source/source-mirror.cpp"
)
SET(SOURCE_HEADER
	"${PROJECT_SOURCE_DIR}/source/source-mirror.h"
)

# Transitions
SET(TRANSITION_SOURCE
)
SET(TRANSITION_HEADER
)

# All
SET(obs-stream-effects_HEADERS
	${FILTER_HEADER}
	${SOURCE_HEADER}
	${TRANSITION_HEADER}
	${UTILITY_HEADER}
	"${PROJECT_SOURCE_DIR}/source/plugin.h"
	"${PROJECT_BINARY_DIR}/source/version.h"
	"${PROJECT_SOURCE_DIR}/source/strings.h"
)
SET(obs-stream-effects_SOURCES
	${FILTER_SOURCE}
	${SOURCE_SOURCE}
	${TRANSITION_SOURCE}
	${UTILITY_SOURCE}
	"${PROJECT_SOURCE_DIR}/source/plugin.cpp"
)
SET(obs-stream-effects_LOCALE
	"${PROJECT_SOURCE_DIR}/data/locale/en-US.ini"
)
SET(obs-stream-effects_EFFECTS
	"${PROJECT_SOURCE_DIR}/data/effects/bilateral-blur.effect"
	"${PROJECT_SOURCE_DIR}/data/effects/box-blur.effect"
	"${PROJECT_SOURCE_DIR}/data/effects/color-conversion.effect"
	"${PROJECT_SOURCE_DIR}/data/effects/displace.effect"
	"${PROJECT_SOURCE_DIR}/data/effects/gaussian-blur.effect"
	"${PROJECT_SOURCE_DIR}/data/effects/mip-mapper.effect"
	"${PROJECT_SOURCE_DIR}/data/effects/signeddistancefield.effect"
)
SET(obs-stream-effects_LIBRARIES
)

# Groups
source_group("Source Files\\Filters" FILES ${FILTER_SOURCE})
source_group("Header Files\\Filters" FILES ${FILTER_HEADER})
source_group("Source Files\\Sources" FILES ${SOURCE_SOURCE})
source_group("Header Files\\Sources" FILES ${SOURCE_HEADER})
source_group("Source Files\\Transitions" FILES ${TRANSITION_SOURCE})
source_group("Header Files\\Transitions" FILES ${TRANSITION_HEADER})
source_group("Source Files\\Utility" FILES ${UTILITY_SOURCE})
source_group("Header Files\\Utility" FILES ${UTILITY_HEADER})
source_group("Data Files\\Locale" FILES ${obs-stream-effects_LOCALE})
source_group("Data Files\\Effects" FILES ${obs-stream-effects_EFFECTS})

################################################################################
# Standalone and OBS Studio Build Data
################################################################################
if(TARGET libobs)
	# OBS Studio Specific
	INCLUDE_DIRECTORIES(
		"${CMAKE_SOURCE_DIR}"
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}/source"
		"${PROJECT_SOURCE_DIR}"
		"${PROJECT_SOURCE_DIR}/source"
	)
	SET(LIBOBS_LIBRARIES libobs)
else()
	# Standlone Specific
	SET(PATH_OBSStudio "" CACHE PATH "OBS Studio Source Code Directory")
	if(PATH_OBSStudio STREQUAL "")
		message(FATAL_ERROR "PATH_OBSStudio not set!")
		return()
	endif()
	if(NOT EXISTS "${PATH_OBSStudio}/libobs/obs-module.h")
		message(FATAL_ERROR "PATH_OBSStudio invalid!")
		return()
	endif()

	# Find OBS Libraries
	SET(obsPath "${PATH_OBSStudio}")
	INCLUDE("${PATH_OBSStudio}/cmake/external/FindLibobs.cmake")

	# Compiling
	INCLUDE_DIRECTORIES(
		"${PROJECT_BINARY_DIR}"
		"${PROJECT_BINARY_DIR}/source"
		"${PROJECT_SOURCE_DIR}"
		"${PROJECT_SOURCE_DIR}/source"
		"${PATH_OBSStudio}"
	)
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# All Warnings, Extra Warnings, Pedantic
if(MSVC)
	# Force to always compile with W4
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
	# Update if necessary
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

################################################################################
# Build
################################################################################
ADD_LIBRARY(obs-stream-effects MODULE
	${obs-stream-effects_HEADERS}
	${obs-stream-effects_SOURCES}
	${obs-stream-effects_LOCALE}
	${obs-stream-effects_EFFECTS}
)
TARGET_LINK_LIBRARIES(obs-stream-effects
	${LIBOBS_LIBRARIES}
	${obs-stream-effects_LIBRARIES}
)

if(TARGET libobs)
	install_obs_plugin_with_data(obs-stream-effects data)
else()
	INSTALL(TARGETS obs-stream-effects DESTINATION "${INSTALL_DIR}/obs-plugins/${BITS}bit")
	INSTALL(FILES $<TARGET_PDB_FILE:obs-stream-effects> DESTINATION "${INSTALL_DIR}/obs-plugins/${BITS}bit" OPTIONAL)
	INSTALL(DIRECTORY "${PROJECT_SOURCE_DIR}/data/" DESTINATION "${INSTALL_DIR}/data/obs-plugins/obs-stream-effects" OPTIONAL)
	
	# Zip Generator
	ADD_CUSTOM_TARGET(PACKAGE_ZIP COMMAND
		${CMAKE_COMMAND} -E tar "cfv" "${INSTALL_DIR}/${PACKAGE_PREFIX}obs-stream-effects${PACKAGE_SUFFIX}.zip" --format=zip -- "${INSTALL_DIR}/data/" "${INSTALL_DIR}/obs-plugins"
		DEPENDS INSTALL obs-stream-effects
		WORKING_DIRECTORY "${INSTALL_DIR}")
	
	# 7-Zip Generator
	ADD_CUSTOM_TARGET(PACKAGE_7ZIP COMMAND
		${CMAKE_COMMAND} -E tar "cfv" "${INSTALL_DIR}/${PACKAGE_PREFIX}obs-stream-effects${PACKAGE_SUFFIX}.7z" --format=7zip -- "${INSTALL_DIR}/data/" "${INSTALL_DIR}/obs-plugins"
		DEPENDS INSTALL obs-stream-effects
		WORKING_DIRECTORY "${INSTALL_DIR}")
	
endif()
