file(GLOB MOVCreatorDemo_src *.h *.cpp)

include_directories(../include)
link_directories(../lib)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ../bin)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ../bin)

add_executable (MOVCreatorDemo ${MOVCreatorDemo_src})

target_link_libraries(MOVCreatorDemo mg_Core_r mg_Engine_r mg_IO_r mg_Memory_r mg_QuicktimeWriter_r)