add_library(libc STATIC
        stdio.cpp
        sys/types.cpp)
target_compile_features(libc PUBLIC cxx_std_20 c_std_11)

