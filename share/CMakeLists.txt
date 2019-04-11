# list of files for which we add a copy rule
set(resource_files
    corrections.txt)

foreach(item IN LISTS resource_files)
    message(STATUS "Copying ${item} to build directory")
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${item}"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CPPEPHEM_SHARE_OUTPUT_DIRECTORY}/${item}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
    )
endforeach()

add_custom_target(resource-target ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/corrections.txt")