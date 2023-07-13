file(GLOB_RECURSE src CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(engine STATIC ${src})
target_include_directories(engine PUBLIC include)
