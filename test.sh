# Build with AddressSanitizer
# cmake -S . -B build -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer"


cmake --build build && ./build/tests/tinyecs-tests