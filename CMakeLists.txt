project(LibMessageStudio C ASM)

add_library(LibMessageStudio OBJECT
  include/commonbin.h
  include/libms.h
  include/msgfile.h
  include/projfile.h
  include/flowchart.h
  include/types.h

  src/commonbin.c
  src/libms.c
  src/msgfile.c
  src/projfile.c
  src/flowchart.c
)

target_compile_options(LibMessageStudio PRIVATE -fno-exceptions)
target_compile_options(LibMessageStudio PRIVATE -fno-strict-aliasing)
target_compile_options(LibMessageStudio PRIVATE -Wno-invalid-offsetof)
target_include_directories(LibMessageStudio PUBLIC include/)