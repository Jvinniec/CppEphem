
include_directories (./)

aux_source_directory(./ SOFA_SOURCES) 
add_library (sofa_c ${SOFA_SOURCES})

target_link_libraries(sofa_c)
