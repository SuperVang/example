# Most of this is derived from: https://searchcode.com/codesearch/view/15162231/ (THANKS :)
cmake_minimum_required(VERSION 2.8.4)

find_package(Boost COMPONENTS system thread signals filesystem QUIET) 
IF (NOT ${Boost_FOUND})
	message("Boost libraries (system thread signals) not intalled! ${Boost_LIBRARIES}  ${Boost_ERROR_REASON}")
	set(MONGOCLIENT_FOUND FALSE)
	return()
ENDIF()

link_directories ( ${Boost_LIBRARY_DIRS} )
include_directories ( ${Boost_INCLUDE_DIRS} )

if (NOT WIN32)
    FIND_PATH(MongoClient_INCLUDE_DIR version.h
      HINTS
      /usr/include/mongo
      )

    FIND_LIBRARY(MongoClient_LIBRARIES NAMES libmongoclient mongoclient
      HINTS
      /usr/lib
      )

    if (MongoClient_INCLUDE_DIR AND MongoClient_LIBRARIES)
      set(MONGOCLIENT_FOUND TRUE)
    else (MongoClient_INCLUDE_DIR AND MongoClient_LIBRARIES)
      set(MONGOCLIENT_FOUND FALSE)
    endif (MongoClient_INCLUDE_DIR AND MongoClient_LIBRARIES)

    if (MONGOCLIENT_FOUND)
      if (NOT MONGOCLIENT_FIND_QUIETLY)
	  message(STATUS "Found Mongoclient: ${MongoClient_LIBRARIES}")
      endif (NOT MONGOCLIENT_FIND_QUIETLY)
    else (MONGOCLIENT_FOUND)
      message(STATUS "Could NOT find Udev")
    endif (MONGOCLIENT_FOUND)

    mark_as_advanced(MongoClient_INCLUDE_DIR MongoClient_LIBRARIES)

endif(NOT WIN32)