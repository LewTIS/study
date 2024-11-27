aux_source_directory(. SRC_LIST)

include_directories(../include)

add_executable(main ${SRC_LIST})

SET (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

