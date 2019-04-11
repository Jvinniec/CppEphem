# list of files for which we add a copy rule
set(resource_files
    corrections.txt)

add_custom_target(resource-target ALL DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/corrections.txt")

foreach(item IN LISTS resource_files)
    message(STATUS "Copying:\n     ${CMAKE_CURRENT_SOURCE_DIR}/${item}\n   ->\n     ${CPPEPHEM_SHARE_OUTPUT_DIRECTORY}/${item}")
    add_custom_command(
        TARGET resource-target POST_BUILD
        #OUTPUT "${CPPEPHEM_SHARE_OUTPUT_DIRECTORY}/${item}"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${item}" "${CPPEPHEM_SHARE_OUTPUT_DIRECTORY}/${item}"
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${item}"
    )
endforeach()
