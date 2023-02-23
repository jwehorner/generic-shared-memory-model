cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(generic_shared_memory_model)

#################
###  Options  ###
#################
option(BUILD_GENERIC_SHARED_MEMORY_MODEL_TESTS "Optionally download test dependancies and compile test cases." OFF)

############################
###  Configured Headers  ###
############################

#####################################
###  Global Compiler Definitions  ###
#####################################

##########################
###  Dependency Setup  ###
##########################

##########################
###  Global Variables  ###
##########################

#################################
###  Compiler Specific Fixes  ###
#################################

########################
###  Subdirectories  ###
########################
if(BUILD_GENERIC_SHARED_MEMORY_MODEL_TESTS) 
	add_subdirectory(test)
endif()
