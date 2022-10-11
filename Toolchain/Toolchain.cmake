
# See https://stackoverflow.com/a/53635241
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

set(TOOLS $ENV{HOME}/opt/cross)

set(CMAKE_CXX_COMPILER ${TOOLS}/bin/i686-voidos-g++)
set(CMAKE_C_COMPILER ${TOOLS}/bin/i686-voidos-gcc)
set(CMAKE_ASM_COMPILER ${TOOLS}/bin/i686-voidos-as)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR i686)


#set(CMAKE_CXX_STANDARD 20)
#set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-ffreestanding -O0 -Wall -Wextra -fno-exceptions -fno-rtti")
set(CMAKE_C_FLAGS "-ffreestanding -O0 -Wall -Wextra")

set(arch i386)
