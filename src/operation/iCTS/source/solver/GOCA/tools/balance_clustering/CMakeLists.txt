add_subdirectory(${ICTS_SOLVER}/GOCA/tools/balance_clustering/min_cost_flow)
add_subdirectory(${ICTS_SOLVER}/GOCA/tools/balance_clustering/mip)

if(DEBUG_ICTS_GOCA_BALANCE_CLUSTERING)
  message(STATUS "CTS: DEBUG_ICTS_GOCA_BALANCE_CLUSTERING")
  set(CMAKE_BUILD_TYPE "Debug")
else()
  message(STATUS "CTS: RELEASE_ICTS_GOCA_BALANCE_CLUSTERING")
  set(CMAKE_BUILD_TYPE "Release")
endif()

add_library(icts_goca_balance_clustering
            ${ICTS_SOLVER}/GOCA/tools/balance_clustering/BalanceClustering.cc)

target_link_libraries(
  icts_goca_balance_clustering
  PUBLIC icts_data_manager
         icts_api
         icts_goca_database
         icts_min_cost_flow
         icts_mip
         icts_goca_timing_propagator
         icts_goca_tree_builder)

target_include_directories(
  icts_goca_balance_clustering
  PUBLIC ${ICTS_SOLVER}/GOCA/database
         ${ICTS_SOLVER}/GOCA/tools/balance_clustering/min_cost_flow
         ${ICTS_SOLVER}/GOCA/tools/balance_clustering/mip
         ${ICTS_SOLVER}/GOCA/tools/timing_propagator
         ${ICTS_SOLVER}/GOCA/tools/tree_builder
         ${ICTS_DATABASE})