add_subdirectory(${IRT_UTILITY}/logger)
add_subdirectory(${IRT_UTILITY}/monitor)
add_subdirectory(${IRT_UTILITY}/reporter)

add_library(irt_utility INTERFACE)

target_link_libraries(irt_utility 
    INTERFACE
        irt_logger
        irt_monitor
        irt_reporter
        irt_api
        irt_source_external_libs
)

target_include_directories(irt_utility 
    INTERFACE
        ${IRT_UTILITY}
)
