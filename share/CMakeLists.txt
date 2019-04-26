# list of files for which we add a copy rule
set(resource_files
    nutation.txt
    ttut1_historic.txt
    ttut1_predicted.txt)

add_custom_target(resource-target ALL DEPENDS ${resource_files})
message(STATUS "Copying corrections files for testing")

foreach(item IN LISTS resource_files)
    add_custom_command(
        TARGET resource-target POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CPPEPHEM_SHARE_OUTPUT_DIRECTORY}/${item}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
    )
endforeach()
