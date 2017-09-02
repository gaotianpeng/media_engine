set(OUTPUT_BIN_DIR ${PROJECT_BINARY_DIR}/bin)
make_directory(${OUTPUT_BIN_DIR})
set(OUTPUT_LIB_DIR ${PROJECT_BINARY_DIR}/lib)
make_directory(${OUTPUT_LIB_DIR})

option(DEVELOP_MODE "Build captions" TRUE)

set(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BIN_DIR})
set(LIBRARY_OUTPUT_PATH ${OUTPUT_BIN_DIR})

if(MSVC_IDE)
	set(cmake_library_output_directory  ${output_bin_dir} CACHE PATH "build directory")
else()
	set(cmake_library_output_directory ${output_lib_dir} CACHE PATH "build directory")
endif()

macro(set_project_label_and_group FOLDER)
	set_target_properties(${TARGET_NAME} PROPERTIES FOLDER ${FOLDER})

	set(TARGET_LABEL "")
	if(TARGET_LABEL)
	  set_target_properties(${TARGET_NAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}${TARGET_NAME}")
	endif(TARGET_LABEL)
endmacro(set_project_label_and_group)

macro(me_add_library LIB_NAME)
	if(NOT TARGET_NAME)
	  set(TARGET_NAME ${LIB_NAME})
	endif(NOT TARGET_NAME)
	set(LIBRARY_TYPE ${ARGV1})
	if(NOT LIBRARY_TYPE OR "${LIBRARY_TYPE}" STREQUAL "FRAMEWORK")
	  set(LIBRARY_TYPE SHARED)
	endif()

	add_library(${LIB_NAME} ${LIBRARY_TYPE}
	  ${LIB_HEADERS}
	  ${LIB_SOURCES}
	  )	
	target_link_libraries(${LIB_NAME} ${LIB_INTERNAL_DEPS})
	set_project_label_and_group(${MyCategory})

	if(BUILD_AT_UNICODE)
	  add_definitions(-DUNICODE)
	endif(BUILD_AT_UNICODE)
endmacro(me_add_library)

macro(me_add_executable execName)

  if(NOT TARGET_NAME)
    set(TARGET_NAME ${execName})
  endif(NOT TARGET_NAME)

  add_executable(${TARGET_NAME} ${TARGET_SRC})
  target_link_libraries(${TARGET_NAME} ${LIB_INTERNAL_DEPS})

  set_project_label_and_group(${MyCategory})

  set_target_properties(${TARGET_NAME} PROPERTIES PROJECT_LABEL "${TARGET_LABEL}")
  set_target_properties(${TARGET_NAME} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")
  set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_OUTPUT_NAME "${TARGET_NAME}${cmake_debug_postfix}")
  set_target_properties(${TARGET_NAME} PROPERTIES RELEASE_OUTPUT_NAME "${TARGET_NAME}${cmake_release_postfix}")
  set_target_properties(${TARGET_NAME} PROPERTIES RELWITHDEBINFO_OUTPUT_NAME "${TARGET_NAME}${cmake_relwithdebinfo_postfix}")
  set_target_properties(${TARGET_NAME} PROPERTIES MINSIZEREL_OUTPUT_NAME "${TARGET_NAME}${cmake_minsizerel_postfix}")
  
  if(WIN32)
    set_target_properties(${TGTNAME} PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(${TGTNAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
  endif(WIN32)
  
  if( BUILD_AT_UNICODE )
    add_definitions(-DUNICODE)
  endif( BUILD_AT_UNICODE )
endmacro(me_add_executable)

set(CMAKE_LIBRARY_PATH ${PROJECT_SOURCE_DIR}/dependency/bin)
if(DEVELOP_MODE)
	set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ${LIBRARY_OUTPUT_PATH})
endif()
link_directories(${CMAKE_LIBRARY_PATH})