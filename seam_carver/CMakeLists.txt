set(_target_name seam_carver)
file(GLOB _srcs ../src/*.cc)
include_directories(header)
add_library(${_target_name} ${_srcs})
target_link_libraries(${_target_name} ${OpenCV_LIBS})

