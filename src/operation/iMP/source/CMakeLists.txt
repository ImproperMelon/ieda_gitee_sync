## set path 
set(IMP_DATAMANAGER ${IMP_SOURCE}/data_manager)
set(IMP_MODULE ${IMP_SOURCE}/module)
set(IMP_SOLVER ${IMP_SOURCE}/solver)
set(IMP_UTILITY ${IMP_SOURCE}/utility)

## build
add_subdirectory(${IMP_DATAMANAGER})
add_subdirectory(${IMP_MODULE})
add_subdirectory(${IMP_SOLVER})
add_subdirectory(${IMP_UTILITY})

add_library(imp_source
    ${IMP_SOURCE}/MP.cc
)

target_link_libraries(imp_source
    PRIVATE
        imp-datamanager
        imp-structure
        imp-module
        # imp_utility
)

target_include_directories(imp_source
    PUBLIC
        ${IMP_SOURCE}
)
