if(DEBUG_ICTS_BST)
  message(STATUS "CTS: DEBUG_ICTS_BST")
  set(CMAKE_BUILD_TYPE "Debug")
else()
  message(STATUS "CTS: RELEASE_ICTS_BST")
  set(CMAKE_BUILD_TYPE "Release")
endif()

add_library(
  icts_bst
  ${ICTS_SOLVER}/GOCA/tools/tree_builder/bound_skew_tree/BoundSkewTree.cc
  ${ICTS_SOLVER}/GOCA/tools/tree_builder/bound_skew_tree/GeomCalc.cc)

target_link_libraries(
  icts_bst PUBLIC icts_api icts_goca_database icts_balance_clustering
                  icts_timing_propagator icts_tree_builder)

target_include_directories(
  icts_bst
  PUBLIC ${ICTS_SOLVER}/GOCA/database
         ${ICTS_SOLVER}/GOCA/tools/balance_clustering
         ${ICTS_SOLVER}/GOCA/tools/timing_propagator
         ${ICTS_SOLVER}/GOCA/tools/tree_builder)
