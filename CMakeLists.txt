cmake_minimum_required (VERSION 2.6)
project (kxmlVersOpenFst)

# The version number.
set (Production_VERSION_MAJOR 1)
set (Production_VERSION_MINOR 0)




# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/kxmlVersOpenFstConfig.h.in"
  "${PROJECT_BINARY_DIR}/kxmlVersOpenFstConfig.h"
  )

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories ("$(PROJECT_SOURCE_DIR)/tinyxml2-master")
include_directories ("$(PROJECT_SOURCE_DIR)/openfst-1.5.0")

#ajout de la library
add_library(tinyxml tinyxml2-master/tinyxml2.cpp)

 
add_executable(ProductionMdT parserKXML.cpp)
#ajout des lien avec les librairies
target_link_libraries (ProductionMdT tinyxml)
#target_link_libraries (ProductionMdT tinystr)
#target_link_libraries (ProductionMdT tinyxmlerror)
#target_link_libraries (ProductionMdT tinyxmlparser)
#target_link_libraries (ProductionMdT xmltest)

