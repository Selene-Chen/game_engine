file(GLOB_RECURSE src CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_executable(sanbox ${src})
target_include_directories(sanbox PUBLIC include)
target_link_libraries(sanbox PUBLIC engine)
