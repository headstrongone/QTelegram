# set the input and output documentation paths

set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../src)

# configure the source Doxyfile by copying it and replacing all @variables@
set(DOXYGEN_CONFIGURED_INPUT ${DOXYGEN_OUTPUT_DIR}/Doxyfile)
configure_file(${DOXYGEN_OUTPUT_DIR}/Doxyfile ${DOXYGEN_CONFIGURED_INPUT} @ONLY)
#configure_file(${DOXYGEN_INPUT_DIR}/doc/header.html.in ${DOXYGEN_OUTPUT_DIR}/header.html @ONLY)

# copy the files needed by the documentation
#configure_file(${DOXYGEN_INPUT_DIR}/doc/doxygen.css ${DOXYGEN_OUTPUT_DIR}/html/doxygen.css COPYONLY)

# target setup
add_custom_target(doc ALL
                  COMMAND ${CMAKE_COMMAND} -E echo_append "Building Documentation..."
                  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIGURED_INPUT}
                  COMMAND ${CMAKE_COMMAND} -E echo "Done."
                  WORKING_DIRECTORY ${DOXYGEN_INPUT_DIR}
                  SOURCES Doxyfile)

# setup install rules
#install(DIRECTORY ${DOXYGEN_OUTPUT_DIR}/html
#        DESTINATION ${CMAKE_INSTALL_DOCDIR}
#        COMPONENT doc)
