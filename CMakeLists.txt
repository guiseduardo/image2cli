cmake_minimum_required (VERSION 3.0)

get_filename_component(ProjectId ${CMAKE_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
string(TOLOWER ${ProjectId} ProjectId)
project(${ProjectId})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -g -std=c++2a -fconcepts" )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)

find_package(OpenCV REQUIRED)
# set(THIRD_PARTY ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdParty)
# get_filename_component(THIRD_PARTY ${THIRD_PARTY} ABSOLUTE)
# message(THIRD_PARTY="${THIRD_PARTY}")
#
# find_package(OpenCV REQUIRED PATHS
#     ${THIRD_PARTY}/opencv-4.x_build     # default path
#     ${THIRD_PARTY}/opencv-4.x_build/opencv/build  # github actions path
# )
include_directories(${OpenCV_INCLUDE_DIRS} ".")

file(GLOB_RECURSE SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADERS ${CMAKE_SOURCE_DIR}/inc/*.hpp)
include_directories(${CMAKE_SOURCE_DIR}/inc/)

add_custom_target(deploy)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_dependencies(deploy ${PROJECT_NAME})
target_link_libraries(${PROJECT_NAME} ${NOTIFY_LIBRARIES} dl ${OpenCV_LIBS})
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION "0")

file(GLOB APPS RELATIVE ${CMAKE_SOURCE_DIR}/app/ ${CMAKE_SOURCE_DIR}/app/*.cpp)

foreach(appsourcefile ${APPS})
	string(REPLACE ".cpp" "" appname ${appsourcefile})
	add_executable(${appname} ${CMAKE_SOURCE_DIR}/app/${appsourcefile})
	add_dependencies(deploy ${appname})
	target_link_libraries(${appname} ${PROJECT_NAME} ${OpenCV_LIBS})
	string(REPLACE ".cpp" "" TEMP ${appsourcefile})
	string(CONCAT BINARY "${CMAKE_SOURCE_DIR}/bin/" ${TEMP})
	list(APPEND BINS ${BINARY})
endforeach(appsourcefile ${APPS})

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/lib/ COMPONENT package_library)
install(PROGRAMS ${BINS} DESTINATION /usr/local/bin/ COMPONENT package_bins)
install(FILES ${HEADERS} DESTINATION /usr/local/include/${PROJECT_NAME} COMPONENT package_headers)
install(PROGRAMS ${DESKTOP} DESTINATION ~/.local/share/applications/ COMPONENT package_desktop)
if(EXISTS ${RESOURCES})
	install(DIRECTORY ${RESOURCES}/ DESTINATION /usr/local/share/${PROJECT_NAME} COMPONENT package_resources)
endif()
install(CODE "execute_process(COMMAND ldconfig)")

