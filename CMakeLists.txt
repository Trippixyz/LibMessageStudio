project(LibMessageStudio CXX ASM)

add_library(NintendoSDK OBJECT
  include/LMS_Impl.h
  include/LMS_Mem.h
  include/LMS_Message.h
  include/LMS_Project.h
  include/LMS_Types.h

  src/LMS_Impl.c
  src/LMS_Mem.c
  src/LMS_Message.c
  src/LMS_Project.c
)

target_compile_options(LibMessageStudio PRIVATE -fno-exceptions)
target_compile_options(LibMessageStudio PRIVATE -fno-strict-aliasing)
target_compile_options(LibMessageStudio PRIVATE -Wno-invalid-offsetof)
target_include_directories(LibMessageStudio PUBLIC include/)