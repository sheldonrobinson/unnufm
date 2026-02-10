# Builds SpeechProcessor
cmake_minimum_required(VERSION 3.26)
project(unnufm LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
include(FetchContent)
include(ExternalProject)

find_package(OpenMP REQUIRED)
find_package(Python COMPONENTS Interpreter REQUIRED)

if (MINGW)
	set(BUILD_SHARED_LIBS_DEFAULT OFF)
else()
	set(BUILD_SHARED_LIBS_DEFAULT ON)
endif()

option(UFM_BUILD_SHARED_LIBS "Build Unnu Film Maker as shared library" ${BUILD_SHARED_LIBS_DEFAULT})

if (WIN32)
    add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
endif()

#################### MSVC vcvars
FetchContent_Declare(
  uf-find-vcvars
  GIT_REPOSITORY 		 https://github.com/scikit-build/cmake-FindVcvars.git
  GIT_TAG        		 origin/master
  GIT_SHALLOW 			 TRUE
  GIT_SUBMODULES_RECURSE TRUE
  
  EXCLUDE_FROM_ALL
)

FetchContent_Populate(uf-find-vcvars)

file(REAL_PATH ${uf-find-vcvars_SOURCE_DIR} FINDVCVARS_SOURCE_DIR)

list(INSERT CMAKE_MODULE_PATH 0 ${FINDVCVARS_SOURCE_DIR})

set(cmd_wrapper)
if(MSVC)
  set(Vcvars_MSVC_ARCH 64)
  set(Vcvars_FIND_VCVARSALL TRUE)
  find_package(Vcvars REQUIRED)
  set(cmd_wrapper ${Vcvars_LAUNCHER})
endif()

################## unnutts
FetchContent_Declare(
  unnutts
  GIT_REPOSITORY 		 https://github.com/sheldonrobinson/unnutts.git
  GIT_TAG        		 origin/develop
  GIT_SHALLOW 			 TRUE
  GIT_SUBMODULES_RECURSE TRUE
  
  EXCLUDE_FROM_ALL
		 
  OVERRIDE_FIND_PACKAGE
)

set(UT_BUILD_SHARED_LIBS ON CACHE BOOL "Build as shared library" Force)

FetchContent_MakeAvailable(unnutts)

file(REAL_PATH ${unnutts_SOURCE_DIR} UNNUTTS_SOURCE_DIR)

set(UNNUTTS_INCLUDE_DIR "${UNNUTTS_SOURCE_DIR}/include" "unnutts include dir" FORCE)

if(NOT TARGET nlohmann_json::nlohmann_json)
	################## nlohmann
	FetchContent_Declare(
	  ufnlohmann_json
	  GIT_REPOSITORY 		 https://github.com/nlohmann/json.git
	  GIT_TAG        		 v3.12.0
	  GIT_SHALLOW 			 TRUE
	  GIT_SUBMODULES_RECURSE TRUE
			 
	  OVERRIDE_FIND_PACKAGE
	)

	set(JSON_BuildTests OFF CACHE BOOL "Build the unit tests when BUILD_TESTING is enabled." FORCE)
	set(JSON_CI OFF CACHE BOOL "Enable CI build targets." FORCE)
	set(JSON_Diagnostics OFF CACHE BOOL  "Use extended diagnostic messages." FORCE)
	set(JSON_Diagnostic_Positions OFF CACHE BOOL "Enable diagnostic positions." FORCE)
	set(JSON_GlobalUDLs ON CACHE BOOL "Place user-defined string literals in the global namespace." FORCE)
	set(JSON_ImplicitConversions ON CACHE BOOL "Enable implicit conversions." FORCE)
	set(JSON_DisableEnumSerialization OFF CACHE BOOL "Disable default integer enum serialization." FORCE)
	set(JSON_LegacyDiscardedValueComparison OFF CACHE BOOL "Enable legacy discarded value comparison." FORCE)
	set(JSON_Install OFF CACHE BOOL "Install CMake targets during install step." FORCE)
	set(JSON_MultipleHeaders ON CACHE BOOL "Use non-amalgamated version of the library." FORCE)
	set(JSON_SystemInclude OFF CACHE BOOL "Include as system headers (skip for clang-tidy)." FORCE)

	FetchContent_MakeAvailable(ufnlohmann_json)

	file(REAL_PATH ${ufnlohmann_json_SOURCE_DIR} NLOHMANN_JSON_SOURCE_DIR)

	set(NLOHMANN_JSON_INCLUDE_DIR "${NLOHMANN_JSON_SOURCE_DIR}/single_include" "nholmann json include dir" FORCE)

	if(NOT TARGET nlohmann::json)
		add_library(nlohmann::json INTERFACE IMPORTED)
		set_target_properties(nlohmann::json PROPERTIES
							  INTERFACE_LINK_LIBRARIES nlohmann_json 							  
							  INTERFACE_POSITION_INDEPENDENT_CODE ON
							  )
	endif()
endif()

################## concurrentqueue
FetchContent_Declare(
  ufconcurrentqueue
  GIT_REPOSITORY 		 https://github.com/cameron314/concurrentqueue.git
  GIT_TAG        		 origin/master
  GIT_SHALLOW 			 TRUE
  GIT_SUBMODULES_RECURSE TRUE
		 
  OVERRIDE_FIND_PACKAGE
)

FetchContent_MakeAvailable(ufconcurrentqueue)

file(REAL_PATH ${ufconcurrentqueue_SOURCE_DIR} CONCURRENTQUEUE_SOURCE_DIR)

set(CONCURRENTQUEUE_INCLUDE_DIR "${CONCURRENTQUEUE_SOURCE_DIR}" "concurrentqueue include dir" FORCE)

if(NOT TARGET moodycamel::concurrentqueue)
	add_library(moodycamel::concurrentqueue INTERFACE IMPORTED)
    set_target_properties(moodycamel::concurrentqueue PROPERTIES
                          INTERFACE_LINK_LIBRARIES concurrentqueue  							  
						  INTERFACE_POSITION_INDEPENDENT_CODE ON
						  )
endif()



include_directories(${NLOHMANN_JSON_INCLUDE_DIR} ${UNNUTTS_INCLUDE_DIR} ${CONCURRENTQUEUE_SOURCE_DIR} ${LIBPIPER_INCLUDE_DIR})


if (UF_BUILD_SHARED_LIBS)
	add_library(unnufm SHARED src/unnufm.cpp)
else()
	add_library(unnufm STATIC src/unnufm.cpp)
endif()

target_link_libraries(unnufm PUBLIC piper unnutts concurrentqueue nlohmann_json nlohmann_json::nlohmann_json)


set_target_properties(unnufm PROPERTIES
  BUILD_WITH_INSTALL_RPATH TRUE
  LINKER_LANGUAGE CXX
  OUTPUT_NAME "unnufm"
  POSITION_INDEPENDENT_CODE True
)

target_include_directories(unnufm PUBLIC include)
target_compile_features   (unnufm PRIVATE cxx_std_17) # don't bump

if (UT_BUILD_SHARED_LIBS)
    set_target_properties(unnufm PROPERTIES POSITION_INDEPENDENT_CODE ON)
    target_compile_definitions(unnufm PRIVATE UNNU_FM_BUILD)
    target_compile_definitions(unnufm PUBLIC  UNNU_FM_SHARED)
endif()


install(TARGETS unnufm EXPORT unnufm_target
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

install(DIRECTORY include/ TYPE INCLUDE)
