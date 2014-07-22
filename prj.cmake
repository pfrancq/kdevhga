#ok, this file contain specific project definition
# - dependency checks
# - file to compile
# - sub project,
# - ...

#normaly all def in CMakeLists.txt
PROJECT(kdevhga CXX)

R_LOAD_DEPENDENCY("R" "R_LIB" "r")

#LINK_DIRECTORIES(${EXTRA_LIB_DIRECTORY})

FIND_PACKAGE(KDE4 REQUIRED)
#ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(SUB_PROJECT kdevhga)
ADD_SUBDIRECTORY(kdevhga)
