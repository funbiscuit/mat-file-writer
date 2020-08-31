add_executable(matfile_example
        ${CMAKE_CURRENT_SOURCE_DIR}/example.cpp
        )

target_include_directories(matfile_example
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
        )

target_link_libraries(matfile_example PRIVATE
        MatFileWriter::MatFileWriter
        )
