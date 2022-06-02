add_library(tello INTERFACE)

target_include_directories(tello INTERFACE include)
target_link_libraries(tello INTERFACE ws2_32)

# target_source(tello src/<anysrcfile>)
