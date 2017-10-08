add_subdirectory(fmt)

add_library(optional INTERFACE)
target_include_directories(optional INTERFACE tl)

add_library(status_value INTERFACE)
target_include_directories(status_value INTERFACE .)
